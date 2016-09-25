#include <stdio.h>
#include <string.h>

#include "buffer.h"

static int32_t GetBufferCurrentCount2(FileBuffer* fileBuffer);

int32_t CreateBuffer(FileBuffer* fileBuffer, uint8_t* buf, uint32_t size)
{
	int32_t ret = 0;
	memset(fileBuffer, 0, sizeof(FileBuffer));
	fileBuffer->read = 0;
	fileBuffer->write = 0;
	fileBuffer->count = 0;
	fileBuffer->buf = buf;
	fileBuffer->size = size;
	fileBuffer->initFlag = 0;
	fileBuffer->lock = sceKernelCreateMutex("ringbuflock", SCE_KERNEL_ATTR_TH_FIFO, 0, &fileBuffer->optParam);

	if (fileBuffer->lock < 0) { printf("ERROR: sceKernelCreateLwMutex 0x%08x\n", ret); return ret; }
	fileBuffer->initFlag |= RINGBUF_INIT_CREATE;
	return ret;
}



int32_t DeleteBuffer(FileBuffer* fileBuffer)
{
	int32_t ret = 0;
	if (!(fileBuffer->initFlag & RINGBUF_INIT_CREATE)) { ret = RINGBUF_ERROR_NOT_INITIALIZED; return ret; }
	ret = sceKernelDeleteMutex(&fileBuffer->lock);
	return ret;
}



int32_t ResetBuffer(FileBuffer* fileBuffer)
{
	int32_t ret = 0;
	fileBuffer->read = 0;
	fileBuffer->write = 0;
	fileBuffer->count = 0;
	return ret;
}



int32_t SetBufferData(FileBuffer* fileBuffer, uint8_t* data, uint32_t setSize)
{
	int32_t  ret = 0;
	uint8_t* dstBuf;
	sceKernelLockMutex(&fileBuffer->lock, 1, 0);
	if (setSize > GetBufferCapacity(fileBuffer)) {
		ret = RINGBUF_ERROR_INVALID_VALUE;
		sceKernelUnlockMutex(&fileBuffer->lock, 1);
		return ret;
	}
	if (fileBuffer->write + setSize > fileBuffer->size) {
		int32_t diff;
		diff = fileBuffer->size - fileBuffer->write;
		dstBuf = fileBuffer->buf + fileBuffer->write;
		memcpy((void*)dstBuf, (void*)data, diff);
		setSize -= diff;
		fileBuffer->count += diff;
		fileBuffer->write = (fileBuffer->write + diff) % fileBuffer->size;
		data = (uint8_t*)((uint32_t)data + diff);
	}
	dstBuf = (uint8_t*)((uint32_t)fileBuffer->buf + fileBuffer->write);
	memcpy((void*)dstBuf, (void*)data, setSize);
	fileBuffer->write = (fileBuffer->write + setSize) % fileBuffer->size;
	fileBuffer->count += setSize;
	sceKernelUnlockMutex(&fileBuffer->lock, 1);
	return ret;
}



int32_t GetBufferData(FileBuffer* fileBuffer, uint8_t* data, uint32_t getSize)
{
	int32_t ret = 0;
	uint32_t readNum = 0;
	uint32_t len;
	uint8_t* srcBuf;
	sceKernelLockMutex(&fileBuffer->lock, 1, 0);
	if (getSize > GetBufferCurrentCount(fileBuffer)) {
		ret = RINGBUF_ERROR_INVALID_VALUE;
		sceKernelUnlockMutex(&fileBuffer->lock, 1);
		return ret;
	}
	len = GetBufferCurrentCount2(fileBuffer);
	if (len < getSize) {
		readNum = len;
		srcBuf = (uint8_t*)((uint32_t)fileBuffer->buf + fileBuffer->read);
		memcpy((void*)data, srcBuf, readNum);
		fileBuffer->read = (fileBuffer->read + readNum) % fileBuffer->size;
		fileBuffer->count -= readNum;
		data += readNum;
		getSize -= readNum;
	}
	readNum = getSize;
	srcBuf = (uint8_t*)((uint32_t)fileBuffer->buf + fileBuffer->read);
	memcpy((void*)data, srcBuf, readNum);
	fileBuffer->read = (fileBuffer->read + readNum) % fileBuffer->size;
	fileBuffer->count -= readNum;
	sceKernelUnlockMutex(&fileBuffer->lock, 1);
	return ret;
}



// GET THE AMOUNT OF SPACE
int32_t GetBufferCapacity(FileBuffer* fileBuffer)
{
	int32_t ret = 0;

	ret = fileBuffer->size - fileBuffer->count;
	return ret;
}



// GET THE AMOUNT OF THE STORED DATA
int32_t GetBufferCurrentCount(FileBuffer* fileBuffer)
{
	int32_t ret = 0;
	ret = fileBuffer->count;
	return ret;
}



static int32_t GetBufferCurrentCount2(FileBuffer* fileBuffer)
{
	int32_t ret = 0;
	uint32_t diff;
	// GET THE AMOUNT OF THE STORED DATA (TILL THE END OF THE BUFFER)
	diff = fileBuffer->size - fileBuffer->read;
	if (diff < fileBuffer->count) { ret = diff; }
	else { ret = fileBuffer->count; }
	return ret;
}
