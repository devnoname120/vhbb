#pragma once

#include "../global_include.h"
#include "../singleton.h"
#include "../font.h"
#include "commonDialog.h"
#include "activity.h"
#include "View.h"

#define DIALOG_WIDTH 760
#define DIALOG_HEIGHT 440
#define DIALOG_PADDING_X 79
#define DIALOG_PADDING_Y 30
#define DIALOG_BTN_WIDTH 322
#define DIALOG_BTN_HEIGHT 56
#define DIALOG_BTN_PADDING 5
#define DIALOG_MSG_HEIGHT 315
#define DIALOG_X ((SCREEN_WIDTH-DIALOG_WIDTH)/2)
#define DIALOG_Y ((SCREEN_HEIGHT-DIALOG_HEIGHT)/2)
#define DIALOG_BTN_Y (DIALOG_Y+365)
#define DIALOG_BTN_INNER_TOP_Y (DIALOG_BTN_Y+DIALOG_BTN_PADDING)
#define DIALOG_BTN_INNER_BOT_Y (DIALOG_BTN_Y+DIALOG_BTN_HEIGHT-DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_1_X (DIALOG_X+(DIALOG_X+DIALOG_WIDTH/2-DIALOG_BTN_WIDTH/2))
#define DIALOG_BTN_1_OF_1_INNER_LEFT_X (DIALOG_BTN_1_OF_1_X+DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_1_INNER_RIGHT_X (DIALOG_BTN_1_OF_1_X+DIALOG_BTN_WIDTH-DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_2_X (DIALOG_X+44)
#define DIALOG_BTN_1_OF_2_INNER_LEFT_X (DIALOG_BTN_1_OF_2_X+DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_2_INNER_RIGHT_X (DIALOG_BTN_1_OF_2_X+DIALOG_BTN_WIDTH-DIALOG_BTN_PADDING)
#define DIALOG_BTN_2_OF_2_X (DIALOG_X+394)
#define DIALOG_BTN_2_OF_2_INNER_LEFT_X (DIALOG_BTN_2_OF_2_X+DIALOG_BTN_PADDING)
#define DIALOG_BTN_2_OF_2_INNER_RIGHT_X (DIALOG_BTN_2_OF_2_X+DIALOG_BTN_WIDTH-DIALOG_BTN_PADDING)
#define DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES 45


enum DialogType {
	DIALOG_TYPE_OK,
	DIALOG_TYPE_YESNO
};

struct DialogViewResult {
	CommonDialogStatus status = COMMON_DIALOG_STATUS_NONE;
	bool accepted = false;
};

class DialogView : Singleton<DialogView>, public View {
public:
	DialogView();
	~DialogView();

	static void openDialogView(DialogViewResult *result, std::string message, DialogType type);

	int Display() override;
	int HandleInput(int focus, const Input& input) override;

private:
	std::shared_ptr<DialogView> me_ptr;

	Font msg_font;
	Font btn_font;

	Texture img_dialog_msg_bg;
	Texture img_dialog_msg_btn;
	Texture img_dialog_msg_btn_active;
	Texture img_dialog_msg_btn_focus;

	void prepare(DialogViewResult *result, std::string message, DialogType type);
	void DrawBtn(const std::string &text, const Point &sprPt, const Rectangle &textRect, int idx);
	int GetGlowCycleAlpha();
	void HandleBtnFocus(const Input& input);
	int GetTouchedBtnIdx(const Input& input);
	void HandleBtnTouch(const Input& input);

	CommonDialogStatus _status = COMMON_DIALOG_STATUS_NONE;
	bool _accepted = false;
	DialogViewResult *_result;
	std::string _message;
	DialogType _type;
	int _btnFocus, _btnTouched = -1;
	unsigned int _focusGlowCycle = 0;
};
