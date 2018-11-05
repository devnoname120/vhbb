#include <psp2/kernel/threadmgr.h>
#include <zconf.h>
#include <alloca.h>
#include <pthread.h>
#include <exception>
#include "vitasdk_quirks.h"
#include "debug.h"


// This is required so that exceptions work

extern "C" {
unsigned int sleep(unsigned int seconds) {
    sceKernelDelayThread(seconds * 1000 * 1000);
    return 0;
}

int usleep(useconds_t usec) {
    sceKernelDelayThread(usec);
    return 0;
}

void __sinit(struct _reent *);
}

__attribute__((constructor(101))) void pthread_setup(void) {
    pthread_init();
    __sinit(_REENT);
}

// FIXME: Doesn't seem to actually work
void terminate_logger() {
    std::exception_ptr p = std::current_exception();
    try {
        std::rethrow_exception(p);
    } catch (const std::exception &e) {
        dbg_printf(DBG_ERROR, "terminate() because of %s", e.what());
    }
}