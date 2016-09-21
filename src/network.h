#pragma once

#include <psp2/sysmodule.h>

#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>


void netInit();
void netTerm();
void httpInit();
void httpTerm();
void download(const char *url, const char *dest);
