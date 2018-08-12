#pragma once

#include <functional>

typedef std::function<bool ()> CancelGetter;

class CancelHandler {
public:
    void Cancel();
    bool GetCancelStatus();
private:
    bool m_shouldCancel = false;
};
