#include "Main.h"
#include <WS2tcpip.h>
#include <ShellScalingAPI.h>
#include <Shlwapi.h>
#include "DialogMain\DialogMain.h"
#include "Menu\Menu.h"
#include "Net\Net.h"
#include "NotifyIcon\NotifyIcon.h"
#include "Resource\resource.h"

std::wstring file_name;
std::wstring file_path;

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, INT iCmdShow) {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	WCHAR path_file[MAX_PATH];
	GetModuleFileNameW(NULL, path_file, MAX_PATH);
	file_path = path_file;
	file_name = PathFindFileNameW(path_file);
	file_name.erase(file_name.end() - 4, file_name.end());

	Menu::Init();
	Net::Init();

	DialogBoxW(hInstance, MAKEINTRESOURCE(DIALOG_MAIN), NULL, DialogMainProcess);
	return 0;
}
