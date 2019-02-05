#pragma once

#include <global_include.h>

#include <singleton.h>
#include <Views/View.h>
#include <activity.h>
#include <sys/socket.h>


enum IMEViewStatus {
	IMEVIEW_STATUS_NONE     = SCE_COMMON_DIALOG_STATUS_NONE,
	IMEVIEW_STATUS_RUNNING  = SCE_COMMON_DIALOG_STATUS_RUNNING,
	IMEVIEW_STATUS_FINISHED = SCE_COMMON_DIALOG_STATUS_FINISHED,
	IMEVIEW_STATUS_CANCELED
};


struct IMEViewResult {
	IMEViewStatus status = IMEVIEW_STATUS_NONE;
	std::string userText;
};


class IMEView : Singleton<IMEView>,public View {
public:
	IMEView();
//	static void openIMEView(IMEViewResult *result, const std::string *title , const std::string *showText,
//	                        SceUInt32 maxInputLength);
//	static void openIMEView(IMEViewResult *result, const std::string *title , const std::string *showText,
//	                        const std::string *initialText=nullptr, SceUInt32 maxInputLength=SCE_IME_DIALOG_MAX_TEXT_LENGTH);
	static void openIMEView(IMEViewResult *result, std::string title , std::string showText,
	                        SceUInt32 maxInputLength);
	static void openIMEView(IMEViewResult *result, std::string title , std::string showText,
	                        std::string initialText="", SceUInt32 maxInputLength=SCE_IME_DIALOG_MAX_TEXT_LENGTH);
	~IMEView() override;

	int Display() override;

private:
//	void prepare(IMEViewResult *result, const std::string *title , const std::string *showText, const std::string *initialText, SceUInt32 maxInputLength);
	void prepare(IMEViewResult *result, std::string title , std::string showText, std::string initialText, SceUInt32 maxInputLength);

	uint16_t _title[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
	uint16_t _showText[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
	SceUInt32 _maxTextLength;
	uint16_t _initialText[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
	uint16_t *_input_text_buffer_utf16 = nullptr;
	uint8_t *_input_text_buffer_utf8 = nullptr;
	IMEViewStatus _status = IMEVIEW_STATUS_NONE;
	IMEViewResult *_result;
	bool shown_dialog = false;

	void utf8_to_utf16(uint8_t *src, uint16_t *dst);
	void utf16_to_utf8(uint16_t *src, uint8_t *dst);
};
