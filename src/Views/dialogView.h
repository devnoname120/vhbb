#pragma once

#include "../global_include.h"
#include "../singleton.h"
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

	static void openDialogView(DialogViewResult *result, std::string message, DialogType type);

	int Display() override;

	unsigned int priority = 150;

private:
	std::shared_ptr<DialogView> me_ptr;

	void prepare(DialogViewResult *result, std::string message, DialogType type);
	CommonDialogStatus _status = COMMON_DIALOG_STATUS_NONE;
	DialogViewResult *_result;
	bool shown_dialog = false;
	SceMsgDialogParam msgParam;
};
