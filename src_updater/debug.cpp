#include "debug.h"

#ifdef DEBUGNET

int log_init() {
	debugNetInit(DEBUGNETIP, 18194, 3);

	return 0;
}

int _log_printf(int level, const char *format, ...) {
	va_list args;
	va_start(args, format);

	char buf[512];
	vsnprintf(buf, 512, format, args);

	char buf_colored[512];
	switch (level) {
		case DBG_INFO:
			snprintf(buf_colored, 512, "\033[1;34;7m[INFO]\033[0m\033[1;34m %s\033[0m", buf);
			break;
		case DBG_ERROR:
			snprintf(buf_colored, 512, "\033[1;31;7m[ERROR]\033[0m\033[1;31m %s\033[0m", buf);
			break;
		case DBG_WARNING:
			snprintf(buf_colored, 512, "\033[1;35;7m[WARNING]\033[0m\033[1;35m %s\033[0m", buf);
			break;
		case DBG_DEBUG:
			snprintf(buf_colored, 512, "\033[1;30;7m[DEBUG]\033[0m\033[1;30m %s\033[0m", buf);
			break;
		default:
			snprintf(buf_colored, 512, "\033[1;31;7m[UNK]\033[0m\033[1;31m %s\033[0m", buf);
			break;
	}

	va_end(args);

	debugNetUDPSend(buf_colored);

	return 0;
}

#endif