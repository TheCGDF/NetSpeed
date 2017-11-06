#pragma once
#include <WS2tcpip.h>
class Dialog_Main;
#include "../Dialog/Dialog.h"

class Dialog_Main :public Dialog<Dialog_Main> {
public:
	static INT_PTR WINAPI	Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG);
	static POINT			Position_Get();
	static VOID				Position_Set(POINT Position);
	static BOOL				Show_Get();
	static VOID				Show_Set(BOOL Show);
	static INT				Transparency_Get();
	static VOID				Transparency_Set(INT Transparency);
private:
	static HBRUSH	Handle_Color_Background_;
	static BOOL		Show_;
};



