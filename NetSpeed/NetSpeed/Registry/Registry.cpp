#include "Registry.h"

#include "../Dialog/Dialog_Main/Dialog_Main.h"
#include "../Dialog/Dialog_Setting/Dialog_Setting.h"
#include "../Main.h"
#include "../Text/Text.h"

const std::wstring	Registry::Path_NetSpeed_ = L"Software\\NetSpeed";
const std::wstring	Registry::Path_Network_ = L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}";
const std::wstring	Registry::Path_Startup_ = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const std::wstring	Registry::Value_ColorBackground_ = L"ColorBackground";
const std::wstring	Registry::Value_ColorCustom_[16] = {
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
const std::wstring	Registry::Value_ColorText_ = L"ColorText";
const std::wstring	Registry::Value_Language_ = L"Language";
const std::wstring	Registry::Value_PositionX_ = L"Position_X";
const std::wstring	Registry::Value_PositionY_ = L"Position_Y";
const std::wstring	Registry::Value_Size_ = L"Size";
const std::wstring	Registry::Value_Transparency_ = L"Transparency";

//public:

VOID Registry::Clean() {
	std::wstring CleanFinishMessage = Text::Message_CleanFinish();
	Registry::Startup_Set(FALSE);
	Registry::Language_Set(0);
	Dialog_Setting::GatherAll();
	RegDeleteTreeW(HKEY_CURRENT_USER, Path_NetSpeed_.c_str());
	MessageBoxW(NULL, CleanFinishMessage.c_str(), L"", MB_OK);
}

//public:

COLORREF Registry::ColorBackground_Get() {
	COLORREF ColorBackground;
	LSTATUS ColorBackgroundStatus = Registry::Value_Get(Path_NetSpeed_, Value_ColorBackground_, RRF_RT_REG_DWORD, &ColorBackground);
	if (ColorBackgroundStatus != ERROR_SUCCESS) {
		ColorBackground = RGB(0, 0, 0);
		Registry::ColorBackground_Set(ColorBackground);
	}
	return ColorBackground;
}

VOID Registry::ColorBackground_Set(COLORREF ColorBackground) {
	Value_Set(Path_NetSpeed_, Value_ColorBackground_, REG_DWORD, (PBYTE)&ColorBackground, sizeof(COLORREF));
}

COLORREF Registry::ColorCustom_Get(INT ColorCustomIndex) {
	COLORREF ColorCustom;
	LSTATUS ColorCustomStatus = Registry::Value_Get(Path_NetSpeed_, Value_ColorCustom_[ColorCustomIndex], RRF_RT_REG_DWORD, &ColorCustom);
	if (ColorCustomStatus != ERROR_SUCCESS) {
		ColorCustom = RGB(255, 255, 255);
		Registry::ColorCustom_Set(ColorCustomIndex, ColorCustom);
	}
	return ColorCustom;
}

VOID Registry::ColorCustom_Set(INT ColorCustomIndex, COLORREF ColorCustom) {
	Registry::Value_Set(Path_NetSpeed_, Value_ColorCustom_[ColorCustomIndex], REG_DWORD, (PBYTE)&ColorCustom, sizeof(COLORREF));
}


COLORREF Registry::ColorText_Get() {
	COLORREF ColorText;
	LSTATUS ColorTextStatus = Registry::Value_Get(Path_NetSpeed_, Value_ColorText_, RRF_RT_REG_DWORD, &ColorText);
	if (ColorTextStatus != ERROR_SUCCESS) {
		ColorText = RGB(255, 255, 255);
		Registry::ColorText_Set(ColorText);
	}
	return ColorText;
}

VOID Registry::ColorText_Set(COLORREF ColorText) {
	Registry::Value_Set(Path_NetSpeed_, Value_ColorText_, REG_DWORD, (PBYTE)&ColorText, sizeof(COLORREF));
}

INT Registry::Language_Get() {
	WCHAR Language[100];
	LSTATUS LanguageStatus = Registry::Value_Get(Path_NetSpeed_, Value_Language_, RRF_RT_REG_SZ, Language);
	if (LanguageStatus == ERROR_SUCCESS) {
		INT Language_Exist = Text::Language_Get(Language);
		if (Language_Exist != -1) {
			return Language_Exist;
		}
	}
	std::wstring LanguageName = Text::Name(0);
	Registry::Value_Set(Path_NetSpeed_, Value_Language_.c_str(), REG_SZ, (PBYTE)LanguageName.c_str(), LanguageName.length() * sizeof(WCHAR));
	return 0;
}

VOID Registry::Language_Set(INT LanguageIndex) {
	std::wstring LanguageName = Text::Name(LanguageIndex);
	Registry::Value_Set(Path_NetSpeed_, Value_Language_.c_str(), REG_SZ, (PBYTE)LanguageName.c_str(), LanguageName.length() * sizeof(WCHAR));
}

std::set<std::wstring> Registry::PCI_Get() {
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
	INT PathIndex = 0;
	WCHAR PathName[MAX_PATH];
	DWORD Name_Size = MAX_PATH;
	while (RegEnumKeyExW(Key_Network, PathIndex, PathName, &Name_Size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
		if (PathName[0] == L'{') {
			std::wstring Path_New = Path_Network_ + L"\\" + PathName + L"\\Connection";
			WCHAR Value[MAX_PATH];
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

POINT Registry::Position_Get() {
	POINT Poisition;
	LSTATUS PositionXStatus = Registry::Value_Get(Path_NetSpeed_, Value_PositionX_, RRF_RT_REG_DWORD, &Poisition.x);
	LSTATUS PositionYStatus = Registry::Value_Get(Path_NetSpeed_, Value_PositionY_, RRF_RT_REG_DWORD, &Poisition.y);
	if (PositionXStatus != ERROR_SUCCESS || PositionYStatus != ERROR_SUCCESS) {
		Poisition = Dialog_Main::Position_Get();
		Registry::Position_Set(Poisition);
	}
	return Poisition;
}

VOID Registry::Position_Set(POINT Position) {
	Registry::Value_Set(Path_NetSpeed_, Value_PositionX_.c_str(), REG_DWORD, (PBYTE)&Position.x, sizeof(LONG));
	Registry::Value_Set(Path_NetSpeed_, Value_PositionY_.c_str(), REG_DWORD, (PBYTE)&Position.y, sizeof(LONG));
}

INT Registry::Size_Get() {
	INT Size;
	LSTATUS SizeStatus = Registry::Value_Get(Path_NetSpeed_, Value_Size_, RRF_RT_REG_DWORD, &Size);
	if (SizeStatus != ERROR_SUCCESS) {
		Size = 12;
		Registry::Size_Set(Size);
	}
	return Size;
}

VOID Registry::Size_Set(INT Size) {
	Registry::Value_Set(Path_NetSpeed_, Value_Size_, REG_DWORD, (PBYTE)&Size, sizeof(INT));
}

BOOL Registry::Startup_Get() {
	LSTATUS StartupStatus = Registry::Value_Get(Path_Startup_, FileName, RRF_RT_REG_SZ, NULL);
	if (StartupStatus != ERROR_SUCCESS) {
		return FALSE;
	}
	//Rest the .exe path
	Registry::Value_Set(Path_Startup_, FileName, REG_SZ, (PBYTE)FilePath.c_str(), FilePath.length() * sizeof(WCHAR));
	return TRUE;
}

VOID Registry::Startup_Set(BOOL Startup) {
	if (Startup == TRUE) {
		Registry::Value_Set(Path_Startup_, FileName, REG_SZ, (PBYTE)FilePath.c_str(), FilePath.length() * sizeof(WCHAR));
	}
	else {
		RegDeleteKeyValueW(HKEY_CURRENT_USER, Path_Startup_.c_str(), FileName.c_str());
	}
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Gather_Check_Startup();
	}
}

INT Registry::Transparency_Get() {
	INT Transparency;
	LSTATUS TransparencyStatus = Registry::Value_Get(Path_NetSpeed_, Value_Transparency_, RRF_RT_REG_DWORD, &Transparency);
	if (TransparencyStatus != ERROR_SUCCESS) {
		Transparency = 65;
		Registry::Transparency_Set(Transparency);
	}
	return Transparency;
}

VOID Registry::Transparency_Set(INT Transparency) {
	Registry::Value_Set(Path_NetSpeed_, Value_Transparency_, REG_DWORD, (PBYTE)&Transparency, sizeof(INT));
}

//private:

LSTATUS Registry::Value_Get(std::wstring Path, std::wstring Value, DWORD Flag, PVOID Data) {
	DWORD Size_Data = MAX_PATH;
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

VOID Registry::Value_Set(std::wstring Path, std::wstring Value, DWORD Type, PBYTE Data, DWORD Size) {
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
