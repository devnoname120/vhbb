/*
 * 	Copyright (c) 2016 Arkanite
 *
 *	 	-A Native HomeBrew Browser for the Playstation Vita-
 *
 *	This program is free software:
 *		you can redistribute it and/or modify it under the terms of the
 *		GNU General Public License as published by the Free Software Foundation,
 *		either version 3 of the License, or (at your option) any later version.
 *
 *		This program comes WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		You should have received a copy of the GNU General Public License
 *		along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

 // TOOLS ------------------------------------------------

#include "vhbb.h"
#include "macros.h"
#include "tools.h"

/* |||| STRINGS ||| */
char* string_join(int count, ...)
{
	va_list ap; int si; int len = 1;
	va_start(ap, count);
	for (si = 0; si < count; si++) { len += strlen(va_arg(ap, char*)); }
	va_end(ap);
	char *merged = calloc(sizeof(char), len);
	int null_pos = 0;
	va_start(ap, count);
	for (si = 0; si < count; si++) {
		char *s = va_arg(ap, char*);
		strcpy(merged + null_pos, s);
		null_pos += strlen(s);
	}
	va_end(ap);
	return merged;
}
void string_remove_newline(char *str) { str[strcspn(str, "\r\n")] = 0; }


/* ||| DIRECTORIES ||| */
void dir_create(const char *dir)
{
	char dir_copy[0x400] = { 0 };
	snprintf(dir_copy, sizeof(dir_copy) - 2, "%s", dir);
	dir_copy[strlen(dir_copy)] = '/';
	char *c;
	for (c = dir_copy; *c; ++c) {
		if (*c == '/') { *c = '\0'; sceIoMkdir(dir_copy, 0777); *c = '/'; }
	}
}



/* ||| FILES ||| */
void file_delete(const char *path)
{
	int ret = sceIoRemove(path);
	if (ret < 0) { /* error could not find file. */ }
}
int file_is_png(char *file)
{
	unsigned char pngHead[8];
	unsigned char pngSignature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	FILE *dat = fopen(file, "rt");
	if (dat == NULL) { return 1; } // Just return 1 as the file will be downloaded next load
	fread(pngHead, sizeof(pngHead), 1, dat);
	if (memcmp(pngSignature, pngHead, 8) == 0) { fclose(dat); return 1; }
	else { fclose(dat); return 0; }
}
unsigned long file_size(char* file)
{
	FILE * f = fopen(file, "r");
	fseek(f, 0, SEEK_END);
	unsigned long len = (unsigned long)ftell(f);
	fclose(f);
	return len;
}


/* ||| LOGCAT ||| */
void logcat_add(char *str1, char *str2, char *str3)
{
	FILE *lf = fopen(VHBB_APP_FILE_LOGCAT, "a+");
	if (lf != NULL) {
		fprintf(lf, "%s%s%s", str1, str2, str3);
	}
	else {
		FILE *lf = fopen(VHBB_APP_FILE_LOGCAT, "ab+");
		fprintf(lf, "%s%s%s", str1, str2, str3);
	}
	fclose(lf);
}
void logcat_add_real(char *str1, int real1, char *str2)
{
	FILE *lf = fopen(VHBB_APP_FILE_LOGCAT, "a+");
	if (lf != NULL) {
		fprintf(lf, "%s%d%s", str1, real1, str2);
	}
	else {
		FILE *lf = fopen(VHBB_APP_FILE_LOGCAT, "ab+");
		fprintf(lf, "%s%d%s", str1, real1, str2);
	}
	fclose(lf);
}


/* ||| POSITION CHECKING ||| */
int point_in_rectangle(int px, int py, int x1, int y1, int x2, int y2)
{
	if (px > x1 && px < x2 && py > y1 && py < y2) { return 1; }
	else { return 0; }
}


/* ||| DATE/TIME ||| */
void getDateString(char *string, int date_format, SceDateTime *time)
{
	switch (date_format) {
	case SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD:
		sprintf(string, "%04d/%02d/%02d", time->year, time->month, time->day);
		break;

	case SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY:
		sprintf(string, "%02d/%02d/%04d", time->day, time->month, time->year);
		break;

	case SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY:
		sprintf(string, "%02d/%02d/%04d", time->month, time->day, time->year);
		break;
	}
}

void getTimeString(char *string, int time_format, SceDateTime *time)
{
	switch (time_format) {
	case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
		sprintf(string, "%02d:%02d %s", (time->hour > 12) ? (time->hour - 12) : ((time->hour == 0) ? 12 : time->hour), time->minute, time->hour >= 12 ? "PM" : "AM");
		break;

	case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
		sprintf(string, "%02d:%02d", time->hour, time->minute);
		break;
	}
}
//-------------------------------------------------------
