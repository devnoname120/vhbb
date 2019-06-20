#include "Overclock.h"

#include <psp2/power.h>

Overclock::Overclock() :
    armClock(scePowerGetArmClockFrequency()),
    busClock(scePowerGetBusClockFrequency()),
    gpuClock(scePowerGetGpuClockFrequency()),
    gpuXbarClock(scePowerGetGpuXbarClockFrequency())
{
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
}

Overclock::~Overclock()
{
    scePowerSetArmClockFrequency(armClock);
    scePowerSetBusClockFrequency(busClock);
    scePowerSetGpuClockFrequency(gpuClock);
    scePowerSetGpuXbarClockFrequency(gpuXbarClock);
}
