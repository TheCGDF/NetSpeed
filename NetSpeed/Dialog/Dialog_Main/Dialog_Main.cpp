#include "Dialog_Main.h"

#include "../../Menu.hpp"
#include "../../Timer_Refresh.hpp"
#include "../../NotifyIcon.hpp"
#include "../../Registry.hpp"
#include "../../Resource/resource.h"
#include "../Dialog_Setting/Dialog_Setting.h"

COLORREF Dialog_Main::Color_Background_;
COLORREF Dialog_Main::Color_Text_;
long  Dialog_Main::DialogOriginHeight_;
long  Dialog_Main::DialogOriginWidth_;
HBRUSH Dialog_Main::Handle_Bursh_Background_ = NULL;
HBRUSH Dialog_Main::Handle_Bursh_Text_ = NULL;
HFONT Dialog_Main::Handle_Font_Text_ = NULL;
HPEN  Dialog_Main::Handle_Pen_Background_ = NULL;
long  Dialog_Main::TextOriginHeight_;
long  Dialog_Main::TextOriginWidth_;

//public:

INT_PTR Dialog_Main::Process(HWND Handle_Dialog, UINT DialogMessage, WPARAM ParamWORD, LPARAM ParamLONG) {
    Dialog_Main::Handle_Set(Handle_Dialog);
    bool ProcessResult = true;
    switch (DialogMessage) {
    case WM_COMMAND: {
        switch (LOWORD(ParamWORD)) {
        case WA_INACTIVE: {
            SetWindowPos(Handle_Dialog, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;
        }
        case WM_MENU_EXIT: {
            Menu::Item_Exit();
            break;
        }
        case WM_MENU_SETTING: {
            Menu::Item_Setting();
            break;
        }
        case WM_MENU_SHOW: {
            Menu::Item_Show();
            break;
        }
        case WM_MENU_STARTUP: {
            Menu::Item_Startup();
            break;
        }
        default: {
            ProcessResult = false;
            break;
        }
        }
        break;
    }
    case WM_CTLCOLORDLG: {
        return (INT_PTR)Handle_Bursh_Background_;
    }
    case WM_CTLCOLORSTATIC: {
        SetBkMode((HDC)ParamWORD, TRANSPARENT);
        SetTextColor((HDC)ParamWORD, Color_Text_);
        return (INT_PTR)Handle_Bursh_Background_;
    }
    case WM_DESTROY: {
        NotifyIcon::Destroy();
        return 0;
    }
    case WM_ERASEBKGND: {
        RECT DialogRect;
        SelectObject((HDC)ParamWORD, Handle_Pen_Background_);
        SelectObject((HDC)ParamWORD, Handle_Bursh_Background_);
        GetClientRect(Handle_Dialog, &DialogRect);
        Rectangle((HDC)ParamWORD, DialogRect.left, DialogRect.top, DialogRect.right, DialogRect.bottom);
        break;
    }
    case WM_EXITSIZEMOVE: {
        RECT Rect;
        GetWindowRect(Handle_Get(), &Rect);
        Registry::Position_Set(POINT{ .x = Rect.left,.y = Rect.top });
        break;
    }
    case WM_INITDIALOG: {
        Color_Background_ = Registry::ColorBackground_Get();
        Color_Text_ = Registry::ColorText_Get();
        Dialog_Main::ColorBackground_Set(Color_Background_);

        auto Position = Registry::Position_Get();
        Dialog_Main::Position_Set(Position);

        SetWindowLongW(
            Handle_Dialog,
            GWL_EXSTYLE,
            GetWindowLongW(Handle_Dialog, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW
        );

        auto Transparency = Registry::Transparency_Get();
        Dialog_Main::Transparency_Set(Transparency);

        RECT DialogRect;
        GetWindowRect(Handle_Dialog, &DialogRect);
        DialogOriginHeight_ = DialogRect.bottom - DialogRect.top;
        DialogOriginWidth_ = DialogRect.right - DialogRect.left;
        RECT TextRect;
        GetWindowRect(GetDlgItem(Handle_Dialog, ID_Static_Upload), &TextRect);
        TextOriginHeight_ = TextRect.bottom - TextRect.top;
        TextOriginWidth_ = TextRect.right - TextRect.left;

        auto Size = Registry::Size_Get();
        Dialog_Main::Size_Set(Size);

        Timer_Refresh::Init();
        Timer_Refresh::Start();
        break;
    }
    case WM_LBUTTONDOWN: {
        SendMessageW(Handle_Dialog, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(LOWORD(ParamLONG), HIWORD(ParamLONG)));
        break;
    }
    case WM_WINDOWPOSCHANGING: {
        //该消息会一直发送
        PWINDOWPOS WindowPosition = (PWINDOWPOS)ParamLONG;
        if (WindowPosition->flags == (SWP_NOMOVE | SWP_NOSIZE)) {
            break;
        }
        RECT Rect;
        GetWindowRect(Dialog_Main::Handle_Get(), &Rect);
        if (Rect.right - Rect.left < GetSystemMetrics(SM_CXSCREEN)) {
            if (Rect.left < 0) {
                WindowPosition->x = 0;
            }
            if (GetSystemMetrics(SM_CXSCREEN) < Rect.right) {
                WindowPosition->x = GetSystemMetrics(SM_CXSCREEN) - Rect.right + Rect.left;
            }
        }
        if (Rect.bottom - Rect.top < GetSystemMetrics(SM_CYSCREEN)) {
            if (Rect.top < 0) {
                WindowPosition->y = 0;
            }
            if (GetSystemMetrics(SM_CYSCREEN) < Rect.bottom) {
                WindowPosition->y = GetSystemMetrics(SM_CYSCREEN) - Rect.bottom + Rect.top;
            }
        }
        break;
    }
    case WM_NCPAINT: {
        NotifyIcon::Init();
        break;
    }
    case WM_NOTIFYICON: {
        switch (ParamLONG) {
        case WM_RBUTTONUP: {
            Menu::Pop();
            return true;
        }
        default: {
            ProcessResult = false;
            break;
        }
        }
        break;
    }
    case WM_RBUTTONUP: {
        Menu::Pop();
        break;
    }
    default: {
        ProcessResult = false;
        break;
    }
    }
    return ProcessResult;
}

//public:

void Dialog_Main::ColorBackground_Set(COLORREF Background_Color) {
    Color_Background_ = Background_Color;
    if (Handle_Bursh_Background_ != NULL) {
        DeleteObject(Handle_Bursh_Background_);
    }
    if (Handle_Pen_Background_ != NULL) {
        DeleteObject(Handle_Pen_Background_);
    }
    Handle_Bursh_Background_ = CreateSolidBrush(Color_Background_);
    Handle_Pen_Background_ = CreatePen(PS_SOLID, 0, Color_Background_);
    InvalidateRect(Dialog_Main::Handle_Get(), NULL, true);
}

void Dialog_Main::ColorText_Set(COLORREF Text_Color) {
    Color_Text_ = Text_Color;
    InvalidateRect(Dialog_Main::Handle_Get(), NULL, true);
}

void Dialog_Main::Position_Set(POINT Position) {
    SetWindowPos(Dialog_Main::Handle_Get(), NULL, Position.x, Position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

bool Dialog_Main::Show_Get() {
    return IsWindowVisible(Dialog_Main::Handle_Get());
}

void Dialog_Main::Show_Set(bool Show) {
    auto Handle_Dialog = Dialog_Main::Handle_Get();
    ShowWindow(Handle_Dialog, Show ? SW_SHOW : SW_HIDE);
    if (Handle_Dialog != NULL) {
        Dialog_Setting::Gather_Check_Show();
    }
}

void Dialog_Main::Size_Set(int Size) {
    LOGFONTW FontInfo;
    ZeroMemory(&FontInfo, sizeof(LOGFONTW));
    auto Handle_Dialog = Dialog_Main::Handle_Get();
    auto Handle_DC = GetDC(NULL);
    FontInfo.lfHeight = -MulDiv(Size, GetDeviceCaps(Handle_DC, LOGPIXELSY), 72);
    wcscpy_s(FontInfo.lfFaceName, LF_FACESIZE, L"Consolas");
    FontInfo.lfWeight = 400;
    if (Handle_Font_Text_ != NULL) {
        DeleteObject(Handle_Font_Text_);
    }
    Handle_Font_Text_ = CreateFontIndirectW(&FontInfo);
    ReleaseDC(NULL, Handle_DC);
    SendDlgItemMessageW(Handle_Dialog, ID_Static_Download, WM_SETFONT, (WPARAM)Handle_Font_Text_, true);
    SendDlgItemMessageW(Handle_Dialog, ID_Static_Upload, WM_SETFONT, (WPARAM)Handle_Font_Text_, true);

    SetWindowPos(
        Handle_Dialog,
        NULL,
        NULL,
        NULL,
        DialogOriginWidth_ * Size / 12,
        DialogOriginHeight_ * Size / 12,
        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER
    );
    SetWindowPos(
        GetDlgItem(Handle_Dialog, ID_Static_Download),
        NULL,
        NULL,
        NULL,
        TextOriginWidth_ * Size / 12,
        TextOriginHeight_ * Size / 12,
        SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER
    );
    SetWindowPos(
        GetDlgItem(Handle_Dialog, ID_Static_Upload),
        NULL,
        DialogOriginWidth_ * Size / 12 / 2 + 2 * TextOriginWidth_ / 28,
        NULL,
        TextOriginWidth_ * Size / 12,
        TextOriginHeight_ * Size / 12,
        SWP_NOACTIVATE | SWP_NOZORDER
    );
    InvalidateRect(Handle_Dialog, NULL, true);
}

void Dialog_Main::Transparency_Set(int Transparency) {
    SetLayeredWindowAttributes(Dialog_Main::Handle_Get(), 0, (255.0 * Transparency) / 100.0 + 0.5, LWA_ALPHA);
}
