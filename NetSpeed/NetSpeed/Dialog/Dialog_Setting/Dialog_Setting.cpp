#include "Dialog_Setting.h"
#include <CommCtrl.h>
#include <windowsx.h>

#include "../../Registry.hpp"
#include "../../Resource/resource.h"
#include "../../Text.hpp"
#include "../Dialog_Main/Dialog_Main.h"

//public:

INT_PTR Dialog_Setting::Process(HWND Handle_Dialog, UINT DialogMessage, WPARAM Param_WORD, LPARAM Param_LONG) {
    Dialog_Setting::Handle_Set(Handle_Dialog);
    bool Result_Process = true;
    switch (DialogMessage) {
    case WM_CLOSE: {
        DestroyWindow(Handle_Dialog);
        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(Param_WORD)) {
        case ID_Button_BackgroundColor: {
            auto Color_Background = Registry::ColorBackground_Get();
            Dialog_Setting::SelectColor(&Color_Background);
            Registry::ColorBackground_Set(Color_Background);
            Dialog_Main::ColorBackground_Set(Color_Background);
            break;
        }
        case ID_Button_Clean: {
            Dialog_Setting::Clean();
            break;
        }
        case ID_Button_CleanAndExit: {
            Dialog_Setting::Clean();
            Dialog_Main::Destroy();
            Dialog_Setting::Destroy();
            break;
        }
        case ID_Button_TextColor: {
            auto Color_Text = Registry::ColorText_Get();
            Dialog_Setting::SelectColor(&Color_Text);
            Registry::ColorText_Set(Color_Text);
            Dialog_Main::ColorText_Set(Color_Text);
            break;
        }
        case ID_Combo_Language: {
            switch (HIWORD(Param_WORD)) {
            case CBN_SELCHANGE: {
                auto Handle_Combo_Language = GetDlgItem(Handle_Dialog, ID_Combo_Language);
                auto Languae_Index = ComboBox_GetCurSel(Handle_Combo_Language);
                Registry::Language_Set(Languae_Index);
                Dialog_Setting::GatherAll();
                break;
            }
            default: {
                Result_Process = false;
            }
            }
            break;
        }
        case ID_Check_Show: {
            auto Handle_Check_Show = GetDlgItem(Handle_Dialog, ID_Check_Show);
            auto Show = Button_GetCheck(Handle_Check_Show);
            Dialog_Main::Show_Set(Show);
            break;
        }
        case ID_Check_Startup: {
            auto Handle_Check_Startup = GetDlgItem(Handle_Dialog, ID_Check_Startup);
            auto Startup = Button_GetCheck(Handle_Check_Startup);
            Registry::Startup_Set(Startup);
            Dialog_Setting::Gather_Check_Startup();
            break;
        }
        case ID_Edit_Size: {
            switch (HIWORD(Param_WORD)) {
            case EN_CHANGE: {
                wchar_t Size_Text[4];
                GetDlgItemTextW(Handle_Dialog, ID_Edit_Size, Size_Text, 4);
                auto Size = _wtoi(Size_Text);
                if (Size == 0) {
                    break;
                }
                if (Size > 128) {
                    SetDlgItemTextW(Handle_Dialog, ID_Edit_Size, L"128");
                    Size = 128;
                }
                Registry::Size_Set(Size);
                Dialog_Main::Size_Set(Size);
                break;
            }
            default: {
                Result_Process = false;
            }
            }
            break;
        }
        default: {
            Result_Process = false;
        }
        }
        break;
    }
    case WM_HSCROLL: {
        auto Handle_Slider_Transparency = GetDlgItem(Handle_Dialog, ID_Slider_Transparency);
        if (Param_LONG == (LPARAM)Handle_Slider_Transparency) {
            int Position_Slider = HIWORD(Param_WORD);
            switch (LOWORD(Param_WORD)) {
            case TB_THUMBTRACK: {
                Dialog_Main::Transparency_Set(Position_Slider);
                auto Transparency_wstring = Dialog_Setting::Transparency_String(Position_Slider);
                SetDlgItemTextW(Handle_Dialog, ID_Static_Transparency, Transparency_wstring.c_str());
                break;
            }
            case TB_THUMBPOSITION: {
                Registry::Transparency_Set(Position_Slider);
                break;
            }
            default: {
                Result_Process = false;
            }
            }
        }
        break;
    }
    case WM_INITDIALOG: {
        Dialog_Setting::GatherAll();
        break;
    }
    case WM_NCDESTROY: {
        Dialog_Setting::Handle_Set(NULL);
        return true;
    }
    default: {
        Result_Process = false;
    }
    }
    return Result_Process;
}

//public:

void Dialog_Setting::SelectColor(COLORREF* Color_Bind) {
    COLORREF Color_Custom[16];
    for (auto ColorCustomIndex = 0; ColorCustomIndex <= 15; ColorCustomIndex++) {
        Color_Custom[ColorCustomIndex] = Registry::ColorCustom_Get(ColorCustomIndex);
    }
    CHOOSECOLOR ColorChooser{
        .lStructSize = sizeof(CHOOSECOLOR),
        .hwndOwner = Dialog_Setting::Handle_Get(),
        .hInstance = NULL,
        .rgbResult = *Color_Bind,
        .lpCustColors = Color_Custom,
        .Flags = CC_FULLOPEN | CC_RGBINIT,
        .lCustData = 0,
        .lpfnHook = NULL,
        .lpTemplateName = NULL,
    };
    ChooseColor(&ColorChooser);
    *Color_Bind = ColorChooser.rgbResult;
    for (auto ColorCustomSetIndex = 0; ColorCustomSetIndex <= 15; ColorCustomSetIndex++) {
        Registry::ColorCustom_Set(ColorCustomSetIndex, Color_Custom[ColorCustomSetIndex]);
    }
}

void Dialog_Setting::Gather_Check_Show() {
    bool Show = Dialog_Main::Show_Get();
    HWND Handle_Dialog = Dialog_Setting::Handle_Get();
    HWND Handle_Check_Show = GetDlgItem(Handle_Dialog, ID_Check_Show);
    Button_SetCheck(Handle_Check_Show, Show);
    SetDlgItemTextW(Handle_Dialog, ID_Check_Show, I18N.MenuItem_Show_.c_str());
}

void Dialog_Setting::Gather_Check_Startup() {
    bool Startup = Registry::Startup_Get();
    HWND Handle_Dialog = Dialog_Setting::Handle_Get();
    HWND Handle_Check_Startup = GetDlgItem(Handle_Dialog, ID_Check_Startup);
    Button_SetCheck(Handle_Check_Startup, Startup);
    SetDlgItemTextW(Handle_Dialog, ID_Check_Startup, I18N.MenuItem_Startup_.c_str());
}

void Dialog_Setting::GatherAll() {
    SetWindowTextW(Dialog_Setting::Handle_Get(), I18N.Dialog_Setting_Caption_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_Clean, I18N.Button_Clean_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_CleanAndExit, I18N.Button_CleanAndExit_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_BackgroundColor, I18N.Button_ColorBackground_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_TextColor, I18N.Button_ColorText_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Language, I18N.Static_Language_.c_str());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Size, I18N.Static_Size_.c_str());
    Dialog_Setting::Gather_Check_Show();
    Dialog_Setting::Gather_Check_Startup();
    HWND Handle_Combo_Language = GetDlgItem(Dialog_Setting::Handle_Get(), ID_Combo_Language);
    ComboBox_ResetContent(Handle_Combo_Language);
    for (auto Index = 0; Index <= Text::Language::Total - 1; Index++) {
        ComboBox_AddString(Handle_Combo_Language, Text::Name(Index).c_str());
    }
    ComboBox_SetCurSel(Handle_Combo_Language, Registry::Language_Get());
    SetDlgItemTextW(
        Dialog_Setting::Handle_Get(),
        ID_Edit_Size,
        std::to_wstring(Registry::Size_Get()).c_str()
    );
    SendMessageW(
        GetDlgItem(Dialog_Setting::Handle_Get(), ID_Slider_Transparency),
        TBM_SETPOS,
        (WPARAM)true,
        (LPARAM)Registry::Transparency_Get()
    );
    std::wstring Transparency_wstring = Dialog_Setting::Transparency_String(Registry::Transparency_Get());
    SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Transparency, Transparency_wstring.c_str());
}

//private:

std::wstring Dialog_Setting::Transparency_String(int Transparency) {
    return I18N.Static_Transparency_ + std::to_wstring(Transparency) + L"%";
}
