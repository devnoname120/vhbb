#include "dialogView.h"

DialogView::DialogView() {
	log_printf(DBG_DEBUG, "DialogView::DialogView()");
	commonDialogSetConfig();
	me_ptr = std::shared_ptr<DialogView>(this);
}

DialogView::~DialogView() {
	log_printf(DBG_WARNING, "DialogView destructor called");
	sceMsgDialogTerm();
}

void DialogView::openDialogView(DialogViewResult *result, std::string message, DialogType type) {
	DialogView *dialogView = DialogView::create_instance();
	dialogView->prepare(result, std::move(message), type);
	Activity::get_instance()->AddView(dialogView->me_ptr);
}

void DialogView::prepare(DialogViewResult *result, std::string message, DialogType type) {
	log_printf(DBG_DEBUG, "Created DialogView \"%s\"", message.c_str());
	if (_status == COMMON_DIALOG_STATUS_RUNNING) {
		log_printf(DBG_WARNING, "Canceling current DialogView");
		sceMsgDialogTerm();
		if (_result)
			_result->status = COMMON_DIALOG_STATUS_CANCELED;
	}
	_result = result;
	shown_dialog = false;
	request_destroy = false;

	SceMsgDialogButtonType buttonType;
	switch(type)
	{
		default:
		case DIALOG_TYPE_OK:
			buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
			break;
		case DIALOG_TYPE_YESNO: buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_YESNO;
			break;
	};

	SceMsgDialogUserMessageParam userMsgParam;
	sceMsgDialogParamInit(&msgParam);
	msgParam.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
	memset(&userMsgParam, 0, sizeof(SceMsgDialogUserMessageParam));
	msgParam.userMsgParam = &userMsgParam;
	msgParam.userMsgParam->msg = reinterpret_cast<const SceChar8 *>(message.c_str());
	msgParam.userMsgParam->buttonType = buttonType;

	msgParam.commonParam.infobarParam = nullptr;
	msgParam.commonParam.dimmerColor = nullptr;
	msgParam.commonParam.bgColor = nullptr;
}

int DialogView::Display() {
	if (request_destroy) { // done here!
		sceMsgDialogTerm();
		return 0;
	}

	if (!shown_dialog) {
		log_printf(DBG_DEBUG, "DialogView: Initializing dialog");
		SceInt32 res = sceMsgDialogInit(&msgParam);
		shown_dialog = res == 0;
		if (!shown_dialog) {
			log_printf(DBG_ERROR,
			           "Couldn't open IMEView DialogView: 0x%lx\nFind the status codes at "
			           "https://github.com/vitasdk/vita-headers/blob/master/include/psp2/common_dialog.h",
			           res);
			if (_result)
				_result->status = COMMON_DIALOG_STATUS_CANCELED;
			request_destroy = true;
			sceMsgDialogTerm();
		}
		return 0;
	}

	auto new_status = (CommonDialogStatus)sceMsgDialogGetStatus();
	if (_status != new_status)
		switch (new_status) {
			case COMMON_DIALOG_STATUS_NONE:
				log_printf(DBG_DEBUG, "DialogView status \"COMMON_DIALOG_STATUS_NONE\"");
				break;
			case COMMON_DIALOG_STATUS_RUNNING:
				log_printf(DBG_DEBUG, "DialogView status \"COMMON_DIALOG_STATUS_RUNNING\"");
				break;
			case COMMON_DIALOG_STATUS_FINISHED:
				log_printf(DBG_DEBUG, "DialogView status \"COMMON_DIALOG_STATUS_FINISHED\"");
				break;
			case COMMON_DIALOG_STATUS_CANCELED:
				log_printf(DBG_DEBUG, "DialogView status \"COMMON_DIALOG_STATUS_CANCELED\"");
				break;
		}
	_status = new_status;

	if (_status == COMMON_DIALOG_STATUS_FINISHED) {
		SceMsgDialogResult result{};
		sceMsgDialogGetResult(&result);

		switch (result.buttonId) {
			case SCE_MSG_DIALOG_BUTTON_ID_YES:
				if(_result)
					_result->accepted = true;
				break;
			case SCE_MSG_DIALOG_BUTTON_ID_NO:
			default:
				if (_result)
					_result->accepted = false;
				break;
		}

		request_destroy = true;
		sceMsgDialogTerm();
	}

	if (_result)
		_result->status = _status;

	return 0;
}