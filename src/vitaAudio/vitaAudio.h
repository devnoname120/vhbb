#ifndef _AUDIO_H
#define _AUDIO_H

#include <stdint.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>

#include "buffer/buffer.h"

typedef struct
	{
	SceUID			soundThread;
	SceUID			readThread;
	SceUID			fileHandle;
	uint8_t*		header;
	uint8_t*		buffer;
	uint8_t*		readBuf;
	uint8_t*		fillBuf;
	uint32_t		dataSize;
	uint32_t		offset;
	uint32_t		readEnd;
	uint32_t		endflag;
	uint32_t		memory;
	uint32_t		format;
	FileBuffer*		fileBuffer;
	void*			handle;
	uint32_t		mode;
	uint32_t		channels;
	uint32_t		samplingRate;
	int16_t			wavBuff[BUFNUM][USER_GRAIN*STEREO];
	} vaudio;
// Specify a vitaAudio variable
// vaudio snd_text_soud;


void vaudio_play_sound_wav( vaudio *audioHandler, char* filename );

void vaudio_play_sound_ogg( vaudio *audioHandler, char* filename );

void vaudio_play_music_wav( vaudio *audioHandler, char* filename );

void vaudio_play_music_ogg( vaudio *audioHandler, char* filename );

void vaudio_stop( vaudio *audioHandler );
	
int vaudio_is_playing( vaudio *audioHandler );

void vaudio_free( vaudio *audioHandler );

#endif // _AUDIO_H
