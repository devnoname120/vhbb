/*
 * libnogg: a decoder library for Ogg Vorbis streams
 * Copyright (c) 2014-2016 Andrew Church <achurch@achurch.org>
 *
 * This software may be copied and redistributed under certain conditions;
 * see the file "COPYING" in the source code distribution for details.
 * NO WARRANTY is provided with this software.
 */

/*
 * This file implements vorbis_open_with_callbacks(), which is treated as
 * the "base" open function for the library; all other open functions are
 * implemented by calling this function with an appropriate set of
 * callbacks.
 */

#include "../ogg.h"
#include "../common.h"
#include "../util/memory.h"

#include <stdlib.h>
#include <string.h>


/* Callback wrappers for stb_vorbis. */
static int32_t stb_read(void *opaque, void *buf, int32_t len) {
    vorbis_t *handle = (vorbis_t *)opaque;
    return (*handle->callbacks.read)(handle->callback_data, buf, len);
}
static void stb_seek(void *opaque, int64_t offset) {
    vorbis_t *handle = (vorbis_t *)opaque;
    (*handle->callbacks.seek)(handle->callback_data, offset);
}
static int64_t stb_tell(void *opaque) {
    vorbis_t *handle = (vorbis_t *)opaque;
    return (*handle->callbacks.tell)(handle->callback_data);
}


vorbis_t *vorbis_open_callbacks(
    vorbis_callbacks_t callbacks, void *opaque, unsigned int options,
    vorbis_error_t *error_ret)
{
    vorbis_t *handle = NULL;
    vorbis_error_t error = VORBIS_NO_ERROR;

    /* Validate the function arguments. */
    if (!callbacks.read) {
        error = VORBIS_ERROR_INVALID_ARGUMENT;
        goto exit;
    }
    if (callbacks.length && (!callbacks.tell || !callbacks.seek)) {
        error = VORBIS_ERROR_INVALID_ARGUMENT;
        goto exit;
    }
    if ((callbacks.malloc != NULL) != (callbacks.free != NULL)) {
        error = VORBIS_ERROR_INVALID_ARGUMENT;
        goto exit;
    }

    /* Allocate and initialize a handle structure. */
    if (callbacks.malloc) {
        handle = (*callbacks.malloc)(opaque, sizeof(*handle), 0);
    } else {
        handle = malloc(sizeof(*handle));
    }
    if (!handle) {
        error = VORBIS_ERROR_INSUFFICIENT_RESOURCES;
        goto exit;
    }
    handle->read_int16_only = ((options & VORBIS_OPTION_READ_INT16_ONLY) != 0);
    handle->callbacks = callbacks;
    handle->callback_data = opaque;
    if (handle->callbacks.length) {
        handle->data_length =
            (*handle->callbacks.length)(handle->callback_data);
    } else {
        handle->data_length = -1;
    }
    handle->read_error_flag = 0;
    handle->eos_flag = 0;
    handle->frame_pos = 0;
    handle->decode_buf_len = 0;
    handle->decode_buf_pos = 0;

    /* Create an stb_vorbis handle for the stream. */
    int stb_error;
    handle->decoder = stb_vorbis_open_callbacks(
        stb_read, stb_seek, stb_tell, handle, handle->data_length, options,
        &stb_error);
    if (!handle->decoder) {
        if (stb_error == VORBIS_outofmem) {
            error = VORBIS_ERROR_INSUFFICIENT_RESOURCES;
        } else if (stb_error == VORBIS_unexpected_eof
                || stb_error == VORBIS_reached_eof
                || stb_error == VORBIS_missing_capture_pattern
                || stb_error == VORBIS_invalid_stream_structure_version
                || stb_error == VORBIS_invalid_first_page
                || stb_error == VORBIS_invalid_stream) {
            error = VORBIS_ERROR_STREAM_INVALID;
        } else {
            error = VORBIS_ERROR_DECODE_SETUP_FAILED;
        }
        goto error_free_handle;
    }

    /* Save the audio parameters. */
    stb_vorbis_info info = stb_vorbis_get_info(handle->decoder);
    handle->channels = info.channels;
    handle->rate = info.sample_rate;

    /* Allocate a decoding buffer based on the maximum decoded frame size.
     * We align this to a 64-byte boundary to help optimizations which
     * require aligned data. */
    const int sample_size = (handle->read_int16_only ? 2 : 4);
    const int32_t decode_buf_size =
        sample_size * handle->channels * info.max_frame_size;
    handle->decode_buf = mem_alloc(handle, decode_buf_size, 64);
    if (!handle->decode_buf) {
        error = VORBIS_ERROR_INSUFFICIENT_RESOURCES;
        goto error_close_decoder;
    }

  exit:
    if (error_ret) {
        *error_ret = error;
    }
    return handle;

  error_close_decoder:
    stb_vorbis_close(handle->decoder);
  error_free_handle:
    if (callbacks.free) {
        (*callbacks.free)(opaque, handle);
    } else {
        free(handle);
    }
    handle = NULL;
    goto exit;
}
