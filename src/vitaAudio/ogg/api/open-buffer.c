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

#include <stdlib.h>
#include <string.h>

/*************************************************************************/
/******************** Buffer data type and callbacks *********************/
/*************************************************************************/

/* The argument to each of these callbacks is the stream handle itself. */

static int64_t buffer_length(void *opaque)
{
    vorbis_t *vorbis = (vorbis_t *)opaque;
    return vorbis->data_length;
}

static int64_t buffer_tell(void *opaque)
{
    vorbis_t *vorbis = (vorbis_t *)opaque;
    return vorbis->buffer_read_pos;
}

static void buffer_seek(void *opaque, int64_t offset)
{
    vorbis_t *vorbis = (vorbis_t *)opaque;
    vorbis->buffer_read_pos = offset;
}

static int32_t buffer_read(void *opaque, void *buffer, int32_t length)
{
    vorbis_t *vorbis = (vorbis_t *)opaque;
    if (length > vorbis->data_length - vorbis->buffer_read_pos) {
        length = vorbis->data_length - vorbis->buffer_read_pos;
    }
    memcpy(buffer, vorbis->buffer_data + vorbis->buffer_read_pos, length);
    vorbis->buffer_read_pos += length;
    return length;
}

static const vorbis_callbacks_t buffer_callbacks = {
    .length = buffer_length,
    .tell   = buffer_tell,
    .seek   = buffer_seek,
    .read   = buffer_read,
};

/*************************************************************************/
/*************************** Interface routine ***************************/
/*************************************************************************/

vorbis_t *vorbis_open_buffer(
    const void *buffer, int64_t length, unsigned int options,
    vorbis_error_t *error_ret)
{
    if (!buffer || length < 0) {
        if (error_ret) {
            *error_ret = VORBIS_ERROR_INVALID_ARGUMENT;
        }
        return NULL;
    }

    /* We pass the stream handle as the opaque callback parameter, but
     * that leads to a chicken-and-egg problem: open_callbacks() needs
     * the callback parameter to read from the stream, but we don't know
     * ahead of time where the stream handle will be allocated.  We get
     * around this by setting up a dummy handle on the stack with just the
     * fields needed by the callbacks, then modify the actual handle after
     * the open succeeds. */
    vorbis_t dummy;
    dummy.buffer_data = buffer;
    dummy.buffer_read_pos = 0;
    dummy.data_length = length;

    vorbis_t *handle =
        vorbis_open_callbacks(buffer_callbacks, &dummy, options, error_ret);
    if (handle) {
        handle->callback_data = handle;
        handle->buffer_data = dummy.buffer_data;
        handle->buffer_read_pos = dummy.buffer_read_pos;
    }

    return handle;
}

/*************************************************************************/
/*************************************************************************/
