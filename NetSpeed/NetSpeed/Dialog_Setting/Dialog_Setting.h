#pragma once
#include <WS2tcpip.h>
class Dialog_Setting;
#include "../Dialog/Dialog.h"

class Dialog_Setting :public Dialog<Dialog_Setting> {
public:
	static INT_PTR WINAPI	Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG);
	static VOID				Refresh_All();
	static VOID				Refresh_Button_Clean();
	static VOID				Refresh_Button_CleanAndExit();
	static VOID				Refresh_Caption();
	static VOID				Refresh_Check_Setting_Show();
	static VOID				Refresh_Check_Setting_Startup();
	static VOID				Refresh_Combo_Setting_Language();
	static VOID				Refresh_Slider_Transparency();
	static VOID				Refresh_Static_Language();
	static VOID				Refresh_Static_Transparency();
private:
};