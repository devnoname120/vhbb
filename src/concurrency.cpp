#include "concurrency.h"

SceMutex::SceMutex(std::string name) {
	id = sceKernelCreateMutex(name.c_str(), 0, 0, nullptr);
}
SceMutex::~SceMutex() {
	int ret = sceKernelDeleteMutex(id);
	if (ret < 0) {
		log_printf(DBG_ERROR, "sceKernelDeleteMutex(%i) = %i (0x%04x)", id, ret, ret);
	}
}

void SceMutex::lock() {
	int ret = sceKernelLockMutex(id, 1, nullptr);
	if (ret < 0) {
		log_printf(DBG_ERROR, "sceKernelLockMutex(%i, 1, nullptr) = %i (0x%04x)", id, ret, ret);
	}
}

void SceMutex::unlock() {
	int ret = sceKernelUnlockMutex(id, 1);
	if (ret < 0) {
		log_printf(DBG_ERROR, "sceKernelUnlockMutex(%i, 1) = %i (0x%04x)", id, ret, ret);
	}
}
