#include "Dialog_Main.h"

#include "../../Menu/Menu.h"
#include "../../Timer_Refresh/Timer_Refresh.h"
#include "../../NotifyIcon/NotifyIcon.h"
#include "../../Registry/Registry.h"
#include "../../Resource/resource.h"
#include "../Dialog_Setting/Dialog_Setting.h"

HBRUSH		Dialog_Main::Bursh_Background_ = NULL;
HBRUSH		Dialog_Main::Bursh_Text_ = NULL;
COLORREF	Dialog_Main::Color_Background_;
COLORREF	Dialog_Main::Color_Text_;
HFONT		Dialog_Main::Font_Text_ = NULL;
LONG		Dialog_Main::Origin_Height_Dialog_;
LONG		Dialog_Main::Origin_Height_Text_;
LONG		Dialog_Main::Origin_Width_Dialog_;
LONG		Dialog_Main::Origin_Width_Text_;
HPEN		Dialog_Main::Pen_Background_ = NULL;

//public:

INT_PTR Dialog_Main::Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG) {
	Dialog_Main::Handle_Set(Handle_Dialog);
	BOOL Result_Process = TRUE;
	switch (Message_Dialog) {
		case WM_COMMAND:
		{
			switch (LOWORD(Param_WORD)) {
				case WM_MENU_EXIT:
				{
					Menu::Item_Exit();
					break;
				}
				case WM_MENU_SETTING:
				{
					Menu::Item_Setting();
					break;
				}
				case WM_MENU_SHOW:
				{
					Menu::Item_Show();
					break;
				}
				case WM_MENU_STARTUP:
				{
					Menu::Item_Startup();
					break;
				}
				default:
				{
					Result_Process = FALSE;
					break;
				}
			}
			break;
		}
		case WM_CTLCOLORDLG:
		{
			return (INT_PTR)Bursh_Background_;
		}
		case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)Param_WORD, TRANSPARENT);
			SetTextColor((HDC)Param_WORD, Color_Text_);
			return (INT_PTR)Bursh_Background_;
		}
		case WM_DESTROY:
		{
			NotifyIcon::Destroy();
			return 0;
		}
		case WM_ERASEBKGND:
		{
			RECT Rect_Dialog;
			SelectObject((HDC)Param_WORD, Pen_Background_);
			SelectObject((HDC)Param_WORD, Bursh_Background_);
			GetClientRect(Handle_Dialog, &Rect_Dialog);
			Rectangle((HDC)Param_WORD, Rect_Dialog.left, Rect_Dialog.top, Rect_Dialog.right, Rect_Dialog.bottom);
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			Registry::Position_Set(Position_Get());
			break;
		}
		case WM_INITDIALOG:
		{
			Color_Background_ = Registry::ColorBackground_Get();
			Color_Text_ = Registry::ColorText_Get();
			Dialog_Main::ColorBackground_Set(Color_Background_);

			POINT Position = Registry::Position_Get();
			Dialog_Main::Position_Set(Position);

			SetWindowLongW(
				Handle_Dialog,
				GWL_EXSTYLE,
				GetWindowLongW(Handle_Dialog, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW
			);

			INT Transparency = Registry::Transparency_Get();
			Dialog_Main::Transparency_Set(Transparency);

			RECT Rect_Dialog;
			GetWindowRect(Handle_Dialog, &Rect_Dialog);
			Origin_Height_Dialog_ = Rect_Dialog.bottom - Rect_Dialog.top;
			Origin_Width_Dialog_ = Rect_Dialog.right - Rect_Dialog.left;
			RECT Rect_Text;
			GetWindowRect(GetDlgItem(Handle_Dialog, ID_Static_Upload), &Rect_Text);
			Origin_Height_Text_ = Rect_Text.bottom - Rect_Text.top;
			Origin_Width_Text_ = Rect_Text.right - Rect_Text.left;

			INT Size = Registry::Size_Get();
			Dialog_Main::Size_Set(Size);

			Timer_Refresh::Init();
			Timer_Refresh::Start();
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SendMessageW(Handle_Dialog, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(LOWORD(Param_LONG), HIWORD(Param_LONG)));
			break;
		}
		case WM_NCPAINT:
		{
			NotifyIcon::Init();
			break;
		}
		case WM_NOTIFYICON:
		{
			switch (Param_LONG) {
				case WM_RBUTTONUP:
				{
					Menu::Pop();
					return TRUE;
				}
				default:
				{
					Result_Process = FALSE;
					break;
				}
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			Menu::Pop();
			break;
		}
		default:
		{
			Result_Process = FALSE;
			break;
		}
	}
	return Result_Process;
}

//public:

VOID Dialog_Main::ColorBackground_Set(COLORREF Background_Color) {
	Color_Background_ = Background_Color;
	if (Bursh_Background_ != NULL) {
		DeleteObject(Bursh_Background_);
	}
	if (Pen_Background_ != NULL) {
		DeleteObject(Pen_Background_);
	}
	Bursh_Background_ = CreateSolidBrush(Color_Background_);
	Pen_Background_ = CreatePen(PS_SOLID, 0, Color_Background_);
	InvalidateRect(Dialog_Main::Handle_Get(), NULL, TRUE);
}

VOID Dialog_Main::ColorText_Set(COLORREF Text_Color) {
	Color_Text_ = Text_Color;
	InvalidateRect(Dialog_Main::Handle_Get(), NULL, TRUE);
}

POINT Dialog_Main::Position_Get() {
	RECT Position_RECT;
	HWND Handle_Dialog = Dialog_Main::Handle_Get();
	GetWindowRect(Handle_Dialog, &Position_RECT);
	MapWindowPoints(HWND_DESKTOP, GetParent(Handle_Dialog), (LPPOINT)&Position_RECT, sizeof(RECT) / sizeof(POINT));
	POINT Position_POINT;
	Position_POINT.x = Position_RECT.left;
	Position_POINT.y = Position_RECT.top;
	return Position_POINT;
}

VOID Dialog_Main::Position_Set(POINT Position) {
	SetWindowPos(Dialog_Main::Handle_Get(), NULL, Position.x, Position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL Dialog_Main::Show_Get() {
	return IsWindowVisible(Dialog_Main::Handle_Get());
}

VOID Dialog_Main::Show_Set(BOOL Show) {
	HWND Handle_Dialog = Dialog_Main::Handle_Get();
	if (Show == TRUE) {
		ShowWindow(Handle_Dialog, SW_SHOW);
	}
	else {
		ShowWindow(Handle_Dialog, SW_HIDE);
	}
	if (Handle_Dialog != NULL) {
		Dialog_Setting::Gather_Check_Show();
	}
}

VOID Dialog_Main::Size_Set(INT Size) {
	LOGFONTW Font_Info;
	ZeroMemory(&Font_Info, sizeof(LOGFONTW));
	HWND Handle_Dialog = Dialog_Main::Handle_Get();
	HDC Handle_DC = GetDC(NULL);
	Font_Info.lfHeight = -MulDiv(Size, GetDeviceCaps(Handle_DC, LOGPIXELSY), 72);
	wcscpy_s(Font_Info.lfFaceName, LF_FACESIZE, L"Consolas");
	Font_Info.lfWeight = 400;
	if (Font_Text_ != NULL) {
		DeleteObject(Font_Text_);
	}
	Font_Text_ = CreateFontIndirectW(&Font_Info);
	ReleaseDC(NULL, Handle_DC);
	SendDlgItemMessageW(Handle_Dialog, ID_Static_Download, WM_SETFONT, (WPARAM)Font_Text_, TRUE);
	SendDlgItemMessageW(Handle_Dialog, ID_Static_Upload, WM_SETFONT, (WPARAM)Font_Text_, TRUE);

	SetWindowPos(
		Handle_Dialog,
		NULL,
		NULL,
		NULL,
		Origin_Width_Dialog_ * Size / 12,
		Origin_Height_Dialog_ * Size / 12,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER
	);
	SetWindowPos(
		GetDlgItem(Handle_Dialog, ID_Static_Download),
		NULL,
		NULL,
		NULL,
		Origin_Width_Text_*Size / 12,
		Origin_Height_Text_*Size / 12,
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER
	);
	SetWindowPos(
		GetDlgItem(Handle_Dialog, ID_Static_Upload),
		NULL,
		Origin_Width_Dialog_ * Size / 12 / 2 + 2 * Origin_Width_Text_ / 28,
		NULL,
		Origin_Width_Text_*Size / 12,
		Origin_Height_Text_*Size / 12,
		SWP_NOACTIVATE | SWP_NOZORDER
	);
	InvalidateRect(Handle_Dialog, NULL, TRUE);
}

VOID Dialog_Main::Topmost_Check() {
	BOOL Top_State = GetWindowLong(Dialog_Main::Handle_Get(), GWL_EXSTYLE) & WS_EX_TOPMOST;
	static BOOL First = TRUE;
	if (First == FALSE) {
		return;
	}
	if (Top_State == FALSE) {
		MessageBoxW(NULL, L"top error", L"top error", MB_OK);
		First = FALSE;
	}
}

VOID Dialog_Main::Transparency_Set(INT Transparency) {
	SetLayeredWindowAttributes(Dialog_Main::Handle_Get(), 0, (255.0* Transparency) / 100.0 + 0.5, LWA_ALPHA);
}
