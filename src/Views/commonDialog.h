# pragma once

#include <debug.h>
#include <psp2/common_dialog.h>

enum CommonDialogStatus {
	COMMON_DIALOG_STATUS_NONE     = SCE_COMMON_DIALOG_STATUS_NONE,
	COMMON_DIALOG_STATUS_RUNNING  = SCE_COMMON_DIALOG_STATUS_RUNNING,
	COMMON_DIALOG_STATUS_FINISHED = SCE_COMMON_DIALOG_STATUS_FINISHED,
	COMMON_DIALOG_STATUS_CANCELED
};

inline void commonDialogSetConfig() {
	SceCommonDialogConfigParam sce_common_dialog_config_param;
	sceCommonDialogConfigParamInit(&sce_common_dialog_config_param);
	sce_common_dialog_config_param.language = SCE_SYSTEM_PARAM_LANG_ENGLISH_US;
	sce_common_dialog_config_param.enterButtonAssign = SCE_SYSTEM_PARAM_ENTER_BUTTON_CROSS;
	int res = sceCommonDialogSetConfigParam(&sce_common_dialog_config_param);
	if (res)
		log_printf(DBG_ERROR, "sceCommonDialogSetConfigParam failed: %0.8x", res);
};
