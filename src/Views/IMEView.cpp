#include "IMEView.h"


IMEView::IMEView(IMEViewResult *result, const char title[] , const char showText[]) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title);
	utf8_to_utf16((uint8_t *)title, _title);
	utf8_to_utf16((uint8_t *)showText, _showText);
	_result = result;
	*_initialText = '\0';
	_maxTextLength = SCE_IME_DIALOG_MAX_TEXT_LENGTH;
}

IMEView::IMEView(IMEViewResult *result, const char title[] , const char showText[],
		SceUInt32 maxInputLength) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title);
	utf8_to_utf16((uint8_t *)title, _title);
	utf8_to_utf16((uint8_t *)showText, _showText);
	_result = result;
	*_initialText = '\0';
	_maxTextLength = maxInputLength;
}


IMEView::IMEView(IMEViewResult *result, const char title[] , const char showText[],
		const char initialText[], uint16_t initialTextSize) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title);
	utf8_to_utf16((uint8_t *)title, _title);
	utf8_to_utf16((uint8_t *)showText, _showText);
	_result = result;
	utf8_to_utf16((uint8_t *)initialText, _initialText);
	_maxTextLength = SCE_IME_DIALOG_MAX_TEXT_LENGTH;
}

IMEView::IMEView(IMEViewResult *result, const char title[] , const char showText[],
		const char initialText[], uint16_t initialTextSize, SceUInt32 maxInputLength) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title);
	utf8_to_utf16((uint8_t *)title, _title);
	utf8_to_utf16((uint8_t *)showText, _showText);
	_result = result;
	utf8_to_utf16((uint8_t *)initialText, _initialText);
	_maxTextLength = maxInputLength;
}

IMEView::~IMEView() {
	sceImeDialogTerm();
	delete _input_text_buffer_utf16;
	delete _input_text_buffer_utf8;
}


int IMEView::Display() {
	if (request_destroy) // done here!
		return 0;

	if (!shown_dialog) {
		log_printf(DBG_DEBUG, "Initializing IMEView");
		_input_text_buffer_utf16 = new uint16_t[_maxTextLength + 1];
		_input_text_buffer_utf8 = new uint8_t[_maxTextLength + 1];

		SceImeDialogParam param;
		sceImeDialogParamInit(&param);

		param.supportedLanguages = 0x0001FFFF;
		param.languagesForced = SCE_TRUE;
//		param.type = SCE_IME_TYPE_BASIC_LATIN;
		param.title = _title;
		param.enterLabel = SCE_IME_ENTER_LABEL_SEARCH;
//		param.dialogMode = SCE_IME_DIALOG_DIALOG_MODE_WITH_CANCEL;
		param.maxTextLength = _maxTextLength;
		param.inputMethod = 0;
		if (_initialText && *_initialText != '\0') {
			param.initialText = _initialText;
		}
		param.inputTextBuffer = _input_text_buffer_utf16;

		SceInt32 res = sceImeDialogInit(&param);
		shown_dialog = res == 0;
		if (!shown_dialog) {
			log_printf(DBG_ERROR,
			           "Couldn't open IMEView dialog: 0x%lx\nFind the status codes at "
			           "https://github.com/vitasdk/vita-headers/blob/master/include/psp2/common_dialog.h",
			           res);
			if (_result)
				_result->status = IMEVIEW_STATUS_CANCELED;
			request_destroy = true;
		}
		return 0;
	}

	auto new_status = (IMEViewStatus)sceImeDialogGetStatus();
	if (_status != new_status)
		switch (new_status) {
			case IMEVIEW_STATUS_NONE:
				log_printf(DBG_DEBUG, "IMEView status \"IMEVIEW_STATUS_NONE\"");
				break;
			case IMEVIEW_STATUS_RUNNING:
				log_printf(DBG_DEBUG, "IMEView status \"IMEVIEW_STATUS_RUNNING\"");
				break;
			case IMEVIEW_STATUS_FINISHED:
				log_printf(DBG_DEBUG, "IMEView status \"IMEVIEW_STATUS_FINISHED\"");
				break;
			case IMEVIEW_STATUS_CANCELED:
				log_printf(DBG_DEBUG, "IMEView status \"IMEVIEW_STATUS_CANCELED\"");
				break;
		}
	_status = new_status;

	if (_status == IMEVIEW_STATUS_FINISHED) {
		SceImeDialogResult result={};
		sceImeDialogGetResult(&result);

		if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE)
			_status = IMEVIEW_STATUS_CANCELED;
		else {
			utf16_to_utf8(_input_text_buffer_utf16, _input_text_buffer_utf8);
			if (_result)
				_result->userText = std::string((char *)_input_text_buffer_utf8);
		}

		sceImeDialogTerm();
		request_destroy = true;
	}

	if (_result)
		_result->status = _status;

	return 0;
}

void IMEView::utf8_to_utf16(uint8_t *src, uint16_t *dst) {
	int i;
	for (i = 0; src[i];) {
		if ((src[i] & 0xE0) == 0xE0) {
			*(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
			i += 3;
		} else if ((src[i] & 0xC0) == 0xC0) {
			*(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
			i += 2;
		} else {
			*(dst++) = src[i];
			i += 1;
		}
	}

	*dst = '\0';
}

void IMEView::utf16_to_utf8(uint16_t *src, uint8_t *dst) {
	int i;
	for (i = 0; src[i]; i++) {
		if ((src[i] & 0xFF80) == 0) {
			*(dst++) = src[i] & 0xFF;
		} else if((src[i] & 0xF800) == 0) {
			*(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		} else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
			*(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
			*(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
			*(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
			*(dst++) = (src[i + 1] & 0x3F) | 0x80;
			i += 1;
		} else {
			*(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
			*(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		}
	}

	*dst = '\0';
}
