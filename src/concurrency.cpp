#include "concurrency.h"

SceMutex::SceMutex(const std::string& name) :
	name(name)
{
	id = sceKernelCreateMutex(name.c_str(), 0, 0, nullptr);
}
SceMutex::~SceMutex() {
	int ret = sceKernelDeleteMutex(id);
	if (ret < 0) {
		log_printf(DBG_ERROR, "%s: sceKernelDeleteMutex(%i) = %i (0x%04x)\n"
                              "You might find the error code here: https://psp2sdk.github.io/error_8h.html",
                   name.c_str(), id, ret, ret);
	}
}

void SceMutex::lock() {
	int ret = sceKernelLockMutex(id, 1, nullptr);
	if (ret < 0) {
		log_printf(DBG_ERROR, "%s: sceKernelLockMutex(%i, 1, nullptr) = %i (0x%04x)\n"
		                      "You might find the error code here: https://psp2sdk.github.io/error_8h.html",
		           name.c_str(), id, ret, ret);
	}
}

void SceMutex::unlock() {
	int ret = sceKernelUnlockMutex(id, 1);
	if (ret < 0) {
		log_printf(DBG_ERROR, "%s: sceKernelUnlockMutex(%i, 1) = %i (0x%04x)\n"
		                      "You might find the error code here: https://psp2sdk.github.io/error_8h.html",
		           name.c_str(), id, ret, ret);
	}
}
