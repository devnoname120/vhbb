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

#include <string.h>


int32_t vorbis_read_float(
    vorbis_t *handle, float *buf, int32_t len, vorbis_error_t *error_ret)
{
    int32_t count = 0;
    int error = VORBIS_NO_ERROR;

    if (!buf || len < 0 || handle->read_int16_only) {
        error = VORBIS_ERROR_INVALID_ARGUMENT;
        goto out;
    }

    const int channels = handle->channels;
    while (count < len) {
        if (handle->decode_buf_pos >= handle->decode_buf_len) {
            error = decode_frame(handle);
            if (error) {
                break;
            }
        }
        const int copy = min(
            len - count, handle->decode_buf_len - handle->decode_buf_pos);
        memcpy(buf, ((float *)handle->decode_buf
                     + handle->decode_buf_pos * channels),
               copy * channels * sizeof(*buf));
        buf += copy * channels;
        count += copy;
        handle->decode_buf_pos += copy;
    }

  out:
    if (error_ret) {
        *error_ret = error;
    }
    return count;
}
