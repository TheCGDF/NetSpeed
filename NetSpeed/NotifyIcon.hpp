#pragma once
#include <WS2tcpip.h>
#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Resource/resource.h"
#include "Resource/WM_USER.h"

class NotifyIcon {
private:
    NotifyIcon() {};

public:
    static void Destroy() {
        Shell_NotifyIconW(NIM_DELETE, &Data_);
    }

    static void Init() {
        Data_.cbSize = sizeof(Data_);
        Data_.hIcon = (HICON)LoadImageW(
            GetModuleHandleW(NULL),
            MAKEINTRESOURCEW(ID_Icon_Main),
            IMAGE_ICON,
            0,
            0,
            LR_SHARED
        );
        Data_.uCallbackMessage = WM_NOTIFYICON;
        Data_.hWnd = Dialog_Main::Handle_Get();
        Data_.uID = ID_NotifyIcon_Main;
        Data_.uVersion = NOTIFYICON_VERSION_4;

        wchar_t FileName[MAX_PATH];
        GetModuleFileNameW(NULL, FileName, MAX_PATH);
        wchar_t FileVersionInfo[MAX_PATH];
        VS_FIXEDFILEINFO* FixedFileInfo = NULL;
        GetFileVersionInfoW(FileName, NULL, MAX_PATH, &FileVersionInfo);
        VerQueryValueW(&FileVersionInfo, L"\\", (LPVOID*)&FixedFileInfo, NULL);
        wchar_t Tip[MAX_PATH];
        swprintf(Tip, MAX_PATH, L"NetSpeed v%hu.%hu.%hu.%hu",
            HIWORD(FixedFileInfo->dwFileVersionMS),
            LOWORD(FixedFileInfo->dwFileVersionMS),
            HIWORD(FixedFileInfo->dwFileVersionLS),
            LOWORD(FixedFileInfo->dwFileVersionLS)
        );
        wcscpy_s(Data_.szTip, Tip);
        Data_.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
        Shell_NotifyIconW(NIM_ADD, &Data_);
    }

private:
    static inline NOTIFYICONDATAW Data_;//todo must inline?
};