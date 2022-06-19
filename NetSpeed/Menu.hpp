#pragma once
#include <WS2tcpip.h>
#include <string>
#include "Resource/WM_USER.h"
#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Dialog/Dialog_Setting/Dialog_Setting.h"
#include "NotifyIcon.hpp"

class Menu {
private:
    Menu() {};

public:
    static void Init() {
        Handle_ = CreatePopupMenu();
        for (auto Index = 0; Index <= Item::Total - 1; Index++) {
            InsertMenuW(Handle_, 0, MF_STRING, NULL, NULL);
        }
    }

    static void Pop() {
        //Show
        ModifyMenu(
            Handle_,
            Item::Show,
            MF_BYPOSITION | MF_STRING,
            WM_MENU_SHOW,
            (IsWindowVisible(Dialog_Main::Handle_Get()) ? I18N.MenuItem_Hide_ : I18N.MenuItem_Show_).c_str()
        );
        //Startup
        ModifyMenuW(
            Handle_,
            Item::Startup,
            MF_BYPOSITION | MF_STRING | (Registry::Startup_Get() ? MF_CHECKED : MF_UNCHECKED),
            WM_MENU_STARTUP,
            I18N.MenuItem_Startup_.c_str()
        );
        //Setting
        ModifyMenu(Handle_, Item::Setting, MF_BYPOSITION | MF_STRING, WM_MENU_SETTING, I18N.MenuItem_Setting_.c_str());
        //Exit
        ModifyMenu(Handle_, Item::Exit, MF_BYPOSITION | MF_STRING, WM_MENU_EXIT, I18N.MenuItem_Exit_.c_str());
        SetForegroundWindow(Dialog_Main::Handle_Get());
        POINT Point_Cursor;
        GetCursorPos(&Point_Cursor);
        TrackPopupMenu(Handle_, TPM_BOTTOMALIGN | TPM_LEFTALIGN, Point_Cursor.x, Point_Cursor.y, 0, Dialog_Main::Handle_Get(), NULL);
    }

public:
    static void Item_Exit() {
        if (Dialog_Setting::Handle_Get() != NULL) {
            Dialog_Setting::Destroy();
        }
        NotifyIcon::Destroy();
        Dialog_Main::Destroy();
    }
    static void Item_Setting() {
        auto Handle_DialogSetting = Dialog_Setting::Handle_Get();
        if (Handle_DialogSetting != NULL) {
            SetForegroundWindow(Handle_DialogSetting);
            return;
        }
        DialogBoxW(NULL, MAKEINTRESOURCE(ID_Dialog_Setting), NULL, Dialog_Setting::Process);
    }
    static void Item_Show() {
        ShowWindow(
            Dialog_Main::Handle_Get(),
            Dialog_Main::Show_Get() ? SW_HIDE : SW_SHOW
        );
        if (Dialog_Setting::Handle_Get() != NULL) {
            Dialog_Setting::Gather_Check_Show();
        }
    }
    static void Item_Startup() {
        Registry::Startup_Set(!Registry::Startup_Get());
        if (Dialog_Setting::Handle_Get() != NULL) {
            Dialog_Setting::Gather_Check_Startup();
        }
    }

private:
    enum Item {
        Show,
        Startup,
        Setting,
        Exit,
        Total
    };
    static inline HMENU Handle_ = NULL;
};
