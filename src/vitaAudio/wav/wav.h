#ifndef WavFile_H
#define WavFile_H

#include <stdint.h>

#define ERROR_UNKNOWN_FORMAT        (-1)
#define ERROR_READSIZE_IS_TOO_SMALL (-2)
#define ERROR_ILLEGAL_LOOPDATA      (-3)

#define _getShort(a, b) _getN(a, b, sizeof(uint16_t))
#define _getLong(a, b) _getN(a, b, sizeof(uint32_t))

/* PCM */
#define WAVE_FORMAT_PCM             (0x0001)

typedef struct WaveHeader
{
	uint32_t	nChannels;		/* Number of Channels */
	uint32_t	samplingRate;	/* Sampling Rate */
	uint32_t	nBlockAlign;	/* frame_length */
	uint32_t	bits;			/* bit/sample */
	uint32_t	headerByte;		/* header length */
	uint32_t	dataChunkSize;	/* data length */

} WaveHeader;


int32_t ParseWaveHeader(WaveHeader *pHeader,uint8_t *pucCodecAddr,uint32_t uiInputByte);

#endif
