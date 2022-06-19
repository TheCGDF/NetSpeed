#pragma once
#include <WS2tcpip.h>
class Dialog_Main;
#include "../../Dialog/Dialog.h"

class Dialog_Main :public Dialog<Dialog_Main> {
public:
    static INT_PTR WINAPI Process(HWND Handle_Dialog, UINT DialogMessage, WPARAM ParamWORD, LPARAM ParamLONG);

public:
    static void ColorBackground_Set(COLORREF Background_Color);
    static void ColorText_Set(COLORREF Text_Color);
    static POINT Position_Get();
    static void Position_Set(POINT Position);
    static bool Show_Get();
    static void Show_Set(bool Show);
    static void Size_Set(int Size);
    static void Transparency_Set(int Transparency);

private:
    static COLORREF Color_Background_;
    static COLORREF Color_Text_;
    static long DialogOriginHeight_;
    static long DialogOriginWidth_;
    static HBRUSH Handle_Bursh_Background_;
    static HBRUSH Handle_Bursh_Text_;
    static HFONT Handle_Font_Text_;
    static HPEN Handle_Pen_Background_;
    static long TextOriginHeight_;
    static long TextOriginWidth_;
};
