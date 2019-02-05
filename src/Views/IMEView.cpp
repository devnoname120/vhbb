#include <utility>

#include "IMEView.h"


IMEView::IMEView() {
	auto sce_common_dialog_config_param = SceCommonDialogConfigParam{};
	sceCommonDialogSetConfigParam(&sce_common_dialog_config_param);
}

//void IMEView::openIMEView(IMEViewResult *result, const std::string *title , const std::string *showText, SceUInt32 maxInputLength) {
//	openIMEView(result, title, showText, nullptr, maxInputLength);
//}
//
//void IMEView::openIMEView(IMEViewResult *result, const std::string *title , const std::string *showText,
//                          const std::string *initialText, SceUInt32 maxInputLength) {
//	IMEView imeView = *IMEView::create_instance();
//	imeView.prepare(result, title, showText, initialText, maxInputLength);
//	auto imeViewPtr= std::make_shared<IMEView>(imeView);
//	Activity::get_instance()->AddView(imeViewPtr);
//}
//
//void IMEView::prepare(IMEViewResult *result, const std::string *title, const std::string *showText,
//                      const std::string *initialText, SceUInt32 maxInputLength) {
//	const char *title_c;
//	const char *text_c;
//	if (title)
//		title_c = title->c_str();
//	else
//		title_c = "Title";
//	if (showText)
//		text_c = showText->c_str();
//	else
//		text_c = "";
//	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title_c);
//	utf8_to_utf16((uint8_t *)title_c, _title);
//	utf8_to_utf16((uint8_t *)text_c, _showText);
//	_result = result;
//	if (initialText) {
//		log_printf(DBG_DEBUG, "initialText set \"%s\"", initialText->c_str());
//		utf8_to_utf16((uint8_t *) initialText->c_str(), _initialText);
//	} else {
//		*_initialText = '\0';
//	}
//	_maxTextLength = maxInputLength;
//	shown_dialog = false;
//	request_destroy = false;
//}

void IMEView::openIMEView(IMEViewResult *result, std::string title , std::string showText, SceUInt32 maxInputLength) {
	openIMEView(result, std::move(title), std::move(showText), "", maxInputLength);
}

void IMEView::openIMEView(IMEViewResult *result, std::string title , std::string showText,
                          std::string initialText, SceUInt32 maxInputLength) {
	IMEView imeView = *IMEView::create_instance();
	imeView.prepare(result, std::move(title), std::move(showText), std::move(initialText), maxInputLength);
	auto imeViewPtr= std::make_shared<IMEView>(imeView);
	Activity::get_instance()->AddView(imeViewPtr);
}

void IMEView::prepare(IMEViewResult *result, std::string title, std::string showText,
                      std::string initialText, SceUInt32 maxInputLength) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title.c_str());
	utf8_to_utf16((uint8_t *)title.c_str(), _title);
	utf8_to_utf16((uint8_t *)showText.c_str(), _showText);
	_result = result;
	if (!initialText.empty()) {
		log_printf(DBG_DEBUG, "initialText set \"%s\"", initialText.c_str());
		utf8_to_utf16((uint8_t *) initialText.c_str(), _initialText);
	} else {
		*_initialText = '\0';
	}
	_maxTextLength = maxInputLength;
	shown_dialog = false;
	request_destroy = false;
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
		delete _input_text_buffer_utf16;
		_input_text_buffer_utf16 = new uint16_t[_maxTextLength + 1];
		delete _input_text_buffer_utf8;
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
		if (*_initialText != '\0') {
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
			sceImeDialogTerm();
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

// copied from https://github.com/devingDev/VitaCord/blob/master/src/VitaIME.cpp
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

// copied from https://github.com/devingDev/VitaCord/blob/master/src/VitaIME.cpp
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
