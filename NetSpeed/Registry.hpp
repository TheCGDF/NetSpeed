#pragma once
#include <WS2tcpip.h>
#include <set>
#include <string>

#include "Text.hpp"
#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Main.h"

class Registry {
private:
    Registry() {};

public:
    static void Clean() {
        RegDeleteTreeW(HKEY_CURRENT_USER, Path_NetSpeed_.c_str());
    }

public:
    static COLORREF ColorBackground_Get() {
        auto ColorBackground = RGB(0, 0, 0);
        Registry::Value_Get(Path_NetSpeed_, Value_ColorBackground_, RRF_RT_REG_DWORD, &ColorBackground);
        return ColorBackground;
    }

    static void ColorBackground_Set(COLORREF ColorBackground) {
        Value_Set(Path_NetSpeed_, Value_ColorBackground_, REG_DWORD, (PBYTE)&ColorBackground, sizeof(COLORREF));
    }

    static COLORREF ColorCustom_Get(int ColorCustomIndex) {
        auto ColorCustom = RGB(255, 255, 255);
        Registry::Value_Get(Path_NetSpeed_, Value_ColorCustom_[ColorCustomIndex], RRF_RT_REG_DWORD, &ColorCustom);
        return ColorCustom;
    }

    static void ColorCustom_Set(int ColorCustomIndex, COLORREF ColorCustom) {
        Registry::Value_Set(Path_NetSpeed_, Value_ColorCustom_[ColorCustomIndex], REG_DWORD, (PBYTE)&ColorCustom, sizeof(COLORREF));
    }

    static COLORREF ColorText_Get() {
        auto ColorText = RGB(255, 255, 255);
        Registry::Value_Get(Path_NetSpeed_, Value_ColorText_, RRF_RT_REG_DWORD, &ColorText);
        return ColorText;
    }

    static void ColorText_Set(COLORREF ColorText) {
        Registry::Value_Set(Path_NetSpeed_, Value_ColorText_, REG_DWORD, (PBYTE)&ColorText, sizeof(COLORREF));
    }

    static int Language_Get() {
        wchar_t Language[100];
        auto LanguageStatus = Registry::Value_Get(Path_NetSpeed_, Value_Language_, RRF_RT_REG_SZ, Language);
        if (LanguageStatus == ERROR_SUCCESS) {
            return Text::Language_Get(Language);
        }
        return 0;
    }

    static void Language_Set(int LanguageIndex) {
        auto LanguageName = Text::Name(LanguageIndex);
        Registry::Value_Set(Path_NetSpeed_, Value_Language_.c_str(), REG_SZ, (PBYTE)LanguageName.c_str(), LanguageName.length() * sizeof(WCHAR));
    }

    static std::set<std::wstring> PCI_Get() {
        std::set<std::wstring> Set_PCI;
        HKEY Key_Network;
        RegCreateKeyExW(
            HKEY_LOCAL_MACHINE,
            Path_Network_.c_str(),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_READ,
            NULL,
            &Key_Network,
            NULL
        );
        auto PathIndex = 0;
        WCHAR PathName[MAX_PATH];
        DWORD Name_Size = MAX_PATH;
        while (RegEnumKeyExW(Key_Network, PathIndex, PathName, &Name_Size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            if (PathName[0] == L'{') {
                auto Path_New = Path_Network_ + L"\\" + PathName + L"\\Connection";
                wchar_t Value[MAX_PATH];
                DWORD Value_Size = MAX_PATH;
                RegGetValueW(
                    HKEY_LOCAL_MACHINE,
                    Path_New.c_str(),
                    L"PnPInstanceId",
                    RRF_RT_REG_SZ,
                    NULL,
                    Value,
                    &Value_Size
                );
                if (std::wstring(Value).find(L"PCI") == 0) {
                    Set_PCI.insert(PathName);
                }
            }
            PathIndex++;
            Name_Size = MAX_PATH;
        }
        RegCloseKey(Key_Network);
        return Set_PCI;
    }

    static POINT Position_Get() {
        POINT Position;
        auto PositionXStatus = Registry::Value_Get(Path_NetSpeed_, Value_PositionX_, RRF_RT_REG_DWORD, &Position.x);
        auto PositionYStatus = Registry::Value_Get(Path_NetSpeed_, Value_PositionY_, RRF_RT_REG_DWORD, &Position.y);
        RECT Rect;
        GetWindowRect(Dialog_Main::Handle_Get(), &Rect);
        if (PositionXStatus != ERROR_SUCCESS || PositionYStatus != ERROR_SUCCESS) {
            Position.x = Rect.left;
            Position.y = Rect.top;
        }
        //GetSystemMetrics会包含任务栏，但由于任务栏可能出现在各种位置，因此不作处理
        if (Rect.right - Rect.left < GetSystemMetrics(SM_CXSCREEN) &&
            Rect.bottom - Rect.top < GetSystemMetrics(SM_CYSCREEN) &&
            (Position.x < 0 || GetSystemMetrics(SM_CXSCREEN) < Position.x + Rect.right - Rect.left ||
                Position.y < 0 || GetSystemMetrics(SM_CYSCREEN) < Position.y + Rect.bottom - Rect.top)) {
            Position.x = (GetSystemMetrics(SM_CXSCREEN) - Rect.right + Rect.left) / 2;
            Position.y = (GetSystemMetrics(SM_CYSCREEN) - Rect.bottom + Rect.top) / 2;
            Dialog_Main::Position_Set(Position);
        }
        Registry::Position_Set(Position);
        return Position;
    }

    static void Position_Set(POINT Position) {
        Registry::Value_Set(Path_NetSpeed_, Value_PositionX_.c_str(), REG_DWORD, (PBYTE)&Position.x, sizeof(long));
        Registry::Value_Set(Path_NetSpeed_, Value_PositionY_.c_str(), REG_DWORD, (PBYTE)&Position.y, sizeof(long));
    }

    static int Size_Get() {
        auto Size = 12;
        Registry::Value_Get(Path_NetSpeed_, Value_Size_, RRF_RT_REG_DWORD, &Size);
        return Size;
    }

    static void Size_Set(int Size) {
        Registry::Value_Set(Path_NetSpeed_, Value_Size_, REG_DWORD, (PBYTE)&Size, sizeof(int));
    }

    static bool Startup_Get() {
        auto StartupStatus = Registry::Value_Get(Path_Startup_, FileName, RRF_RT_REG_SZ, NULL);
        if (StartupStatus != ERROR_SUCCESS) {
            return false;
        }
        //Rest the .exe path
        Registry::Value_Set(Path_Startup_, FileName, REG_SZ, (PBYTE)FilePath.c_str(), FilePath.length() * sizeof(wchar_t));
        return true;
    }

    static void Startup_Set(bool Startup) {
        if (Startup) {
            Registry::Value_Set(Path_Startup_, FileName, REG_SZ, (PBYTE)FilePath.c_str(), FilePath.length() * sizeof(wchar_t));
        }
        else {
            RegDeleteKeyValueW(HKEY_CURRENT_USER, Path_Startup_.c_str(), FileName.c_str());
        }
    }

    static int Transparency_Get() {
        auto Transparency = 65;
        Registry::Value_Get(Path_NetSpeed_, Value_Transparency_, RRF_RT_REG_DWORD, &Transparency);
        return Transparency;
    }

    static void Transparency_Set(int Transparency) {
        Registry::Value_Set(Path_NetSpeed_, Value_Transparency_, REG_DWORD, (PBYTE)&Transparency, sizeof(int));
    }

private:
    static inline const std::wstring Path_NetSpeed_ = L"Software\\NetSpeed";
    static inline const std::wstring Path_Network_ = L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}";
    static inline const std::wstring Path_Startup_ = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    static inline const std::wstring Value_ColorBackground_ = L"ColorBackground";
    static inline const std::wstring Value_ColorCustom_[16] = {
        L"ColorCustom00",
        L"ColorCustom01",
        L"ColorCustom02",
        L"ColorCustom03",
        L"ColorCustom04",
        L"ColorCustom05",
        L"ColorCustom06",
        L"ColorCustom07",
        L"ColorCustom08",
        L"ColorCustom09",
        L"ColorCustom10",
        L"ColorCustom11",
        L"ColorCustom12",
        L"ColorCustom13",
        L"ColorCustom14",
        L"ColorCustom15",
    };
    static inline const std::wstring Value_ColorText_ = L"ColorText";
    static inline const std::wstring Value_PositionX_ = L"Position_X";
    static inline const std::wstring Value_PositionY_ = L"Position_Y";
    static inline const std::wstring Value_Size_ = L"Size";
    static inline const std::wstring Value_Transparency_ = L"Transparency";
    static inline const std::wstring Value_Language_ = L"Language";

    static LSTATUS Value_Get(std::wstring Path, std::wstring Value, DWORD Flag, PVOID Data) {
        DWORD Size_Data = MAX_PATH;
        return RegGetValueW(
            HKEY_CURRENT_USER,
            Path.c_str(),
            Value.c_str(),
            Flag,
            NULL,
            Data,
            &Size_Data
        );
    }

    static void Value_Set(std::wstring Path, std::wstring Value, DWORD Type, PBYTE Data, DWORD Size) {
        HKEY Key_Path;
        RegCreateKeyExW(
            HKEY_CURRENT_USER,
            Path.c_str(),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,
            &Key_Path,
            NULL
        );
        RegSetValueExW(Key_Path, Value.c_str(), 0, Type, Data, Size);
        RegCloseKey(Key_Path);
    }
};