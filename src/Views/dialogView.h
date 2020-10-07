#pragma once

#include <atomic>

#include "../singleton.h"
#include "../scrollManager.h"
#include "shapes.h"
#include "../font.h"
#include "commonDialog.h"
#include "activity.h"
#include "View.h"


enum DialogType {
	DIALOG_TYPE_OK,
	DIALOG_TYPE_YESNO
};

struct DialogViewResult {
	std::atomic<CommonDialogStatus> status = COMMON_DIALOG_STATUS_NONE;
	std::atomic_bool accepted = false;
};

class DialogView : Singleton<DialogView>, public View {
public:
	DialogView();
	~DialogView();

	static void openDialogView(std::shared_ptr<DialogViewResult> result, const std::string& message, DialogType type);
	static void finalErrorDialog(const std::string& message);

	int Display() override;
	int HandleInput(int focus, const Input& input) override;

private:
	std::shared_ptr<DialogView> me_ptr;

	ScrollManager<true, true> scrollManager;
	int scrollX = 0;
	int scrollY = 0;

	Font msg_font;
	Font btn_font;

	Texture img_dialog_msg_bg;
	Texture img_dialog_msg_btn;
	Texture img_dialog_msg_btn_active;
	Texture img_dialog_msg_btn_focus;

	void prepare(std::shared_ptr<DialogViewResult> result, const std::string& message, DialogType type);
	void DrawBtn(const std::string &text, const Point &sprPt, const Rectangle &textRect, int idx);
	int GetGlowCycleAlpha();
	void HandleBtnFocus(const Input& input);
	int GetTouchedBtnIdx(const Input& input);
	void HandleBtnTouch(const Input& input);

	CommonDialogStatus _status = COMMON_DIALOG_STATUS_NONE;
	bool _accepted = false;
	std::shared_ptr<DialogViewResult> _result;
	std::string _message;
	Dimensions _messageDim;
	DialogType _type;
	int _btnFocus, _btnTouched = -1;
	unsigned int _focusGlowCycle = 0;
};
