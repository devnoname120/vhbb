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
#include "memory.h"

#include <stdlib.h>

 /*************************************************************************/
 /************************** Interface routines ***************************/
 /*************************************************************************/

void *mem_alloc(vorbis_t *handle, int32_t size, int32_t align)
{
	ASSERT(size >= 0);
	ASSERT(align >= 0);
	ASSERT((align & (align - 1)) == 0);

	if (handle->callbacks.malloc) {
		return (*handle->callbacks.malloc)(handle->callback_data, size, align);
	}
	else {
		int32_t size_to_alloc = size + sizeof(void *);
		if (align > (int)sizeof(void *)) {
			size_to_alloc += align - sizeof(void *);
		}
		void *base = malloc(size_to_alloc);
		if (UNLIKELY(!base)) {
			return NULL;
		}
		ASSERT((uintptr_t)base % sizeof(void *) == 0);
		void *ptr = (void *)((uintptr_t)base + sizeof(void *));
		if (align != 0 && (uintptr_t)ptr % align != 0) {
			ptr = (void *)((uintptr_t)ptr + (align - ((uintptr_t)ptr % align)));
		}
		((void **)ptr)[-1] = base;
		return ptr;
	}
}

/*-----------------------------------------------------------------------*/

void mem_free(vorbis_t *handle, void *ptr)
{
	if (handle->callbacks.free) {
		(*handle->callbacks.free)(handle->callback_data, ptr);
	}
	else {
		if (ptr) {
			free(((void **)ptr)[-1]);
		}
	}
}

/*-----------------------------------------------------------------------*/

void *alloc_channel_array(vorbis_t *handle, int channels, int32_t size,
	int32_t align)
{
	ASSERT(size >= 0);
	ASSERT(align >= 0);
	ASSERT((align & (align - 1)) == 0);

	int32_t array_size = sizeof(void *) * channels;
	if (align != 0 && array_size % align != 0) {
		array_size += align - (array_size % align);
	}
	int32_t subarray_size = size;
	if (align != 0 && subarray_size % align != 0) {
		subarray_size += align - (subarray_size % align);
	}

	char **array =
		mem_alloc(handle, array_size + channels * subarray_size, align);
	if (array) {
		char *subarray_base = (char *)array + array_size;
		for (int channel = 0; channel < channels; channel++) {
			array[channel] = subarray_base + (channel * subarray_size);
		}
	}
	return array;
}

/*************************************************************************/
/*************************************************************************/
