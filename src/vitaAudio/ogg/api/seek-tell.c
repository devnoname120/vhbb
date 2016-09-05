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
#include "../util/decode-frame.h"


int vorbis_seek(vorbis_t *handle, int64_t position)
{
    if (position < 0) {
        return 0;
    }

    (void) stb_vorbis_get_error(handle->decoder);
    const int offset = stb_vorbis_seek(handle->decoder, position);
    if (stb_vorbis_get_error(handle->decoder) != VORBIS__no_error) {
        return 0;
    }

    handle->frame_pos = stb_vorbis_tell_pcm(handle->decoder);
    handle->decode_buf_pos = 0;
    handle->decode_buf_len = 0;

    vorbis_error_t error;
    do {
        error = decode_frame(handle);
    } while (error == VORBIS_ERROR_DECODE_RECOVERED);
    if (error != VORBIS_NO_ERROR && error != VORBIS_ERROR_STREAM_END) {
        return 0;
    }

    handle->decode_buf_pos += offset;
    return 1;
}


int64_t vorbis_tell(const vorbis_t *handle)
{
    return handle->frame_pos + handle->decode_buf_pos;
}
