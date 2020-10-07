#include "dialogView.h"

#include <psp2/ctrl.h>
#include <screen.h>
#include <utility>

#define DIALOG_WIDTH 760
#define DIALOG_HEIGHT 440
#define DIALOG_PADDING_X 79
#define DIALOG_PADDING_Y 30
#define DIALOG_BTN_WIDTH 322
#define DIALOG_BTN_HEIGHT 56
#define DIALOG_BTN_PADDING 5
#define DIALOG_MSG_HEIGHT 315
#define DIALOG_X ((SCREEN_WIDTH - DIALOG_WIDTH) / 2)
#define DIALOG_Y ((SCREEN_HEIGHT - DIALOG_HEIGHT) / 2)
#define DIALOG_BTN_Y (DIALOG_Y + 365)
#define DIALOG_BTN_INNER_TOP_Y (DIALOG_BTN_Y + DIALOG_BTN_PADDING)
#define DIALOG_BTN_INNER_BOT_Y (DIALOG_BTN_Y + DIALOG_BTN_HEIGHT - DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_1_X (DIALOG_X + DIALOG_WIDTH / 2 - DIALOG_BTN_WIDTH / 2)
#define DIALOG_BTN_1_OF_1_INNER_LEFT_X (DIALOG_BTN_1_OF_1_X + DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_1_INNER_RIGHT_X (DIALOG_BTN_1_OF_1_X + DIALOG_BTN_WIDTH - DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_2_X (DIALOG_X + 44)
#define DIALOG_BTN_1_OF_2_INNER_LEFT_X (DIALOG_BTN_1_OF_2_X + DIALOG_BTN_PADDING)
#define DIALOG_BTN_1_OF_2_INNER_RIGHT_X (DIALOG_BTN_1_OF_2_X + DIALOG_BTN_WIDTH - DIALOG_BTN_PADDING)
#define DIALOG_BTN_2_OF_2_X (DIALOG_X + 394)
#define DIALOG_BTN_2_OF_2_INNER_LEFT_X (DIALOG_BTN_2_OF_2_X + DIALOG_BTN_PADDING)
#define DIALOG_BTN_2_OF_2_INNER_RIGHT_X (DIALOG_BTN_2_OF_2_X + DIALOG_BTN_WIDTH - DIALOG_BTN_PADDING)
#define DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES 45

extern unsigned char _binary_assets_spr_img_dialog_msg_bg_png_start;
extern unsigned char _binary_assets_spr_img_dialog_msg_btn_png_start;
extern unsigned char _binary_assets_spr_img_dialog_msg_btn_active_png_start;
extern unsigned char _binary_assets_spr_img_dialog_msg_btn_focus_png_start;

DialogView::DialogView()
    : msg_font(Font(std::string(FONT_DIR "segoeui.ttf"), 28))
    , btn_font(Font(std::string(FONT_DIR "segoeui.ttf"), 25))
    , img_dialog_msg_bg(Texture(&_binary_assets_spr_img_dialog_msg_bg_png_start))
    , img_dialog_msg_btn(Texture(&_binary_assets_spr_img_dialog_msg_btn_png_start))
    , img_dialog_msg_btn_active(Texture(&_binary_assets_spr_img_dialog_msg_btn_active_png_start))
    , img_dialog_msg_btn_focus(Texture(&_binary_assets_spr_img_dialog_msg_btn_focus_png_start))
{
    priority = 550;
    log_printf(DBG_DEBUG, "DialogView::DialogView()");
    // FIXME HACK: when DialogView is passed to Activity::AddView() it's destroyed once the activity is crashed
    // Keeping an internal shared_ptr of itself makes sure that it's never destroyed
    me_ptr = std::shared_ptr<DialogView>(this);
}

DialogView::~DialogView()
{
    log_printf(DBG_WARNING, "DialogView destructor called");
}

void DialogView::openDialogView(std::shared_ptr<DialogViewResult> result, const std::string& message, DialogType type)
{
    DialogView* dialogView = DialogView::create_instance();
    dialogView->prepare(std::move(result), message, type);
    Activity::get_instance()->AddView(dialogView->me_ptr);
}

void DialogView::finalErrorDialog(const std::string& message)
{
    auto res = std::make_shared<DialogViewResult>();
    log_printf(DBG_DEBUG, "finalErrorDialog: %s", message.c_str());
    openDialogView(res, message, DIALOG_TYPE_OK);
    while (res->status != COMMON_DIALOG_STATUS_FINISHED) {
        sceKernelDelayThread(10000);
    }
    Activity::get_instance()->exitFlag = true;
}

void DialogView::prepare(std::shared_ptr<DialogViewResult> result, const std::string& message, DialogType type)
{
    log_printf(DBG_DEBUG, "Created DialogView \"%s\"", message.c_str());
    if (_status == COMMON_DIALOG_STATUS_RUNNING)
    {
        log_printf(DBG_WARNING, "Canceling current DialogView");
        request_destroy = true;
        _status = COMMON_DIALOG_STATUS_CANCELED;
        if (_result)
            _result->status = COMMON_DIALOG_STATUS_CANCELED;
    }
    _result = result;
    _message = msg_font.FitString(message, DIALOG_WIDTH - DIALOG_PADDING_X * 2);
    request_destroy = false;
    _accepted = false;
    _status = COMMON_DIALOG_STATUS_RUNNING;
    _btnTouched = _btnFocus = -1;
    if (_result)
    {
        _result->status = COMMON_DIALOG_STATUS_RUNNING;
    }
    _type = type;
}

int DialogView::GetGlowCycleAlpha()
{
    float alphaCount;
    if (_focusGlowCycle <= DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES)
    {
        alphaCount = _focusGlowCycle;
    }
    else
    {
        alphaCount = DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES * 2 - _focusGlowCycle;
    }
    return int(alphaCount / DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES * 255);
}

void DialogView::HandleBtnFocus(const Input& input)
{
    auto btnFocusBefore = _btnFocus;
    if (input.KeyNewPressed(SCE_CTRL_RIGHT))
    {
        switch (_type)
        {
            case DIALOG_TYPE_OK:
                _btnFocus = 0;
                break;
            case DIALOG_TYPE_YESNO:
                _btnFocus = 1;
                break;
        }
        log_printf(DBG_DEBUG, "SCE_CTRL_RIGHT, _btnFocus %i", _btnFocus);
    }
    if (input.KeyNewPressed(SCE_CTRL_LEFT) || input.KeyNewPressed(SCE_CTRL_UP) || input.KeyNewPressed(SCE_CTRL_DOWN))
    {
        _btnFocus = 0;
        log_printf(DBG_DEBUG, "SCE_CTRL_LEFT||SCE_CTRL_UP||SCE_CTRL_DOWN , _btnFocus %i", _btnFocus);
    }
    if (input.TouchPressed())
    {
        _btnFocus = -1;
    }
    if (_btnFocus >= 0)
    {
        if (btnFocusBefore < 0 || // focus button after none was focused
            (btnFocusBefore != _btnFocus && GetGlowCycleAlpha() < 200))
        { // change focused button and highlight was nearly gone
            log_printf(DBG_DEBUG, "Resetting glow cycle");
            _focusGlowCycle = 0;
        }
    }
    if (input.KeyNewPressed(SCE_CTRL_ENTER) && _btnFocus >= 0)
    {
        log_printf(DBG_DEBUG, "SCE_CTRL_ENTER, choosing button %i", _btnFocus);
        _status = COMMON_DIALOG_STATUS_FINISHED;
        if (_type == DIALOG_TYPE_YESNO && _btnFocus == 1)
        {
            _accepted = true;
        }
    }
}

int DialogView::GetTouchedBtnIdx(const Input& input)
{
    if (!input.TouchPressed())
        return -1;
    switch (_type)
    {
        case DIALOG_TYPE_OK:
        {
            auto btn_tl = Point(DIALOG_BTN_1_OF_1_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y);
            auto btn_br = Point(DIALOG_BTN_1_OF_1_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y);
            if (input.TouchInRectangle(Rectangle(btn_tl, btn_br)))
            {
                return 0;
            }
            break;
        }
        case DIALOG_TYPE_YESNO:
            // NO button
            auto btn_tl = Point(DIALOG_BTN_1_OF_2_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y);
            auto btn_br = Point(DIALOG_BTN_1_OF_2_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y);
            if (input.TouchInRectangle(Rectangle(btn_tl, btn_br)))
            {
                return 0;
            }
            // YES button
            btn_tl = Point(DIALOG_BTN_2_OF_2_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y);
            btn_br = Point(DIALOG_BTN_2_OF_2_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y);
            if (input.TouchInRectangle(Rectangle(btn_tl, btn_br)))
            {
                return 1;
            }
            break;
    }
    return -1;
}

void DialogView::HandleBtnTouch(const Input& input)
{
    if (input.TouchPressed())
    {
        if (input.TouchNewPressed())
        {
            _btnTouched = GetTouchedBtnIdx(input);
        }
        else if (_btnTouched >= 0 && _btnTouched != GetTouchedBtnIdx(input))
        {
            _btnTouched = -1;
        }
    }
    else
    {
        if (_btnTouched >= 0)
        {
            _status = COMMON_DIALOG_STATUS_FINISHED;
            if (_type == DIALOG_TYPE_YESNO && _btnTouched == 1)
            {
                _accepted = true;
            }
            _btnTouched = -1;
        }
    }
}

int DialogView::HandleInput(int focus, const Input& input)
{
    if (!focus)
        return 0;
    auto oldStatus = _status;
    HandleBtnFocus(input);
    HandleBtnTouch(input);

    if (oldStatus != _status)
    {
        if (_result)
        {
            _result->accepted = _accepted;
            _result->status = _status;
        }
        if (_status == COMMON_DIALOG_STATUS_FINISHED || _status == COMMON_DIALOG_STATUS_CANCELED)
        {
            request_destroy = true;
            _status = COMMON_DIALOG_STATUS_NONE;
        }
    }

    return 0;
}

void DialogView::DrawBtn(const std::string& text, const Point& sprPt, const Rectangle& textRect, int idx)
{
    if (_btnTouched == idx)
    {
        img_dialog_msg_btn_active.Draw(sprPt);
    }
    else
    {
        img_dialog_msg_btn.Draw(sprPt);
    }
    btn_font.DrawCentered(textRect, text, COLOR_WHITE, true);
    if (_btnFocus == idx)
    {
        img_dialog_msg_btn_focus.DrawExt(sprPt, GetGlowCycleAlpha());
    }
}

int DialogView::Display()
{
    if (request_destroy)
    { // done here!
        return 0;
    }

    // backdrop
    vita2d_draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGBA8(0, 0, 0, 190));

    img_dialog_msg_bg.Draw(Point(DIALOG_X, DIALOG_Y));

    msg_font.DrawCenteredVertical(
        Rectangle(
            Point(DIALOG_X + DIALOG_PADDING_X, DIALOG_Y + DIALOG_PADDING_Y),
            Point(DIALOG_X + DIALOG_WIDTH - DIALOG_PADDING_X, DIALOG_Y + DIALOG_PADDING_Y + DIALOG_MSG_HEIGHT)),
        _message, COLOR_WHITE, true);

    switch (_type)
    {
        case DIALOG_TYPE_OK:
            DrawBtn(
                "Ok", Point(DIALOG_BTN_1_OF_1_X, DIALOG_BTN_Y),
                Rectangle(
                    Point(DIALOG_BTN_1_OF_1_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y),
                    Point(DIALOG_BTN_1_OF_1_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y)),
                0);
            break;
        case DIALOG_TYPE_YESNO:
            DrawBtn(
                "No", Point(DIALOG_BTN_1_OF_2_X, DIALOG_BTN_Y),
                Rectangle(
                    Point(DIALOG_BTN_1_OF_2_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y),
                    Point(DIALOG_BTN_1_OF_2_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y)),
                0);
            DrawBtn(
                "Yes", Point(DIALOG_BTN_2_OF_2_X, DIALOG_BTN_Y),
                Rectangle(
                    Point(DIALOG_BTN_2_OF_2_INNER_LEFT_X, DIALOG_BTN_INNER_TOP_Y),
                    Point(DIALOG_BTN_2_OF_2_INNER_RIGHT_X, DIALOG_BTN_INNER_BOT_Y)),
                1);
            break;
    }

    _focusGlowCycle = (_focusGlowCycle + 1) % (DIALOG_FOCUS_GLOW_HALF_CYCLE_FRAMES * 2);

    return 0;
}
