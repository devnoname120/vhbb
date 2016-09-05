#include <stdio.h>
#include <string.h>

#include "wav.h"

static uint32_t _getN(uint8_t *pucCodecAddr,uint32_t *pReadbyte,uint32_t size);

int32_t ParseWaveHeader(WaveHeader *pHeader,uint8_t *pucCodecAddr,uint32_t uiInputByte)
{
	uint32_t chunkLength;
	int32_t  format;
	uint32_t readbyte = 0;

	pHeader->dataChunkSize = 0;
	format = ERROR_UNKNOWN_FORMAT;

	if (uiInputByte < readbyte + 12) {
		return ERROR_READSIZE_IS_TOO_SMALL;
	}
	if (_getLong(pucCodecAddr, &readbyte) != 0x46464952) {/* 'RIFF' */
		return format;
	}
	_getLong(pucCodecAddr, &readbyte) - 4; /* skip rLen, RIFF chunk length */
	if (_getLong(pucCodecAddr, &readbyte) != 0x45564157) {/* 'WAVE' */
		return format;
	}

	/*E Processing loop per chunk */
	/*J チャンクごとの処理ループ */
	while (readbyte + 8 < uiInputByte)
	{
		uint32_t chunkType = _getLong(pucCodecAddr, &readbyte);
		chunkLength = _getLong(pucCodecAddr, &readbyte);
		chunkLength += (chunkLength & 1);

		if (readbyte + chunkLength > uiInputByte
			&& chunkType != 0x61746164 /* 'data' */)
		{
			/*E Give up analyzing because the chunk is not read completely */
			/*J 読み込みが不完全な chunk なので解析を諦める */

			return ERROR_READSIZE_IS_TOO_SMALL;
		}
		switch (chunkType) {
		case 0x20746d66: /* 'fmt ' */
			if (format != (int)ERROR_UNKNOWN_FORMAT) {
				/*E fmt chunk appears redundantly */
				/*J fmt chunk が二重に出現 */
				return ERROR_UNKNOWN_FORMAT;
			}

			/* wFormatTag */
			chunkType = _getShort(pucCodecAddr, &readbyte);
			/*E Channel num */
			/*J チャネル数 */
			pHeader->nChannels = _getShort(pucCodecAddr, &readbyte);
			if (pHeader->nChannels == 0 || pHeader->nChannels > 2) {
				/*E Support only up to the stereo */
				/*J ステレオまでのサポート */
				return ERROR_UNKNOWN_FORMAT;
			}

			/* sampling rate */
			pHeader->samplingRate = _getLong(pucCodecAddr, &readbyte);

			/* skip 4 Byte */
			readbyte += 4;

			/*E block size (SU size/framelen) */
			/*J ブロックサイズ (SU size/framelen) */
			pHeader->nBlockAlign = _getShort(pucCodecAddr, &readbyte);
			if (pHeader->nBlockAlign == 0) {
				return ERROR_UNKNOWN_FORMAT;
			}

			/*E Number of bits per sample */
			/*J サンプルあたりのビット数 */
			pHeader->bits = _getShort(pucCodecAddr, &readbyte);

			switch (chunkType) {
			case WAVE_FORMAT_PCM:       /* PCM */
				chunkLength -= 2 + 2 + 8 + 2 + 2;
				format = WAVE_FORMAT_PCM;
				break;

			default:	/* unsupported wFormatTag */
				return ERROR_UNKNOWN_FORMAT;
			}
			break;

		case 0x61746164: /* 'data' */
			/*E Normally ends when it reaches to data chunk */
			/*J data チャンクに到達したら正常終了する */
			pHeader->dataChunkSize = chunkLength;
			pHeader->headerByte = readbyte;

			return format;

		default:	/* unknown chunk */
			break;
		}

		/* skip remain of chunk */
		readbyte += chunkLength;
		if (readbyte > uiInputByte) {
			break;
		}
	}
	/*E Did not reach to data chunk */
	/*J data chunk までたどりつかなかった */
	return ERROR_READSIZE_IS_TOO_SMALL;
}

static uint32_t _getN(uint8_t *pucCodecAddr,uint32_t *pReadbyte,uint32_t size)
{
	uint32_t ret = 0;
	uint32_t end;

	end = *pReadbyte + size;
	do
	{
		size--;
		ret |= pucCodecAddr[*pReadbyte+size] << (size*8);
	} while (size > 0);

	*pReadbyte = end;
	return ret;
}
