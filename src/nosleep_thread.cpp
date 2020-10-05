#include "nosleep_thread.h"

#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>

void StartNoSleepThread() {
  SceUID thid_sleep = sceKernelCreateThread(
  		"nosleep_thread", (SceKernelThreadEntry)nosleep_thread, 0x40, 0x1000, 0,
  		0, nullptr);
  sceKernelStartThread(thid_sleep, 0, nullptr);
}

int nosleep_thread(SceSize args, void *argp) {
  while (true) {
    sceKernelPowerTick(SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND);
    sceKernelDelayThread(10 * 1000 * 1000);
  }

  sceKernelExitDeleteThread(0);
  return 0;
}
