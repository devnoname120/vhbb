

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <psp2/audioout.h>
#include <psp2/io/fcntl.h>

#include "vitaAudio.h"
#include "wav/wav.h"
#include "ogg/ogg.h"




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

	
	
	
int vchannel	= 0;
static int32_t readWavHeader( vaudio* audioHandler );

static int32_t soundThread( uint32_t args, void *argc );
static int32_t readThread(  uint32_t args, void *argc );

// OGG CALLBACKS
static int32_t streaming_read( void *opaque, void *buffer, int32_t length )
	{
	return (int32_t)sceIoRead( (SceUID)opaque, buffer, (size_t)length );
	}

static void streaming_close( void *opaque )
	{
	sceIoClose( (SceUID)opaque );
	}

static const vorbis_callbacks_t streaming_callbacks = 	{
														.read  = streaming_read,
														.close = streaming_close,
														};


// READ WAV HEADER
static int32_t readWavHeader( vaudio* audioHandler )
	{
	int32_t    ret = 0;
	uint32_t   readSize;
	uint32_t   readHeaderSize;
	uint8_t*   ptr;
	WaveHeader header;
	// RESET OFFSET
	ret = sceIoLseek( audioHandler->fileHandle, 0, SCE_SEEK_SET );
	if ( ret < 0 ) 	{ goto term; }
	
	// GET FILESIZE
	ret = sceIoLseek( audioHandler->fileHandle, 0, SCE_SEEK_END );
	if ( ret < 0 )	{ goto term; }
	if ( ret >= HEADER_SIZE ) 	{ readHeaderSize = HEADER_SIZE; }
	else 						{ readHeaderSize = audioHandler->dataSize; }
	
	// RESET OFFSET AGAIN
	ret = sceIoLseek( audioHandler->fileHandle, 0, SCE_SEEK_SET );
	if ( ret < 0 ) 	{ goto term; }
	
	ptr 		= audioHandler->header;
	readSize 	= readHeaderSize;

	// READ WAVE HEADER
	while ( readSize > 0 )
		{
		ret 		= sceIoRead( audioHandler->fileHandle, ptr, readSize );
		if ( ret < 0 ) { goto term; }

		readSize   -= ret;
		ptr 	   += ret;
		}

	// PARSE WAVE HEADER ( GET FMT CHUNK VALUE )
	ret 		= ParseWaveHeader( &header, audioHandler->header, readHeaderSize );
	if ( ret < 0 ) 	{ goto term; }

	// PROCEED FILE POINTER TO DATA CHUNK
	ret 		= sceIoLseek( audioHandler->fileHandle, header.headerByte, SCE_SEEK_SET );
	if ( ret < 0 )	{ goto term; }

	audioHandler->dataSize		= header.dataChunkSize;
	audioHandler->channels		= header.nChannels;
	audioHandler->samplingRate	= header.samplingRate;

  term:
	return ret;
	}



static int32_t soundThread( uint32_t args, void *argc )
	{
	int32_t  ret = 0;
	int32_t  portId;
	uint32_t side;
	int32_t  vol[STEREO];
	int32_t  size;
	int32_t  portType;
	int32_t  samplingRate;
	int32_t  param;
	vaudio* audioHandler;
	printf ( "# Start-SoundThread\n" );
	audioHandler = *( vaudio** )(argc);
	vol[0] = vol[1] = SCE_AUDIO_VOLUME_0DB;

	// SET PORT TYPE, SAMPLING RATE, CHANNELS
	if ( audioHandler->mode == AUDIO_OUT_MAIN )
		{
		portType     = SCE_AUDIO_OUT_PORT_TYPE_MAIN;
		samplingRate = 48000;
		if ( audioHandler->channels == 2 ) 	{ param = SCE_AUDIO_OUT_MODE_STEREO; } 
		else 								{ param = SCE_AUDIO_OUT_MODE_MONO; 	 }
		} 
	else
		{
		portType     = SCE_AUDIO_OUT_PORT_TYPE_BGM;
		samplingRate = audioHandler->samplingRate;
		if ( audioHandler->channels == 2 )  { param = SCE_AUDIO_OUT_MODE_STEREO; } 
		else 								{ param = SCE_AUDIO_OUT_MODE_MONO;   }
		}

	// OPEN PORT
	portId 		= sceAudioOutOpenPort( portType, USER_GRAIN, samplingRate, param );
	if ( portId < 0 ) 	{ goto term; }

	// SET VOLUME
	sceAudioOutSetVolume( portId, (SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH), vol );

	side = 0;
	// WAIT FOR BUFFER FILL
	for (;;) 
		{
		size = GetBufferCurrentCount( audioHandler->fileBuffer );
		if ( size >= FILEBUF_SIZE || audioHandler->readEnd || audioHandler->endflag )
			{ break; }
		sceKernelDelayThread ( 1000 );
		}

	for (;;)
		{
		if ( audioHandler->endflag || ( GetBufferCurrentCount( audioHandler->fileBuffer ) < USER_GRAIN * STEREO * sizeof(short) && audioHandler->readEnd ) )
			{ break; }

		// AUDIO OUTPUT
		if ( GetBufferCurrentCount( audioHandler->fileBuffer ) >= USER_GRAIN * STEREO * sizeof(short) )
			{
			GetBufferData( audioHandler->fileBuffer, (SceUChar8*)( audioHandler->wavBuff[side] ), USER_GRAIN * STEREO * sizeof(short) );
			ret 		= sceAudioOutOutput( portId, audioHandler->wavBuff[side] );
			if ( ret < 0 ) { break; }
			side ^= 0x01;
			} 
		else
			{
			// ***BUFFER UNDERFLOW!!***
			printf ( "currentCount = %u\n", GetBufferCurrentCount( audioHandler->fileBuffer ) );
			sceKernelDelayThread( 1000 );
			}
		}
	sceAudioOutOutput( portId, NULL );

	// TERMINATE FUNCTION
	ret 		= sceAudioOutReleasePort( portId );
	if ( ret < 0 ) { goto term; }

term:
	printf( "# End-SoundThread\n" );
	audioHandler->endflag 	= AUDIO_STATUS_END;
	ret 					= sceKernelExitDeleteThread(0);
	return ret;
	}



static int32_t readThread( uint32_t args, void *argc )
	{
	int32_t  ret 		= 0;
	uint32_t readSize 	= 0;
	uint32_t capacity;
	vaudio* audioHandler;
	printf ( "# Start-ReadThread\n" );
	audioHandler 		= *( vaudio** )(argc);
	readSize 			= audioHandler->dataSize;
	if ( audioHandler->format == 0 )
		{
		if ( audioHandler->memory == 1 )
			{
			int32_t  readPosition = 0;

			// READ WAVE DATA FROM MEMORY
			while ( readSize > 0 || audioHandler->endflag == AUDIO_STATUS_END )
				{
				capacity = GetBufferCapacity( audioHandler->fileBuffer );
				if ( capacity > 0 )
					{
					if ( capacity > READBUF_SIZE ) 	{ capacity = READBUF_SIZE; 	}
					if ( capacity > readSize ) 		{ capacity = readSize; 		}
					if ( capacity < 0 ) { goto term; }
					SetBufferData( audioHandler->fileBuffer, audioHandler->fillBuf + readPosition, capacity );					
					readSize 		-= capacity;
					readPosition 	+= capacity;
					}
				sceKernelDelayThread( 1000 );
				}
			}
		else
			{
			// READ WAVE DATA
			while ( readSize > 0 || audioHandler->endflag == AUDIO_STATUS_END )
				{
				capacity 	= GetBufferCapacity(audioHandler->fileBuffer);
				if ( capacity > 0 )
					{
					if ( capacity > READBUF_SIZE ) 	{ capacity = READBUF_SIZE; 	}
					if ( capacity > readSize ) 		{ capacity = readSize; 		}
					ret 		= sceIoRead( audioHandler->fileHandle, audioHandler->readBuf, capacity );
					if ( ret < 0 ) { goto term; }
					SetBufferData( audioHandler->fileBuffer, audioHandler->readBuf, ret );
					readSize 		-= ret;
					}
				sceKernelDelayThread(1000);
				}
			}
		}
	else if ( audioHandler->format == 1 )
		{
		int count = 1;
		while ( count > 0 || audioHandler->endflag == AUDIO_STATUS_END )
			{
			capacity = GetBufferCapacity( audioHandler->fileBuffer );
			if ( capacity > 0 )
				{
				if ( capacity > READBUF_SIZE ) { capacity = READBUF_SIZE; }
				int buffSize 		= capacity;
				vorbis_error_t error;
				int16_t* buf 		= malloc( sizeof(int16_t) * buffSize );
				int buffer_len 		= buffSize / 4;
				if ( buffer_len > 0 )
					{
					count 		= vorbis_read_int16( (vorbis_t *)audioHandler->handle, buf, buffer_len, &error );
					if ( count < 0 ) { goto term; }
					if ( error == VORBIS_ERROR_DECODE_RECOVERED ) { goto term; }
					else if ( error && error != VORBIS_ERROR_STREAM_END )
						{
						// STREAMING ERROR
						if ( 		error == VORBIS_ERROR_DECODE_FAILED ) 				{  }
						else if (	error == VORBIS_ERROR_INSUFFICIENT_RESOURCES) 		{  }
						else 															{  }
						goto term;
						}
					/*memcpy(audioHandler->readBuf, buf, count * 4);
					free(buf);
					SetBufferData(audioHandler->fileBuffer, audioHandler->readBuf, count * 4);*/
					SetBufferData( audioHandler->fileBuffer, buf, count * 4 );
					free( buf );
					}
				}
			sceKernelDelayThread(1000);
			}
		}
	
term:
	printf( "# End-ReadThread\n" );
	audioHandler->readEnd 	= AUDIO_READFLAG_READEND;
	ret 					= sceKernelExitDeleteThread(0);
	return ret;
	}



int32_t TestOgg( vaudio* audioHandler )
	{
	int count 		= 1;
	int readData 	= 0;
	while ( count > 0 )
		{
		int capacity 		= READBUF_SIZE;
		int buffSize 		= capacity / 2;
		vorbis_error_t error;
		int16_t* buf 		= malloc( sizeof(int16_t) * buffSize );
		int buffer_len 		= buffSize / 4;
		count 				= vorbis_read_int16( (vorbis_t *)audioHandler->handle, buf, buffer_len, &error );
		readData 		   += count;
		free( buf );
		}
	printf( "Samples read %d\n", readData );
	return 0;
	}



int32_t InitializeAudio( vaudio* audioHandler )
	{
	int32_t ret 	= 0;
	memset( audioHandler, 0, sizeof(vaudio) );
	audioHandler->fileHandle 	= -1;
	audioHandler->buffer 		= (uint8_t*)malloc(FILEBUF_SIZE);
	// ERROR HANDLING
	if ( audioHandler->buffer  	  == NULL )  	{ ret = AUDIO_ERROR_OUT_OF_MEMORY; goto error; }
	audioHandler->header 		= (uint8_t*)malloc(HEADER_SIZE);
	if ( audioHandler->header  	  == NULL ) 	{ ret = AUDIO_ERROR_OUT_OF_MEMORY; goto error; }
	audioHandler->readBuf 		= (uint8_t*)malloc(READBUF_SIZE);
	if ( audioHandler->readBuf    == NULL ) 	{ ret = AUDIO_ERROR_OUT_OF_MEMORY; goto error; }
	audioHandler->fileBuffer 	= (FileBuffer*)malloc(sizeof(FileBuffer));
	if ( audioHandler->fileBuffer == NULL ) 	{ ret = AUDIO_ERROR_OUT_OF_MEMORY; goto error; }
	ret 						= CreateBuffer(audioHandler->fileBuffer, audioHandler->buffer, FILEBUF_SIZE);
	if ( ret < 0 )  							{ goto error; }

	// CREATE SOUND THREAD
	audioHandler->soundThread 	= sceKernelCreateThread(
														SOUND_THREAD_NAME,
														soundThread,
														SCE_KERNEL_DEFAULT_PRIORITY_USER,
														SOUND_THREAD_STACK_SIZE,
														0,
														SCE_KERNEL_CPU_MASK_USER_ALL,
														0
														);

	if ( audioHandler->soundThread < 0 )
		{
		printf( "Error: sceKernelCreateThread 0x%08x\n", audioHandler->soundThread );
		ret 		= audioHandler->soundThread;
		goto error;
		}

	// CREATE READ THREAD
	audioHandler->readThread 	= sceKernelCreateThread(
														READ_THREAD_NAME,
														readThread,
														SCE_KERNEL_DEFAULT_PRIORITY_USER + 1,
														READ_THREAD_STACK_SIZE,
														0,
														SCE_KERNEL_CPU_MASK_USER_ALL,
														0
														);

	if ( audioHandler->readThread < 0 )
		{
		printf( "Error: sceKernelCreateThread 0x%08x\n", audioHandler->readThread );
		ret 		= audioHandler->readThread;
		goto error;
		}
	goto term;

error:
	if ( audioHandler->readThread > 0 	) { sceKernelDeleteThread( audioHandler->readThread ); audioHandler->readThread = 0; }
	if ( audioHandler->soundThread > 0 	) { sceKernelDeleteThread ( audioHandler->soundThread ); audioHandler->soundThread = 0; }
	if ( audioHandler->fileBuffer 		) { DeleteBuffer( audioHandler->fileBuffer ); free( audioHandler->fileBuffer ); audioHandler->fileBuffer = NULL; }
	if ( audioHandler->readBuf 			) { free( audioHandler->readBuf ); audioHandler->readBuf = NULL; }
	if ( audioHandler->buffer 			) { free( audioHandler->buffer ); audioHandler->buffer = NULL; }
	if ( audioHandler->header 			) { free( audioHandler->header ); audioHandler->header = NULL; }

term:
	return ret;
	}



int32_t TerminateAudio( vaudio* audioHandler )
	{
	audioHandler->endflag 	= AUDIO_STATUS_END;
	if ( audioHandler->readThread > 0 	) { sceKernelWaitThreadEnd ( audioHandler->readThread, NULL, 0 ); }
	if ( audioHandler->soundThread > 0 	) { sceKernelWaitThreadEnd ( audioHandler->soundThread, NULL, 0 ); }
	if ( audioHandler->readThread > 0 	) { sceKernelDeleteThread ( audioHandler->readThread ); audioHandler->readThread = 0; }
	if ( audioHandler->soundThread > 0 	) { sceKernelDeleteThread ( audioHandler->soundThread ); audioHandler->soundThread = 0; }
	if ( audioHandler->fileHandle >= 0 	) { sceIoClose ( audioHandler->fileHandle ); audioHandler->fileHandle = -1; }
	if ( audioHandler->fileBuffer 		) { DeleteBuffer ( audioHandler->fileBuffer ); free ( audioHandler->fileBuffer ); audioHandler->fileBuffer = NULL;}
	if ( audioHandler->readBuf 			) { free ( audioHandler->readBuf ); audioHandler->readBuf = NULL; }
	if ( audioHandler->buffer 			) { free ( audioHandler->buffer ); audioHandler->buffer = NULL; }
	if ( audioHandler->header 			) { free ( audioHandler->header ); audioHandler->header = NULL; }
	return 0;
	}



int32_t LoadOgg( vaudio* audioHandler, char* filename, SceUInt32 mode, SceUInt32 memory )
	{
	int32_t ret 				= 0;
	vorbis_t *handle 			= NULL;
	vorbis_error_t error;
	const unsigned int options 	= 0;
	audioHandler->mode 			= mode;
	audioHandler->memory 		= memory;
	audioHandler->format 		= 1; // 0 = wav, 1 = ogg
	audioHandler->endflag 		= 0;
	audioHandler->readEnd 		= 0;
	audioHandler->fileHandle 	= sceIoOpen( filename, SCE_O_RDONLY, 0 );
	if ( audioHandler->fileHandle < 0 )
		{
		printf( "sceIoOpen() 0x%08x\n", audioHandler->fileHandle );
		ret 		= audioHandler->fileHandle;
		goto error;
		}
	handle 		= vorbis_open_callbacks( streaming_callbacks, audioHandler->fileHandle, options, &error );
	if ( !handle )
		{
		// ERROR HANDLING
		if ( error == VORBIS_ERROR_INSUFFICIENT_RESOURCES 	) 	{ }
		else if ( error == VORBIS_ERROR_STREAM_INVALID 		) 	{ }
		else if ( error == VORBIS_ERROR_STREAM_END 			) 	{ }
		else if ( error == VORBIS_ERROR_DECODE_SETUP_FAILED ) 	{ }
		else 													{ }
		ret 		= -1;
		goto error;
		}
	// READ HEADER
	audioHandler->handle 		= handle;
	const int channels 			= vorbis_channels(handle);
	const uint32_t rate 		= vorbis_rate(handle);
	audioHandler->channels 		= channels;
	audioHandler->samplingRate 	= rate;
	goto term;

error:
	audioHandler->endflag 		= AUDIO_STATUS_END;
	if ( audioHandler->readThread > 0  ) { sceKernelWaitThreadEnd( audioHandler->readThread, NULL, 0 ); }
	if ( audioHandler->soundThread > 0 ) { sceKernelWaitThreadEnd( audioHandler->soundThread, NULL, 0 ); }
	if ( audioHandler->readThread > 0  ) { sceKernelDeleteThread( audioHandler->readThread ); audioHandler->readThread = 0; }
	if ( audioHandler->soundThread > 0 ) { sceKernelDeleteThread( audioHandler->soundThread ); audioHandler->soundThread = 0; }
	if ( audioHandler->fileHandle >= 0 ) { sceIoClose( audioHandler->fileHandle ); audioHandler->fileHandle = -1; }

term:
	return ret;
	}



int32_t LoadWav( vaudio* audioHandler, char* filename, SceUInt32 mode, SceUInt32 memory )
	{
	int32_t ret 				= 0;
	audioHandler->mode 			= mode;
	audioHandler->memory 		= memory;
	audioHandler->endflag 		= 0;
	audioHandler->readEnd 		= 0;
	audioHandler->format 		= 0; // 0 = wav, 1 = ogg
	audioHandler->fileHandle 	= sceIoOpen( filename, SCE_O_RDONLY, 0 );
	if ( audioHandler->fileHandle < 0 )
		{
		printf( "sceIoOpen() 0x%08x\n", audioHandler->fileHandle );
		ret 		= audioHandler->fileHandle;
		goto error;
		}
	ret 		= readWavHeader( audioHandler );
	if ( ret < 0 ) { goto term; }
	if ( audioHandler->memory == 1 )
		{
		// LOAD DATA FROM WAVE FILE
		uint32_t dataSize 		= audioHandler->dataSize;
		// ALLOC DATA BUFFER
		audioHandler->fillBuf 	= (uint8_t*)malloc(dataSize);
		if ( audioHandler->fillBuf == NULL ) { ret = AUDIO_ERROR_OUT_OF_MEMORY; goto error; }
		ret 	= sceIoRead( audioHandler->fileHandle, audioHandler->fillBuf, dataSize );
		if ( ret < 0 ) { goto term; }
		}
	goto term;

error:
	audioHandler->endflag 	= AUDIO_STATUS_END;
	if ( audioHandler->readThread  > 0 ) { sceKernelWaitThreadEnd( audioHandler->readThread, NULL, 0 ); }
	if ( audioHandler->soundThread > 0 ) { sceKernelWaitThreadEnd( audioHandler->soundThread, NULL, 0 ); }
	if ( audioHandler->readThread  > 0 ) { sceKernelDeleteThread( audioHandler->readThread ); audioHandler->readThread = 0; }
	if ( audioHandler->soundThread > 0 ) { sceKernelDeleteThread(audioHandler->soundThread); audioHandler->soundThread = 0; }
	if ( audioHandler->fileHandle >= 0 ) { sceIoClose( audioHandler->fileHandle ); audioHandler->fileHandle = -1; }

term:
	return ret;
	}



int32_t PlayAudio( vaudio* audioHandler )
	{
	int32_t ret 	= 0;
	ret 			= sceKernelStartThread( audioHandler->readThread, sizeof(audioHandler), &audioHandler );
	if ( ret < 0 ) { goto term; }
	ret 			= sceKernelStartThread( audioHandler->soundThread, sizeof(audioHandler), &audioHandler );
	if ( ret < 0 ) { goto term; }
	goto term;

error:
	audioHandler->endflag = AUDIO_STATUS_END;
	if ( audioHandler->readThread  > 0 ) { sceKernelWaitThreadEnd( audioHandler->readThread,  NULL, 0 ); }
	if ( audioHandler->soundThread > 0 ) { sceKernelWaitThreadEnd( audioHandler->soundThread, NULL, 0 ); }
	if ( audioHandler->readThread  > 0 ) { sceKernelDeleteThread( audioHandler->readThread ); audioHandler->readThread = 0; }
	if ( audioHandler->soundThread > 0 ) { sceKernelDeleteThread( audioHandler->soundThread ); audioHandler->soundThread = 0; }
	if ( audioHandler->fileHandle >= 0 ) { sceIoClose( audioHandler->fileHandle ); audioHandler->fileHandle = -1; }

term:
	return ret;
	}



int32_t StopAudio( vaudio* audioHandler )
	{
	audioHandler->endflag = AUDIO_STATUS_END;
	if ( audioHandler->readThread  > 0 ) { sceKernelWaitThreadEnd( audioHandler->readThread, NULL, NULL ); }
	if ( audioHandler->soundThread > 0 ) { sceKernelWaitThreadEnd( audioHandler->soundThread, NULL, NULL ); }
	if (audioHandler->readThread   > 0 ) { sceKernelDeleteThread( audioHandler->readThread ); audioHandler->readThread = 0; }
	if (audioHandler->soundThread  > 0 ) { sceKernelDeleteThread( audioHandler->soundThread ); audioHandler->soundThread = 0; }
	if ( audioHandler->fileHandle >= 0 ) { sceIoClose( audioHandler->fileHandle ); audioHandler->fileHandle = -1; }
	return 0;
	}



uint32_t GetAudioStatus( vaudio* audioHandler ) { return audioHandler->endflag; }





//-----------------------------------------
// 	vaudio snd_my_sound;
//  vaudio_play_sound_wav( snd_my_sound, "mysound.wav" );
void vaudio_play_sound_wav( vaudio *audioHandler, char* filename )
	{
	StopAudio( audioHandler );
	InitializeAudio( audioHandler );
	LoadWav( audioHandler, filename, AUDIO_OUT_MAIN, vchannel );
	PlayAudio( audioHandler );
	vchannel++;
	if ( vchannel >= 8 ) { vchannel = 0; }
	}

void vaudio_play_sound_ogg( vaudio *audioHandler, char* filename )
	{
	StopAudio( audioHandler );
	InitializeAudio( audioHandler );
	LoadOgg( audioHandler, filename, AUDIO_OUT_MAIN, vchannel );
	PlayAudio( audioHandler );
	vchannel++;
	if ( vchannel >= 8 ) { vchannel = 0; }
	}

void vaudio_play_music_wav( vaudio *audioHandler, char* filename )
	{
	int32_t is 	= 0;
	is 			= GetAudioStatus( audioHandler );
	if ( is == AUDIO_STATUS_END ) { StopAudio( audioHandler ); }
	InitializeAudio( audioHandler );
	LoadWav( audioHandler, filename, AUDIO_OUT_BGM, 0 );
	PlayAudio( audioHandler );
	}

void vaudio_play_music_ogg( vaudio *audioHandler, char* filename )
	{
	int32_t is 	= 0;
	is 			= GetAudioStatus( audioHandler );
	if ( is == AUDIO_STATUS_END ) { StopAudio( audioHandler ); }
	InitializeAudio( audioHandler );
	LoadOgg( audioHandler, filename, AUDIO_OUT_BGM, 0 );
	PlayAudio( audioHandler );
	}

void vaudio_stop( vaudio *audioHandler )
	{
	StopAudio( audioHandler );
	}
	
int vaudio_is_playing( vaudio *audioHandler )
	{
	int32_t is 	= 0;
	is 			= GetAudioStatus( audioHandler );
	if ( is == AUDIO_STATUS_END ) { return 0; }
	else						  { return 1; }
	}

void vaudio_free( vaudio *audioHandler )
	{
	StopAudio( audioHandler );
	TerminateAudio( audioHandler );
	}