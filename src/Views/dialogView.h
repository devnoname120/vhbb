#pragma once

#include "../singleton.h"
#include "../font.h"
#include "commonDialog.h"
#include "activity.h"
#include "View.h"


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

	static void openDialogView(std::shared_ptr<DialogViewResult> result, std::string message, DialogType type);

	int Display() override;
	int HandleInput(int focus, const Input& input) override;

private:
	std::shared_ptr<DialogView> me_ptr;

	Font msg_font;
	Font btn_font;

	CachedTexture img_dialog_msg_bg;
	CachedTexture img_dialog_msg_btn;
	CachedTexture img_dialog_msg_btn_active;
	CachedTexture img_dialog_msg_btn_focus;

	void prepare(std::shared_ptr<DialogViewResult> result, std::string message, DialogType type);
	void DrawBtn(const std::string &text, const Point &sprPt, const Rectangle &textRect, int idx);
	int GetGlowCycleAlpha();
	void HandleBtnFocus(const Input& input);
	int GetTouchedBtnIdx(const Input& input);
	void HandleBtnTouch(const Input& input);

	CommonDialogStatus _status = COMMON_DIALOG_STATUS_NONE;
	bool _accepted = false;
	std::shared_ptr<DialogViewResult> _result;
	std::string _message;
	DialogType _type;
	int _btnFocus, _btnTouched = -1;
	unsigned int _focusGlowCycle = 0;
};
