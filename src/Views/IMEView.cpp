#include <utility>
#include <psp2/ime_dialog.h>

#include "IMEView.h"


IMEView::IMEView() {
	log_printf(DBG_DEBUG, "IMEView::IMEView()");
	commonDialogSetConfig();
	// FIXME HACK: when IMEView is passed to Activity::AddView() it's destroyed once the activity is closed
	// Keeping an internal shared_ptr of itself makes sure that it's never destroyed
	me_ptr = std::shared_ptr<IMEView>(this);
}


void IMEView::openIMEView(std::shared_ptr<IMEViewResult> result, std::string title, SceUInt32 maxInputLength) {
	openIMEView(std::move(result), std::move(title), "", maxInputLength);
}

void IMEView::openIMEView(std::shared_ptr<IMEViewResult> result, std::string title, std::string initialText,
                          SceUInt32 maxInputLength) {
	IMEView *imeView = IMEView::create_instance();
	imeView->prepare(std::move(result), std::move(title), std::move(initialText), maxInputLength);
	Activity::get_instance()->AddView(imeView->me_ptr);
}

void IMEView::closeIMEView() {
	sceImeDialogTerm();
}

void IMEView::prepare(std::shared_ptr<IMEViewResult> result, std::string title, std::string initialText,
                      SceUInt32 maxInputLength) {
	log_printf(DBG_DEBUG, "Created IMEView \"%s\"", title.c_str());
	if (_status == COMMON_DIALOG_STATUS_RUNNING) {
		log_printf(DBG_WARNING, "Canceling current IMEView");
		sceImeDialogTerm();
		if (_result)
			_result->status = COMMON_DIALOG_STATUS_CANCELED;
	}
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	_title = converter.from_bytes(title);
	_result = result;
	if (!initialText.empty())
		log_printf(DBG_DEBUG, "initialText set \"%s\"", initialText.c_str());
	_initialText = converter.from_bytes(initialText);
	_maxTextLength = maxInputLength;
	shown_dialog = false;
	request_destroy = false;
}

IMEView::~IMEView() {
	log_printf(DBG_WARNING, "IMEView destructor called");
	sceImeDialogTerm();
	delete _input_text_buffer_utf16;
}


int IMEView::Display() {
	if (request_destroy) { // done here!
		sceImeDialogTerm();
		return 0;
	}

	if (!shown_dialog) {
		log_printf(DBG_DEBUG, "IMEView: Initializing dialog");
		delete _input_text_buffer_utf16;
		_input_text_buffer_utf16 = new SceWChar16[_maxTextLength + 1];

		SceImeDialogParam param;
		sceImeDialogParamInit(&param);

		param.supportedLanguages = 0x0001FFFF;
		param.languagesForced = SCE_TRUE;
//		param.type = SCE_IME_TYPE_BASIC_LATIN;
		param.title = (const SceWChar16 *)_title.c_str();
		param.enterLabel = SCE_IME_ENTER_LABEL_SEARCH;
//		param.dialogMode = SCE_IME_DIALOG_DIALOG_MODE_WITH_CANCEL;
		param.maxTextLength = _maxTextLength;
		param.inputMethod = 0;
		if (!_initialText.empty())
			param.initialText = (SceWChar16*)_initialText.c_str();
		param.inputTextBuffer = _input_text_buffer_utf16;

		SceInt32 res = sceImeDialogInit(&param);
		shown_dialog = res == 0;
		if (!shown_dialog) {
			log_printf(DBG_ERROR,
			           "Couldn't open IMEView dialog: 0x%lx\nFind the status codes at "
			           "https://github.com/vitasdk/vita-headers/blob/master/include/psp2/common_dialog.h",
			           res);
			if (_result)
				_result->status = COMMON_DIALOG_STATUS_CANCELED;
			request_destroy = true;
			sceImeDialogTerm();
		}
		return 0;
	}

	auto new_status = (CommonDialogStatus)sceImeDialogGetStatus();
	if (_status != new_status)
		switch (new_status) {
			case COMMON_DIALOG_STATUS_NONE:
				log_printf(DBG_DEBUG, "IMEView status \"COMMON_DIALOG_STATUS_NONE\"");
				break;
			case COMMON_DIALOG_STATUS_RUNNING:
				log_printf(DBG_DEBUG, "IMEView status \"COMMON_DIALOG_STATUS_RUNNING\"");
				break;
			case COMMON_DIALOG_STATUS_FINISHED:
				log_printf(DBG_DEBUG, "IMEView status \"COMMON_DIALOG_STATUS_FINISHED\"");
				break;
			case COMMON_DIALOG_STATUS_CANCELED:
				log_printf(DBG_DEBUG, "IMEView status \"COMMON_DIALOG_STATUS_CANCELED\"");
				break;
		}
	_status = new_status;

	if (_status == COMMON_DIALOG_STATUS_FINISHED) {
		SceImeDialogResult result={};
		sceImeDialogGetResult(&result);

		if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE)
			_status = COMMON_DIALOG_STATUS_CANCELED;
		else {
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
			_input_text_buffer_utf8 = converter.to_bytes((char16_t*)_input_text_buffer_utf16);
			if (_result)
				_result->userText = std::string(_input_text_buffer_utf8);
		}

		request_destroy = true;
		sceImeDialogTerm();
	}

	if (_result)
		_result->status = _status;

	return 0;
}
