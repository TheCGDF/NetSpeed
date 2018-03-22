#include "Dialog_Setting.h"
#include <CommCtrl.h>
#include <windowsx.h>

#include "../../Registry/Registry.h"
#include "../../Resource/resource.h"
#include "../../Text/Text.h"
#include "../Dialog_Main/Dialog_Main.h"

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
				case ID_Button_BackgroundColor:
				{
					COLORREF Color_Background = Registry::ColorBackground_Get();
					Color_Choose(&Color_Background);
					Registry::ColorBackground_Set(Color_Background);
					Dialog_Main::ColorBackground_Set(Color_Background);
					break;
				}
				case ID_Button_Clean:
				{
					Registry::Clean();
					break;
				}
				case ID_Button_CleanAndExit:
				{
					Registry::Clean();
					Destroy();
					DestroyWindow(Handle_Dialog);
					break;
				}
				case ID_Button_TextColor:
				{
					COLORREF Color_Text = Registry::ColorText_Get();
					Dialog_Setting::Color_Choose(&Color_Text);
					Registry::ColorText_Set(Color_Text);
					Dialog_Main::ColorText_Set(Color_Text);
					break;
				}
				case ID_Combo_Language:
				{
					switch (HIWORD(Param_WORD)) {
						case CBN_SELCHANGE:
						{
							HWND Handle_Combo_Language = GetDlgItem(Handle_Dialog, ID_Combo_Language);
							INT Languae_Index = ComboBox_GetCurSel(Handle_Combo_Language);
							Registry::Language_Set(Languae_Index);
							Dialog_Setting::Gather_All();
							break;
						}
						default:
						{
							Result_Process = FALSE;
						}
					}
					break;
				}
				case ID_Check_Show:
				{
					HWND Handle_Check_Show = GetDlgItem(Handle_Dialog, ID_Check_Show);
					BOOL Show = Button_GetCheck(Handle_Check_Show);
					Dialog_Main::Show_Set(Show);
					break;
				}
				case ID_Check_Startup:
				{
					HWND Handle_Check_Startup = GetDlgItem(Handle_Dialog, ID_Check_Startup);
					BOOL Startup = Button_GetCheck(Handle_Check_Startup);
					Registry::Startup_Set(Startup);
					break;
				}
				case ID_Edit_Size:
				{
					switch (HIWORD(Param_WORD)) {
						case EN_CHANGE:
						{
							WCHAR Size_Text[4];
							GetDlgItemTextW(Handle_Dialog, ID_Edit_Size, Size_Text, 4);
							INT Size = _wtoi(Size_Text);
							if (Size == 0) {
								break;
							}
							if (Size > 128) {
								SetDlgItemTextW(Handle_Dialog, ID_Edit_Size, L"128");
								Size = 128;
							}
							Registry::Size_Set(Size);
							Dialog_Main::Size_Set(Size);
							break;
						}
						default:
						{
							Result_Process = FALSE;
						}
					}
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
						std::wstring Transparency_wstring = Dialog_Setting::Transparency_String(Position_Slider);
						SetDlgItemTextW(Handle_Dialog, ID_Static_Transparency, Transparency_wstring.c_str());
						break;
					}
					case TB_THUMBPOSITION:
					{
						Registry::Transparency_Set(Position_Slider);
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
			Dialog_Setting::Gather_All();
			break;
		}
		case WM_NCDESTROY:
		{
			Dialog_Setting::Handle_Set(NULL);
			return TRUE;
		}
		default:
		{
			Result_Process = FALSE;
		}
	}
	return Result_Process;
}

//public:

VOID Dialog_Setting::Color_Choose(COLORREF *Color_Bind) {
	CHOOSECOLOR Color_Choose;
	COLORREF Color_Custom[16];
	for (INT ColorCustom_Get_Index = 0; ColorCustom_Get_Index <= 15; ColorCustom_Get_Index++) {
		Color_Custom[ColorCustom_Get_Index] = Registry::ColorCustom_Get(ColorCustom_Get_Index);
	}
	Color_Choose.lStructSize = sizeof(CHOOSECOLOR);
	Color_Choose.hwndOwner = Dialog_Setting::Handle_Get();
	Color_Choose.hInstance = NULL;
	Color_Choose.rgbResult = *Color_Bind;
	Color_Choose.lpCustColors = Color_Custom;
	Color_Choose.Flags = CC_FULLOPEN | CC_RGBINIT;
	Color_Choose.lCustData = 0;
	Color_Choose.lpfnHook = NULL;
	Color_Choose.lpTemplateName = NULL;
	ChooseColor(&Color_Choose);
	*Color_Bind = Color_Choose.rgbResult;
	for (INT ColorCustom_Set_Index = 0; ColorCustom_Set_Index <= 15; ColorCustom_Set_Index++) {
		Registry::ColorCustom_Set(ColorCustom_Set_Index, Color_Custom[ColorCustom_Set_Index]);
	}
}

VOID Dialog_Setting::Gather_All() {
	Dialog_Setting::Gather_Button_Clean();
	Dialog_Setting::Gather_Button_CleanAndExit();
	Dialog_Setting::Gather_Caption();
	Dialog_Setting::Gather_Check_Show();
	Dialog_Setting::Gather_Check_Startup();
	Dialog_Setting::Gather_Combo_Language();
	Dialog_Setting::Gather_Edit_Size();
	Dialog_Setting::Gather_Slider_Transparency();
	Dialog_Setting::Gather_Static_Language();
	Dialog_Setting::Gather_Static_Transparency();
}

VOID Dialog_Setting::Gather_Button_Clean() {
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_Clean, Text::Button_Clean().c_str());
}

VOID Dialog_Setting::Gather_Button_CleanAndExit() {
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Button_CleanAndExit, Text::Button_CleanAndExit().c_str());
}

VOID Dialog_Setting::Gather_Caption() {
	SetWindowTextW(Dialog_Setting::Handle_Get(), Text::Dialog_Caption().c_str());
}

VOID Dialog_Setting::Gather_Check_Show() {
	BOOL Show = Dialog_Main::Show_Get();
	HWND Handle_Dialog = Dialog_Setting::Handle_Get();
	HWND Handle_Check_Show = GetDlgItem(Handle_Dialog, ID_Check_Show);
	Button_SetCheck(Handle_Check_Show, Show);
	SetDlgItemTextW(Handle_Dialog, ID_Check_Show, Text::Menu_Item_Show().c_str());
}

VOID Dialog_Setting::Gather_Check_Startup() {
	BOOL Startup = Registry::Startup_Get();
	HWND Handle_Dialog = Dialog_Setting::Handle_Get();
	HWND Handle_Check_Startup = GetDlgItem(Handle_Dialog, ID_Check_Startup);
	Button_SetCheck(Handle_Check_Startup, Startup);
	SetDlgItemTextW(Handle_Dialog, ID_Check_Startup, Text::Menu_Item_Startup().c_str());
}

VOID Dialog_Setting::Gather_Combo_Language() {
	HWND Handle_Combo_Language = GetDlgItem(Dialog_Setting::Handle_Get(), ID_Combo_Language);
	ComboBox_ResetContent(Handle_Combo_Language);
	for (INT Index = 0; Index <= Text::Language::Total - 1; Index++) {
		ComboBox_AddString(Handle_Combo_Language, Text::Name(Index).c_str());
	}
	ComboBox_SetCurSel(Handle_Combo_Language, Registry::Language_Get());
}

VOID Dialog_Setting::Gather_Edit_Size() {
	INT Size = Registry::Size_Get();
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Edit_Size, std::to_wstring(Size).c_str());
}

VOID Dialog_Setting::Gather_Slider_Transparency() {
	HWND Handle_Slider_Transparency = GetDlgItem(Dialog_Setting::Handle_Get(), ID_Slider_Transparency);
	INT Transparency = Registry::Transparency_Get();
	SendMessageW(Handle_Slider_Transparency, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)Transparency);
}

VOID Dialog_Setting::Gather_Static_Language() {
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Language, Text::Static_Language().c_str());
}

VOID Dialog_Setting::Gather_Static_Size() {
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Size, Text::Static_Size().c_str());
}

VOID Dialog_Setting::Gather_Static_Transparency() {
	INT Transparency = Registry::Transparency_Get();
	std::wstring Transparency_wstring = Dialog_Setting::Transparency_String(Transparency);
	SetDlgItemTextW(Dialog_Setting::Handle_Get(), ID_Static_Transparency, Transparency_wstring.c_str());
}

//private:

std::wstring Dialog_Setting::Transparency_String(INT Transparency) {
	return Text::Static_Transparency() + std::to_wstring(Transparency) + L"%";
}
