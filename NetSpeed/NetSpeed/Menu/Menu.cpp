#include "Menu.h"
#include <windowsx.h>

#include "../Dialog/Dialog_Main/Dialog_Main.h"
#include "../Dialog/Dialog_Setting/Dialog_Setting.h"
#include "../Main.h"
#include "../NotifyIcon/NotifyIcon.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

HMENU	Menu::Handle_ = NULL;

//public:

VOID Menu::Init() {
	Handle_ = CreatePopupMenu();
	for(INT Index = 0; Index <= Item::Total - 1; Index++) {
		InsertMenuW(Handle_, 0, MF_STRING, NULL, NULL);
	}
}

VOID Menu::Pop() {
	//Show
	ModifyMenu(
		Handle_,
		Item::Show,
		MF_BYPOSITION | MF_STRING,
		WM_MENU_SHOW,
		(IsWindowVisible(Dialog_Main::Handle_Get()) ? Text::MenuItem_Hide() : Text::MenuItem_Show()).c_str()
	);
	//Startup
	ModifyMenuW(
		Handle_,
		Item::Startup,
		MF_BYPOSITION | MF_STRING | (Registry::Startup_Get() ? MF_CHECKED : MF_UNCHECKED),
		WM_MENU_STARTUP,
		Text::MenuItem_Startup().c_str()
	);
	//Setting
	ModifyMenu(Handle_, Item::Setting, MF_BYPOSITION | MF_STRING, WM_MENU_SETTING, Text::MenuItem_Setting().c_str());
	//Exit
	ModifyMenu(Handle_, Item::Exit, MF_BYPOSITION | MF_STRING, WM_MENU_EXIT, Text::MenuItem_Exit().c_str());
	SetForegroundWindow(Dialog_Main::Handle_Get());
	POINT Point_Cursor;
	GetCursorPos(&Point_Cursor);
	TrackPopupMenu(Handle_, TPM_BOTTOMALIGN | TPM_LEFTALIGN, Point_Cursor.x, Point_Cursor.y, 0, Dialog_Main::Handle_Get(), NULL);
}

//public:

VOID Menu::Item_Exit() {
	if(Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Destroy();
	}
	NotifyIcon::Destroy();
	Dialog_Main::Destroy();
}

VOID Menu::Item_Setting() {
	HWND Handle_DialogSetting = Dialog_Setting::Handle_Get();
	if(Handle_DialogSetting != NULL) {
		SetForegroundWindow(Handle_DialogSetting);
		return;
	}
	DialogBoxW(NULL, MAKEINTRESOURCE(ID_Dialog_Setting), NULL, Dialog_Setting::Process);
}

VOID Menu::Item_Show() {
	ShowWindow(
		Dialog_Main::Handle_Get(),
		Dialog_Main::Show_Get() ? SW_HIDE : SW_SHOW
	);
	if(Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Gather_Check_Show();
	}
}

VOID Menu::Item_Startup() {
	Registry::Startup_Set(!Registry::Startup_Get());
	if(Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Gather_Check_Startup();
	}
}
