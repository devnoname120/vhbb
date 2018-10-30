#include "MessageDialog.h"

static int loadScePaf() {
    static uint32_t argp[] = { 0x180000, 0xFFFFFFFF, 0xFFFFFFFF, 1, 0xFFFFFFFF, 0xFFFFFFFF };

    int result = -1;

    uint32_t buf[4];
    buf[0] = sizeof(buf);
    buf[1] = (uint32_t)&result;
    buf[2] = 0xFFFFFFFF;
    buf[3] = 0xFFFFFFFF;

    return sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(argp), argp, buf);
}

static int unloadScePaf() {
    uint32_t buf = 0;
    return sceSysmoduleUnloadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, 0, nullptr, &buf);
}


MessageDialog::MessageDialog() {
    int ret1 = loadScePaf();
    dbg_printf(DBG_DEBUG, "Ret1 value: 0x%08X", ret1);

    SceMsgDialogParam param;
    sceMsgDialogParamInit(&param);

    SceMsgDialogUserMessageParam userMsgParam;
    memset(&userMsgParam, 0, sizeof(userMsgParam));

    param.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
    param.userMsgParam = &userMsgParam;
    param.userMsgParam->msg = (int8_t *) "Network unavailable"; // FIXME Configurable
    param.userMsgParam->buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;

    // TODO Check return value
    int ret = sceMsgDialogInit(&param);
    dbg_printf(DBG_DEBUG, "Ret value: 0x%08X", ret);
}

MessageDialog::~MessageDialog() {
    sceMsgDialogTerm();
}

int MessageDialog::HandleInput(int focus, const Input& input) {
    return 0;
}

int MessageDialog::Display() {
    CheckStatus();
    int ret = vita2d_common_dialog_update();
    dbg_printf(DBG_DEBUG, "common update: 0x%08X", ret);

    return 0;
}

void MessageDialog::CheckStatus() {
    SceCommonDialogStatus status;

    // TODO Can only one dialog be run per thread or in total?
    status = sceMsgDialogGetStatus();

    dbg_printf(DBG_DEBUG, "Status: %u", status);

    switch (status) {
        default:
        case SCE_COMMON_DIALOG_STATUS_NONE:
        case SCE_COMMON_DIALOG_STATUS_RUNNING:
            return;
        case SCE_COMMON_DIALOG_STATUS_FINISHED:
//            sceKernelExitProcess(0);
            break;
    }

    SceMsgDialogResult msgResult;
    memset(&msgResult, 0, sizeof(msgResult));

    int res = sceMsgDialogGetResult(&msgResult);
    dbg_printf(DBG_DEBUG, "dialog res: 0x%08X", res);
    dbg_printf(DBG_DEBUG, "dialog result: 0x%08X", msgResult.result);
    dbg_printf(DBG_DEBUG, "button result: 0x%08X", msgResult.buttonId);
    dbg_printf(DBG_DEBUG, "dialog mode: 0x%08X", msgResult.mode);
}
