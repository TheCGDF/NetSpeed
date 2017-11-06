#include "Dialog_Setting.h"
#include <CommCtrl.h>
#include <windowsx.h>
#include "../Dialog_Main/Dialog_Main.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

//public:

INT_PTR Dialog_Setting::Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG) {
	Handle_Set(Handle_Dialog);
	BOOL Result_Process = TRUE;
	switch (Message_Dialog) {
		case WM_CLOSE:
		{
			DestroyWindow(Handle_Dialog);
			return 0;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(Param_WORD)) {
				case ID_Button_Setting_Clean:
				{
					Registry::Clean();
					break;
				}
				case ID_Button_Setting_CleanAndExit:
				{
					Registry::Clean();
					Destroy();
					DestroyWindow(Handle_Dialog);
					break;
				}
				case ID_Combo_Setting_Language:
				{
					switch (HIWORD(Param_WORD)) {
						case CBN_SELCHANGE:
						{
							HWND Handle_Combo_Language = GetDlgItem(Handle_Get(), ID_Combo_Setting_Language);
							INT Languae = ComboBox_GetCurSel(Handle_Combo_Language);
							Text::Language_Set(Languae);
							Refresh_All();
							break;
						}
						default:
						{
							Result_Process = FALSE;
						}
					}
					break;
				}
				case ID_Check_Setting_Show:
				{
					HWND Handle_Check_Show = GetDlgItem(Handle_Get(), ID_Check_Setting_Show);
					BOOL Show = Button_GetCheck(Handle_Check_Show);
					Dialog_Main::Show_Set(Show);
					break;
				}
				case ID_Check_Setting_Startup:
				{
					HWND Handle_Check_Startup = GetDlgItem(Handle_Get(), ID_Check_Setting_Startup);
					BOOL Startup = Button_GetCheck(Handle_Check_Startup);
					Registry::Startup_Set(Startup);
					break;
				}
				default:
				{
					Result_Process = FALSE;
				}
			}
			break;
		}
		case WM_HSCROLL:
		{
			HWND Handle_Slider_Transparency = GetDlgItem(Handle_Dialog, ID_Slider_Transparency);
			if (Param_LONG == (LPARAM)Handle_Slider_Transparency) {
				INT Position_Slider = HIWORD(Param_WORD);
				switch (LOWORD(Param_WORD)) {
					case TB_THUMBTRACK:
					{
						Dialog_Main::Transparency_Set(Position_Slider);
						Refresh_Static_Transparency();
						break;
					}
					case TB_THUMBPOSITION:
					{
						Registry::Transparency_Save();
						break;
					}
					default:
					{
						Result_Process = FALSE;
					}
				}
			}
			break;
		}
		case WM_INITDIALOG:
		{
			Refresh_All();
			break;
		}
		case WM_NCDESTROY:
		{
			Handle_Set(NULL);
			return TRUE;
		}
		default:
		{
			Result_Process = FALSE;
		}
	}
	return Result_Process;
}

VOID Dialog_Setting::Refresh_All() {
	Refresh_Button_Clean();
	Refresh_Button_CleanAndExit();
	Refresh_Caption();
	Refresh_Check_Setting_Show();
	Refresh_Check_Setting_Startup();
	Refresh_Combo_Setting_Language();
	Refresh_Slider_Transparency();
	Refresh_Static_Language();
	Refresh_Static_Transparency();
}

VOID Dialog_Setting::Refresh_Button_Clean() {
	SetDlgItemTextW(Handle_Get(), ID_Button_Setting_Clean, Text::Button_Setting_Clean().c_str());
}

VOID Dialog_Setting::Refresh_Button_CleanAndExit() {
	SetDlgItemTextW(Handle_Get(), ID_Button_Setting_CleanAndExit, Text::Button_Setting_CleanAndExit().c_str());
}

VOID Dialog_Setting::Refresh_Caption() {
	SetWindowTextW(Handle_Get(), Text::Dialog_Setting_Caption().c_str());
}

VOID Dialog_Setting::Refresh_Check_Setting_Show() {
	BOOL Show = Dialog_Main::Show_Get();
	HWND Handle_Check_Show = GetDlgItem(Handle_Get(), ID_Check_Setting_Show);
	Button_SetCheck(Handle_Check_Show, Show);
	SetDlgItemTextW(Handle_Get(), ID_Check_Setting_Show, Text::Menu_Item_Show().c_str());
}

VOID Dialog_Setting::Refresh_Check_Setting_Startup() {
	BOOL Startup = Registry::Startup_Get();
	HWND Handle_Check_Startup = GetDlgItem(Handle_Get(), ID_Check_Setting_Startup);
	Button_SetCheck(Handle_Check_Startup, Startup);
	SetDlgItemTextW(Handle_Get(), ID_Check_Setting_Startup, Text::Menu_Item_Startup().c_str());
}

VOID Dialog_Setting::Refresh_Combo_Setting_Language() {
	HWND Handle_Combo_Language = GetDlgItem(Handle_Get(), ID_Combo_Setting_Language);
	ComboBox_ResetContent(Handle_Combo_Language);
	for (INT Index = 0; Index <= Text::Language::Total - 1; Index++) {
		ComboBox_AddString(Handle_Combo_Language, Text::Name(Index).c_str());
	}
	ComboBox_SetCurSel(Handle_Combo_Language, Text::Language_Get());
}

VOID Dialog_Setting::Refresh_Slider_Transparency() {
	HWND Handle_Slider_Transparency = GetDlgItem(Handle_Get(), ID_Slider_Transparency);
	INT Transparency = Registry::Transparency_Get();
	SendMessageW(Handle_Slider_Transparency, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)Transparency);
}

VOID Dialog_Setting::Refresh_Static_Language() {
	SetDlgItemTextW(Handle_Get(), ID_Static_Language, Text::Static_Setting_Language().c_str());
}

VOID Dialog_Setting::Refresh_Static_Transparency() {
	INT Transparency = Dialog_Main::Transparency_Get();
	std::wstring Transparency_wstring = Text::Static_Setting_Transparency() + std::to_wstring(Transparency) + L"%";
	SetDlgItemTextW(Handle_Get(), ID_Static_Transparency, Transparency_wstring.c_str());
}
