#include "Menu.h"
#include <windowsx.h>
#include "../Dialog_Main/Dialog_Main.h"
#include "../Dialog_Setting/Dialog_Setting.h"
#include "../Main.h"
#include "../NotifyIcon/NotifyIcon.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

HMENU	Menu::Handle_;
BOOL	Menu::Show_ = TRUE;
BOOL	Menu::Startup_ = FALSE;

//public:

VOID Menu::Init() {
	Handle_ = CreatePopupMenu();
	for (INT Index = 0; Index <= Item::Total - 1; Index++) {
		InsertMenuW(Handle_, 0, MF_STRING, NULL, NULL);
	}
}

VOID Menu::Item_Exit() {
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Destroy();
	}
	NotifyIcon::Destroy();
	Dialog_Main::Destroy();
}

VOID Menu::Item_Setting() {
	HWND Handle_Dialog_Setting = Dialog_Setting::Handle_Get();
	if (Handle_Dialog_Setting != NULL) {
		SetForegroundWindow(Handle_Dialog_Setting);
		return;
	}
	DialogBoxW(NULL, MAKEINTRESOURCE(ID_Dialog_Setting), NULL, Dialog_Setting::Process);
}

VOID Menu::Item_Show() {
	if (Show_ == TRUE) {
		ShowWindow(Dialog_Main::Handle_Get(), SW_HIDE);
	}
	else if (Show_ == FALSE) {
		ShowWindow(Dialog_Main::Handle_Get(), SW_SHOW);
	}
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Refresh_Check_Setting_Show();
	}
}

VOID Menu::Item_Startup() {
	Registry::Startup_Set(!Startup_);
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Refresh_Check_Setting_Startup();
	}
}

VOID Menu::Pop() {
	//Show
	Show_ = IsWindowVisible(Dialog_Main::Handle_Get());
	if (Show_ == TRUE) {
		ModifyMenu(Handle_, Item::Show, MF_BYPOSITION | MF_STRING, WM_MENU_SHOW, Text::Menu_Item_Hide().c_str());
	}
	else if (Show_ == FALSE) {
		ModifyMenu(Handle_, Item::Show, MF_BYPOSITION | MF_STRING, WM_MENU_SHOW, Text::Menu_Item_Show().c_str());
	}
	//Startup
	Startup_ = Registry::Startup_Get();
	if (Startup_ == TRUE) {
		ModifyMenuW(Handle_, Item::Startup, MF_BYPOSITION | MF_STRING | MF_CHECKED, WM_MENU_STARTUP, Text::Menu_Item_Startup().c_str());
	}
	else if (Startup_ == FALSE) {
		ModifyMenuW(Handle_, Item::Startup, MF_BYPOSITION | MF_STRING | MF_UNCHECKED, WM_MENU_STARTUP, Text::Menu_Item_Startup().c_str());
	}
	else {
		MessageBoxW(NULL, Text::Message_AdapterFindFail().c_str(), NULL, MB_OK);
	}
	//Setting
	ModifyMenu(Handle_, Item::Setting, MF_BYPOSITION | MF_STRING, WM_MENU_SETTING, Text::Menu_Item_Setting().c_str());
	//Exit
	ModifyMenu(Handle_, Item::Exit, MF_BYPOSITION | MF_STRING, WM_MENU_EXIT, Text::Menu_Item_Exit().c_str());
	SetForegroundWindow(Dialog_Main::Handle_Get());
	POINT Point_Cursor;
	GetCursorPos(&Point_Cursor);
	TrackPopupMenu(Handle_, TPM_BOTTOMALIGN | TPM_LEFTALIGN, Point_Cursor.x, Point_Cursor.y, 0, Dialog_Main::Handle_Get(), NULL);
}