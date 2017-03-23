#pragma once

#define VITASDK
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include <psp2/libssl.h>
#include <psp2/ctrl.h>
#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#include <psp2/touch.h>


#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <algorithm>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <math.h>
#include <sys/stat.h>

#include <psp2/appmgr.h>
#include <psp2/apputil.h>
#include <psp2/types.h>
#include <psp2/message_dialog.h>
#include <psp2/ime_dialog.h>
#include <psp2/display.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/pgf.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/system_param.h>

#include "utils.h"

#ifdef PSP2SHELL
#include <psp2shell.h>
#endif

#ifdef DEBUGNET
#include <debugnet.h>
#endif

#include <vita2d.h>

#include <yaml-cpp/yaml.h>

#include "debug.h"
#include "macros.h"
#include "screen.h"

#include "unordered_map_pair_hash.h"