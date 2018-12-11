#include "Main.h"
#include <ShellScalingAPI.h>
#include <Shlwapi.h>

#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Menu/Menu.h"
#include "Timer_Refresh/Timer_Refresh.h"
#include "NotifyIcon/NotifyIcon.h"
#include "Resource/resource.h"
#include "Text/Text.h"

std::wstring	FileName;
std::wstring	FilePath;

INT WINAPI wWinMain(HINSTANCE Handle_Instance, HINSTANCE Handle_PreviousInstance, LPWSTR Commond, INT ShowCommend) {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	WCHAR PathFile[MAX_PATH];
	GetModuleFileNameW(NULL, PathFile, MAX_PATH);
	FilePath = PathFile;
	FileName = PathFindFileNameW(PathFile);
	FileName.erase(FileName.end() - 4, FileName.end());

	Text::Init();
	Menu::Init();

	DialogBoxW(Handle_Instance, MAKEINTRESOURCE(ID_Dialog_Main), NULL, Dialog_Main::Process);
	return 0;
}
