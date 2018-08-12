#include "CancelHandler.h"
#include "debug.h"

void CancelHandler::Cancel() {
    m_shouldCancel = true;
}

bool CancelHandler::GetCancelStatus() {
    dbg_printf(DBG_DEBUG, "in get cancel status");
    return m_shouldCancel;
}
