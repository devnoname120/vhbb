# pragma once

#include "../global_include.h"

enum CommonDialogStatus {
	COMMON_DIALOG_STATUS_NONE     = SCE_COMMON_DIALOG_STATUS_NONE,
	COMMON_DIALOG_STATUS_RUNNING  = SCE_COMMON_DIALOG_STATUS_RUNNING,
	COMMON_DIALOG_STATUS_FINISHED = SCE_COMMON_DIALOG_STATUS_FINISHED,
	COMMON_DIALOG_STATUS_CANCELED
};

inline void commonDialogSetConfig(){
	auto sce_common_dialog_config_param = SceCommonDialogConfigParam{};
	sceCommonDialogSetConfigParam(&sce_common_dialog_config_param);
};