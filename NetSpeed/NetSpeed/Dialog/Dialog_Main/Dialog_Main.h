#pragma once
#include <WS2tcpip.h>
class Dialog_Main;
#include "../../Dialog/Dialog.h"

class Dialog_Main :public Dialog<Dialog_Main> {
public:
	static INT_PTR WINAPI	Process(HWND Handle_Dialog, UINT DialogMessage, WPARAM ParamWORD, LPARAM ParamLONG);
public:
	static VOID		ColorBackground_Set(COLORREF Background_Color);
	static VOID		ColorText_Set(COLORREF Text_Color);
	static POINT	Position_Get();
	static VOID		Position_Set(POINT Position);
	static BOOL		Show_Get();
	static VOID		Show_Set(BOOL Show);
	static VOID		Size_Set(INT Size);
	static VOID		Topmost_Check();
	static VOID		Transparency_Set(INT Transparency);
private:
	static COLORREF	Color_Background_;
	static COLORREF	Color_Text_;
	static LONG		DialogOriginHeight_;
	static LONG		DialogOriginWidth_;
	static HBRUSH	Handle_Bursh_Background_;
	static HBRUSH	Handle_Bursh_Text_;
	static HFONT	Handle_Font_Text_;
	static HPEN		Handle_Pen_Background_;
	static LONG		TextOriginHeight_;
	static LONG		TextOriginWidth_;
};
