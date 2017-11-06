#include "Main.h"
#include <WS2tcpip.h>
#include <ShellScalingAPI.h>
#include <Shlwapi.h>
#include "Dialog_Main\Dialog_Main.h"
#include "Menu\Menu.h"
#include "Net\Net.h"
#include "NotifyIcon\NotifyIcon.h"
#include "Resource\resource.h"
#include "Text\Text.h"

std::wstring	File_Name;
std::wstring	File_Path;

INT WINAPI wWinMain(HINSTANCE Handle_Instance, HINSTANCE Handle_Instance_Previous, LPWSTR Commond, INT Show_Commend) {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	WCHAR Path_File[MAX_PATH];
	GetModuleFileNameW(NULL, Path_File, MAX_PATH);
	File_Path = Path_File;
	File_Name = PathFindFileNameW(Path_File);
	File_Name.erase(File_Name.end() - 4, File_Name.end());

	Text::Init();
	Menu::Init();
	Net::Init();

	DialogBoxW(Handle_Instance, MAKEINTRESOURCE(ID_Dialog_Main), NULL, Dialog_Main::Process);
	return 0;
}
