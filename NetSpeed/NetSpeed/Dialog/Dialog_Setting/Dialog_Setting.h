#pragma once
#include <WS2tcpip.h>
#include <string>
class Dialog_Setting;
#include "../../Dialog/Dialog.h"
#include "../../Registry.hpp"
#include "../../Text.hpp"

class Dialog_Setting :public Dialog<Dialog_Setting> {
public:
    static INT_PTR WINAPI Process(HWND Handle_Dialog, UINT DialogMessage, WPARAM ParamWORD, LPARAM ParamLONG);
    static void SelectColor(COLORREF* Color_Bind);

public:
    static void Clean() {
        auto CleanFinishMessage = I18N.Message_CleanFinish_;
        Registry::Startup_Set(false);
        Registry::Language_Set(0);
        Dialog_Setting::GatherAll();
        Registry::Clean();
        MessageBoxW(NULL, CleanFinishMessage.c_str(), L"", MB_OK);

    }
    static void Gather_Check_Show();
    static void Gather_Check_Startup();
    static void GatherAll();

private:
    static std::wstring Transparency_String(int Transparency);
};