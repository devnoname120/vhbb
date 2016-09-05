/*
 * libnogg: a decoder library for Ogg Vorbis streams
 * Copyright (c) 2014-2016 Andrew Church <achurch@achurch.org>
 *
 * This software may be copied and redistributed under certain conditions;
 * see the file "COPYING" in the source code distribution for details.
 * NO WARRANTY is provided with this software.
 */

#ifndef NOGG_SRC_DECODE_SETUP_H
#define NOGG_SRC_DECODE_SETUP_H

/*************************************************************************/
/*************************************************************************/

/**
 * start_decoder:  Read the header for a stream and perform all setup
 * necessary for decoding.
 *
 * [Parameters]
 *     handle: Stream handle.
 * [Return value]
 *     True on success, false on error.
 */
#define start_decoder INTERNAL(start_decoder)
extern bool start_decoder(stb_vorbis *handle);

/*************************************************************************/
/*************************************************************************/

#endif  // NOGG_SRC_DECODE_SETUP_H
