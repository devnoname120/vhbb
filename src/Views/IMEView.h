#pragma once

#include <global_include.h>

#include <Views/View.h>
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


class IMEView : public View {
public:
	IMEView(IMEViewResult *result, const char title[] , const char showText[]);
	IMEView(IMEViewResult *result, const char title[] , const char showText[],
		SceUInt32 maxInputLength);
	IMEView(IMEViewResult *result, const char title[] , const char showText[],
		const char initialText[]);
	IMEView(IMEViewResult *result, const char title[] , const char showText[],
		const char initialText[], SceUInt32 maxInputLength);
	~IMEView() override;

	int Display() override;

private:
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
