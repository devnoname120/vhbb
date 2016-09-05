#ifndef FileBuffer_H
#define FileBuffer_H

#include <stdint.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>

#include "../def/definitions.h"

typedef struct FileBuffer
	{
	uint32_t				initFlag;
	SceUID					lock;
	SceKernelMutexOptParam 	optParam;
	uint8_t*				buf;
	uint32_t				write;
	uint32_t				read;
	uint32_t				count;
	uint32_t				size;
	} FileBuffer;

int32_t CreateBuffer	 		( FileBuffer* fileBuffer, uint8_t* buf, uint32_t size );
int32_t DeleteBuffer	 		( FileBuffer* fileBuffer );
int32_t ResetBuffer 	 		( FileBuffer* fileBuffer );
int32_t SetBufferData	 		( FileBuffer* fileBuffer, uint8_t* data, uint32_t setSize );
int32_t GetBufferData	 		( FileBuffer* fileBuffer, uint8_t* data, uint32_t getSize );
int32_t GetBufferCapacity		( FileBuffer* fileBuffer );
int32_t GetBufferCurrentCount	( FileBuffer* fileBuffer );

#endif
