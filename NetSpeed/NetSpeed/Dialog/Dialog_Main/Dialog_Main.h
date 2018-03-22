#pragma once
#include <WS2tcpip.h>
class Dialog_Main;
#include "../../Dialog/Dialog.h"

class Dialog_Main :public Dialog<Dialog_Main> {
public:
	static INT_PTR WINAPI	Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG);
public:
	static VOID		ColorBackground_Set(COLORREF Background_Color);
	static VOID		ColorText_Set(COLORREF Text_Color);
	static POINT	Position_Get();
	static VOID		Position_Set(POINT Position);
	static BOOL		Show_Get();
	static VOID		Show_Set(BOOL Show);
	static VOID		Size_Set(INT Size);
	static VOID		Transparency_Set(INT Transparency);
private:
	static HBRUSH	Bursh_Background_;
	static HBRUSH	Bursh_Text_;
	static COLORREF	Color_Background_;
	static COLORREF	Color_Text_;
	static HFONT	Font_Text_;
	static LONG		Origin_Height_Dialog_;
	static LONG		Origin_Height_Text_;
	static LONG		Origin_Width_Dialog_;
	static LONG		Origin_Width_Text_;
	static HPEN		Pen_Background_;
};
