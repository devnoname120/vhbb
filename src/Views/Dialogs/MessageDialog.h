#pragma once

#include <Views/View.h>

class MessageDialog: public View {
public:
    MessageDialog();
    ~MessageDialog();

    int HandleInput(int focus, const Input& input);
    int Display();

private:
    void CheckStatus();
};
