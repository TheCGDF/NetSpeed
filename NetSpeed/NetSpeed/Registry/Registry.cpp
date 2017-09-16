#include "Registry.h"
#include "REGISTRYPATH.h"
#include "REGISTRYVALUE.h"
#include "../Main.h"
#include "../DialogMain/DialogMain.h"

VOID Registry::Clean() {
	Startup_Delete();
	Position_Delete();
	MessageBoxW(NULL, L"Startup and Position have been cleanned.", L"Complete", MB_OK);
}

VOID Registry::Position_Create() {
	HKEY registry_key;
	RegCreateKeyExW(
		HKEY_CURRENT_USER,
		REGISTRYPATH_POSITION,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&registry_key,
		NULL
	);
	RECT dialog_rect;
	GetWindowRect(dialog_main, &dialog_rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(dialog_main), (LPPOINT)&dialog_rect, sizeof(RECT) / sizeof(POINT));
	RegSetValueExW(
		registry_key,
		REGISTRYVALUE_POSITION_X,
		0,
		REG_DWORD,
		(PBYTE)&dialog_rect.left,
		sizeof(LONG)
	);
	RegSetValueExW(
		registry_key,
		REGISTRYVALUE_POSITION_Y,
		0,
		REG_DWORD,
		(PBYTE)&dialog_rect.top,
		sizeof(LONG)
	);
	RegCloseKey(registry_key);
}

VOID Registry::Position_Delete() {
	RegDeleteTreeW(HKEY_CURRENT_USER, REGISTRYPATH_POSITION);
}

POINT Registry::Position_Get() {
	POINT poisition = {-1, -1};
	DWORD data_size;
	RegGetValueW(
		HKEY_CURRENT_USER,
		REGISTRYPATH_POSITION,
		REGISTRYVALUE_POSITION_X,
		RRF_RT_REG_DWORD,
		NULL,
		&poisition.x,
		&data_size
	);
	RegGetValueW(
		HKEY_CURRENT_USER,
		REGISTRYPATH_POSITION,
		REGISTRYVALUE_POSITION_Y,
		RRF_RT_REG_DWORD,
		NULL,
		&poisition.y,
		&data_size
	);
	return poisition;
}

VOID Registry::Startup_Create() {
	HKEY registry_key;
	RegCreateKeyExW(
		HKEY_CURRENT_USER,
		REGISTRYPATH_STARTUP,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&registry_key,
		NULL
	);
	RegSetValueExW(
		registry_key,
		file_name.c_str(),
		0,
		REG_SZ,
		(PBYTE)file_path.c_str(),
		file_path.length() * sizeof(WCHAR)
	);
	RegCloseKey(registry_key);
}

VOID Registry::Startup_Delete() {
	RegDeleteKeyValueW(HKEY_CURRENT_USER, REGISTRYPATH_STARTUP, file_name.c_str());
}

LSTATUS Registry::Startup_Status() {
	LSTATUS registry_startup = RegGetValueW(
		HKEY_CURRENT_USER,
		REGISTRYPATH_STARTUP,
		file_name.c_str(),
		RRF_RT_REG_SZ,
		NULL,
		NULL,
		NULL
	);
	if (registry_startup == ERROR_SUCCESS) {
		Startup_Create();
	}
	return registry_startup;
}
