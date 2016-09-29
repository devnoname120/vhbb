/*
 * 	Copyright (c) 2016 Arkanite
 *
 *	
 *	input.h
 *
 *
 */

#define SCE_IME_DIALOG_MAX_TITLE_LENGTH	(128)
#define SCE_IME_DIALOG_MAX_TEXT_LENGTH (512)
int inputMaxTextLength = 0;
int show_input = 0;
char userSearch[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
#define IME_DIALOG_RESULT_NONE 0
#define IME_DIALOG_RESULT_RUNNING 1
#define IME_DIALOG_RESULT_FINISHED 2
#define IME_DIALOG_RESULT_CANCELED 3
static uint16_t ime_title_utf16[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
static uint16_t ime_initial_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
static uint16_t ime_input_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
static uint8_t ime_input_text_utf8[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
void utf16_to_utf8(uint16_t *src, uint8_t *dst) {
	int p;
	for (p = 0; src[p]; p++) {
		if ((src[p] & 0xFF80) == 0) {
			*(dst++) = src[p] & 0xFF;
		}
		else if ((src[p] & 0xF800) == 0) {
			*(dst++) = ((src[p] >> 6) & 0xFF) | 0xC0;
			*(dst++) = (src[p] & 0x3F) | 0x80;
		}
		else if ((src[p] & 0xFC00) == 0xD800 && (src[p + 1] & 0xFC00) == 0xDC00) {
			*(dst++) = (((src[p] + 64) >> 8) & 0x3) | 0xF0;
			*(dst++) = (((src[p] >> 2) + 16) & 0x3F) | 0x80;
			*(dst++) = ((src[p] >> 4) & 0x30) | 0x80 | ((src[p + 1] << 2) & 0xF);
			*(dst++) = (src[p + 1] & 0x3F) | 0x80;
			p += 1;
		}
		else {
			*(dst++) = ((src[p] >> 12) & 0xF) | 0xE0;
			*(dst++) = ((src[p] >> 6) & 0x3F) | 0x80;
			*(dst++) = (src[p] & 0x3F) | 0x80;
		}
	}
	*dst = '\0';
}
	
void utf8_to_utf16( uint8_t *src, uint16_t *dst )
	{
	int p;
	for ( p = 0; src[p]; )
		{
		if ( (src[p] & 0xE0) == 0xE0 )
			{
			*(dst++) = ((src[p] & 0x0F) << 12) | ((src[p + 1] & 0x3F) << 6) | (src[p + 2] & 0x3F);
			p += 3;
			}
		else if ( (src[p] & 0xC0) == 0xC0 )
			{
			*(dst++) = ((src[p] & 0x1F) << 6) | (src[p + 1] & 0x3F);
			p += 2;
			}
		else
			{
			*(dst++) = src[p];
			p += 1;
			}
		}
	*dst = '\0';
	}
	
void initImeDialog( char *title, char *initial_text, int max_text_length )
	{
    // Convert UTF8 to UTF16
	utf8_to_utf16( (uint8_t *)title, ime_title_utf16 );
	utf8_to_utf16( (uint8_t *)initial_text, ime_initial_text_utf16 );
    SceImeDialogParam param;
	sceImeDialogParamInit( &param );
	param.sdkVersion 			= 0x03150021,
	param.supportedLanguages 	= 0x0001FFFF;
	param.languagesForced 		= SCE_TRUE;
	param.type 					= SCE_IME_TYPE_BASIC_LATIN;
	param.title 				= ime_title_utf16;
	param.maxTextLength 		= max_text_length;
	param.initialText 			= ime_initial_text_utf16;
	param.inputTextBuffer 		= ime_input_text_utf16;
	sceImeDialogInit( &param );
	return ;
	}
	
void oslOskGetText( char *text )
	{
	// Convert UTF16 to UTF8
	utf16_to_utf8( ime_input_text_utf16, ime_input_text_utf8 );
	strcpy( text, (char*)ime_input_text_utf8 );
	}
	
void input_get( char *title, char *init_text, int max_length )
	{
	inputMaxTextLength	= max_length;
	initImeDialog( title, init_text, inputMaxTextLength );
	show_input			= 1;
	}