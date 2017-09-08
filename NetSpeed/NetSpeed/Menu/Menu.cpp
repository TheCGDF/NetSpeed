#include "Menu.h"
#include "MENUITEM.h"
#include "../Main.h"
#include "../DialogMain/DialogMain.h"
#include "../NotifyIcon/NotifyIcon.h"
#include "../Registry/Registry.h"
#include "../Registry/REGISTRYPATH.h"

BOOL Menu::startup_ = FALSE;
HMENU Menu::handle_;
BOOL Menu::show_ = TRUE;

VOID Menu::Pop() {
	show_ = IsWindowVisible(dialog_main);
	if (show_ == TRUE) {
		ModifyMenu(handle_, MENUITEM_SHOW, MF_BYPOSITION | MF_STRING, WM_MENUSHOW, L"Hide");
	}
	else if (show_ == FALSE) {
		ModifyMenu(handle_, MENUITEM_SHOW, MF_BYPOSITION | MF_STRING, WM_MENUSHOW, L"Show");
	}

	LSTATUS startup_status = Registry::Startup_Status();
	if (startup_status == ERROR_SUCCESS) {
		startup_ = TRUE;
		ModifyMenuW(handle_, MENUITEM_STARTUP, MF_BYPOSITION | MF_STRING | MF_CHECKED, WM_MENUSTARTUP, L"Startup");
	}
	else if (startup_status == ERROR_FILE_NOT_FOUND) {
		startup_ = FALSE;
		ModifyMenuW(handle_, MENUITEM_STARTUP, MF_BYPOSITION | MF_STRING | MF_UNCHECKED, WM_MENUSTARTUP, L"Startup");
	}
	else {
		MessageBoxW(NULL, L"Can't open registry.", L"Error", MB_OK);
	}

	SetForegroundWindow(dialog_main);
	POINT cursor;
	GetCursorPos(&cursor);
	TrackPopupMenu(handle_, TPM_BOTTOMALIGN | TPM_LEFTALIGN, cursor.x, cursor.y, 0, dialog_main, NULL);
}

VOID Menu::Init() {
	handle_ = CreatePopupMenu();
	InsertMenuW(handle_, 0, MF_STRING, WM_MENUSHOW, L"Hide");
	InsertMenuW(handle_, 0, MF_STRING | MF_CHECKED, WM_MENUSTARTUP, L"Startup");
	InsertMenuW(handle_, 0, MF_STRING, WM_MENUCLEAN, L"Clean");
	InsertMenuW(handle_, 0, MF_STRING, WM_MENUEXIT, L"Exit");
}

VOID Menu::Item_Clean() {
	Registry::Clean();
}

VOID Menu::Item_Exit() {
	NotifyIcon::Delete();
	EndDialog(dialog_main, 0);
}

VOID Menu::Item_Show() {
	if (show_ == TRUE) {
		ShowWindow(dialog_main, SW_HIDE);
	}
	else if (show_ == FALSE) {
		ShowWindow(dialog_main, SW_SHOW);
	}
}

VOID Menu::Item_Startup() {
	if (startup_ == TRUE) {
		Registry::Startup_Delete();

	}
	else if (startup_ == FALSE) {
		Registry::Startup_Create();
	}
}
