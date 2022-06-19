#include "Main.h"
#include <WS2tcpip.h>
#include <ShellScalingAPI.h>
#include <Shlwapi.h>

#include "Text.hpp"
#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Menu.hpp"
#include "Timer_Refresh.hpp"
#include "NotifyIcon.hpp"
#include "Resource/resource.h"

std::wstring FileName;
std::wstring FilePath;

int WINAPI wWinMain(HINSTANCE Handle_Instance, HINSTANCE Handle_PreviousInstance, LPWSTR Commond, int ShowCommend) {
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    wchar_t PathFile[MAX_PATH];
    GetModuleFileNameW(NULL, PathFile, MAX_PATH);
    FilePath = PathFile;
    FileName = PathFindFileNameW(PathFile);
    FileName.erase(FileName.end() - 4, FileName.end());

    Text::Init();
    Menu::Init();

    DialogBoxW(Handle_Instance, MAKEINTRESOURCE(ID_Dialog_Main), NULL, Dialog_Main::Process);
    return 0;
}
