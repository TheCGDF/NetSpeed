#include "Registry.h"
#include "../Dialog_Main/Dialog_Main.h"
#include "../Dialog_Setting/Dialog_Setting.h"
#include "../Main.h"
#include "../Text/Text.h"

const std::wstring	Registry::Path_NetSpeed_ = L"Software\\NetSpeed";
const std::wstring	Registry::Path_Startup_ = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const std::wstring	Registry::Value_Position_X_ = L"Position_X";
const std::wstring	Registry::Value_Position_Y_ = L"Position_Y";
const std::wstring	Registry::Value_Transparency_ = L"Transparency";
const std::wstring	Registry::Value_Language_ = L"Language";

//public:

VOID Registry::Clean() {
	Startup_Set(FALSE);
	RegDeleteTreeW(HKEY_CURRENT_USER, Path_NetSpeed_.c_str());
	MessageBoxW(NULL, Text::Message_CleanFinish().c_str(), L"", MB_OK);
}

std::wstring Registry::Language_Get() {
	WCHAR Language[100];
	LSTATUS Status_Language = Value_Get(Value_Language_, RRF_RT_REG_SZ, Language);
	if (Status_Language == ERROR_SUCCESS) {
		return Language;
	}
	std::wstring Language_Name = Text::Name();
	Value_Set(Value_Language_.c_str(), REG_SZ, (PBYTE)Language_Name.c_str(), Language_Name.length() * sizeof(WCHAR));
	return Language_Name;
}

VOID Registry::Language_Save() {
	std::wstring Language_Name = Text::Name();
	Value_Set(Value_Language_.c_str(), REG_SZ, (PBYTE)Language_Name.c_str(), Language_Name.length() * sizeof(WCHAR));
}

POINT Registry::Position_Get() {
	POINT Poisition;
	LSTATUS Status_Position_X = Value_Get(Value_Position_X_, RRF_RT_REG_DWORD, &Poisition.x);
	LSTATUS Status_Position_Y = Value_Get(Value_Position_Y_, RRF_RT_REG_DWORD, &Poisition.y);
	if (Status_Position_X != ERROR_SUCCESS || Status_Position_Y != ERROR_SUCCESS) {
		Position_Save();
		return Position_Get();
	}
	return Poisition;
}

VOID Registry::Position_Save() {
	POINT Point_Dialog = Dialog_Main::Position_Get();
	Value_Set(Value_Position_X_.c_str(), REG_DWORD, (PBYTE)&Point_Dialog.x, sizeof(LONG));
	Value_Set(Value_Position_Y_.c_str(), REG_DWORD, (PBYTE)&Point_Dialog.y, sizeof(LONG));
}

BOOL Registry::Startup_Get() {
	LSTATUS Status_Registry_Startup = Value_Get(File_Name, RRF_RT_REG_SZ, NULL);
	if (Status_Registry_Startup != ERROR_SUCCESS) {
		return FALSE;
	}
	//Rest the .exe path
	Value_Set(File_Name, REG_SZ, (PBYTE)File_Path.c_str(), File_Path.length() * sizeof(WCHAR));
	return TRUE;
}

VOID Registry::Startup_Set(BOOL Startup) {
	if (Startup == TRUE) {
		Value_Set(File_Name, REG_SZ, (PBYTE)File_Path.c_str(), File_Path.length() * sizeof(WCHAR));
	}
	else {
		RegDeleteKeyValueW(HKEY_CURRENT_USER, Path_Startup_.c_str(), File_Name.c_str());
	}
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Refresh_Check_Setting_Startup();
	}
}

INT Registry::Transparency_Get() {
	INT Transparency;
	LSTATUS Status_Transparency = Value_Get(Value_Transparency_, RRF_RT_REG_DWORD, &Transparency);
	if (Status_Transparency != ERROR_SUCCESS) {
		Transparency_Save();
		return Transparency_Get();
	}
	return Transparency;
}

VOID Registry::Transparency_Save() {
	INT Transparency = Dialog_Main::Transparency_Get();
	Value_Set(Value_Transparency_, REG_DWORD, (PBYTE)&Transparency, sizeof(LONG));
}

//private:

LSTATUS Registry::Value_Get(std::wstring Value, DWORD Flag, PVOID Data) {
	std::wstring Path = Path_NetSpeed_;
	if (Value == File_Name) {
		Path = Path_Startup_;
	}
	DWORD Size_Data;
	LSTATUS Status = RegGetValueW(
		HKEY_CURRENT_USER,
		Path.c_str(),
		Value.c_str(),
		Flag,
		NULL,
		Data,
		&Size_Data
	);
	return Status;
}

VOID Registry::Value_Set(std::wstring Value, DWORD Type, PBYTE Data, DWORD Size) {
	HKEY Handle_Key;
	std::wstring Path = Path_NetSpeed_;
	if (Value == File_Name) {
		Path = Path_Startup_;
	}
	RegCreateKeyExW(
		HKEY_CURRENT_USER,
		Path.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&Handle_Key,
		NULL
	);
	RegSetValueExW(Handle_Key, Value.c_str(), 0, Type, Data, Size);
	RegCloseKey(Handle_Key);
}
