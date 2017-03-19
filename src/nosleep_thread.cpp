#include <global_include.h>
#include "nosleep_thread.h"

int nosleep_thread(SceSize args, void *argp) {
	while (true) {
		sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
		sceKernelDelayThread(10 * 1000 * 1000);
	}

	sceKernelExitDeleteThread(0);
}