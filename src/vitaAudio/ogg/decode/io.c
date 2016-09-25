/*
 * libnogg: a decoder library for Ogg Vorbis streams
 * Copyright (c) 2014-2016 Andrew Church <achurch@achurch.org>
 *
 * This software may be copied and redistributed under certain conditions;
 * see the file "COPYING" in the source code distribution for details.
 * NO WARRANTY is provided with this software.
 */

#include "../ogg.h"
#include "../common.h"
#include "common.h"
#include "io.h"

 /*************************************************************************/
 /************************** Interface routines ***************************/
 /*************************************************************************/

uint8_t get8(stb_vorbis *handle)
{
	uint8_t byte;
	if (UNLIKELY((*handle->read_callback)(handle->opaque, &byte, 1) != 1)) {
		handle->eof = true;
		return 0;
	}
	return byte;
}

/*-----------------------------------------------------------------------*/

bool getn(stb_vorbis *handle, uint8_t *buffer, int count)
{
	if (UNLIKELY((*handle->read_callback)(handle->opaque,
		buffer, count) != count)) {
		handle->eof = true;
		return false;
	}
	return true;
}

/*-----------------------------------------------------------------------*/

void skip(stb_vorbis *handle, int count)
{
	const int64_t current = (*handle->tell_callback)(handle->opaque);
	if (count > handle->stream_len - current) {
		count = handle->stream_len - current;
		handle->eof = true;
	}
	(*handle->seek_callback)(handle->opaque, current + count);
}

/*************************************************************************/
/*************************************************************************/
