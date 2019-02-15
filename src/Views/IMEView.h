#include <utility>

#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sys/socket.h>
#include <future>

#include <global_include.h>
#include <singleton.h>
#include <Views/View.h>
#include <activity.h>


enum IMEViewStatus {
	IMEVIEW_STATUS_NONE     = SCE_COMMON_DIALOG_STATUS_NONE,
	IMEVIEW_STATUS_RUNNING  = SCE_COMMON_DIALOG_STATUS_RUNNING,
	IMEVIEW_STATUS_FINISHED = SCE_COMMON_DIALOG_STATUS_FINISHED,
	IMEVIEW_STATUS_CANCELED
};

struct IMEViewResult {
	IMEViewResult(IMEViewStatus status = IMEVIEW_STATUS_NONE, std::string userText = "") : status(status), userText(std::move(userText)) {}
	
	IMEViewStatus status;
	std::string userText;
};

struct IMEViewAlreadyRunningException : public std::exception {
	const char * what () const noexcept override {
		return "IMEView is already running, can't launch another instance";
	}
};

class IMEView : Singleton<IMEView>, public View {
public:
	IMEView();
	static std::future<IMEViewResult> openIMEView(std::string title, SceUInt32 maxInputLength);
	static std::future<IMEViewResult> openIMEView(std::string title, std::string initialText, SceUInt32 maxInputLength = SCE_IME_DIALOG_MAX_TEXT_LENGTH);
	static void closeIMEView();
	~IMEView() override;

	int Display() override;

private:
	std::shared_ptr<IMEView> me_ptr;

	std::future<IMEViewResult> prepare(std::string title, std::string initialText, SceUInt32 maxInputLength);

	std::basic_string<char16_t> _title;
	std::basic_string<char16_t> _initialText;
	SceUInt32 _maxTextLength;
	SceWChar16 *_input_text_buffer_utf16 = nullptr;
	std::string _input_text_buffer_utf8;
	IMEViewStatus _status = IMEVIEW_STATUS_NONE;
	std::promise<IMEViewResult> _result;
	bool shown_dialog = false;
};
