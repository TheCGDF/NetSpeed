#pragma once
#include <WS2tcpip.h>
#include <string>
class Dialog_Setting;
#include "../../Dialog/Dialog.h"

class Dialog_Setting :public Dialog<Dialog_Setting> {
public:
	static INT_PTR WINAPI	Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG);
	static VOID				Color_Choose(COLORREF *Color_Bind);
public:
	static VOID	Gather_All();
	static VOID	Gather_Button_Clean();
	static VOID	Gather_Button_CleanAndExit();
	static VOID Gather_Button_ColorBackground();
	static VOID Gather_Button_ColorText();
	static VOID	Gather_Caption();
	static VOID	Gather_Check_Show();
	static VOID	Gather_Check_Startup();
	static VOID	Gather_Combo_Language();
	static VOID	Gather_Edit_Size();
	static VOID	Gather_Slider_Transparency();
	static VOID	Gather_Static_Language();
	static VOID	Gather_Static_Size();
	static VOID	Gather_Static_Transparency();
private:
	static std::wstring	Transparency_String(INT Transparency);
};
