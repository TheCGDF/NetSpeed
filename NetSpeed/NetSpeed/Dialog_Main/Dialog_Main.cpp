#include "Dialog_Main.h"
#include "../Dialog_Setting/Dialog_Setting.h"
#include "../Menu/Menu.h"
#include "../Timer_Refresh/Timer_Refresh.h"
#include "../NotifyIcon/NotifyIcon.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"

HBRUSH	Dialog_Main::Handle_Color_Background_;

//public:

INT_PTR Dialog_Main::Process(HWND Handle_Dialog, UINT Message_Dialog, WPARAM Param_WORD, LPARAM Param_LONG) {
	Handle_Set(Handle_Dialog);
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
			return (INT_PTR)Handle_Color_Background_;
		}
		case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)Param_WORD, TRANSPARENT);
			SetTextColor((HDC)Param_WORD, RGB(255, 255, 255));
			return (INT_PTR)Handle_Color_Background_;
		}
		case WM_DESTROY:
		{
			NotifyIcon::Destroy();
			return 0;
		}
		case WM_EXITSIZEMOVE:
		{
			Registry::Position_Save();
			break;
		}
		case WM_INITDIALOG:
		{
			Handle_Color_Background_ = CreateSolidBrush(RGB(0, 0, 0));
			POINT Position = Registry::Position_Get();
			Position_Set(Position);
			SetWindowLongW(
				Handle_Get(),
				GWL_EXSTYLE,
				GetWindowLongW(Handle_Get(), GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW
			);
			INT Transparency = Registry::Transparency_Get();
			Transparency_Set(Transparency);
			Timer_Refresh::Start();
			break;
		}
		case WM_LBUTTONDOWN:
		{
			SendMessageW(Handle_Get(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(LOWORD(Param_LONG), HIWORD(Param_LONG)));
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

POINT Dialog_Main::Position_Get() {
	RECT Position_RECT;
	GetWindowRect(Dialog_Main::Handle_Get(), &Position_RECT);
	MapWindowPoints(HWND_DESKTOP, GetParent(Dialog_Main::Handle_Get()), (LPPOINT)&Position_RECT, sizeof(RECT) / sizeof(POINT));
	POINT Position_POINT;
	Position_POINT.x = Position_RECT.left;
	Position_POINT.y = Position_RECT.top;
	return Position_POINT;
}

VOID Dialog_Main::Position_Set(POINT Position) {
	SetWindowPos(Handle_Get(), NULL, Position.x, Position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL Dialog_Main::Show_Get() {
	return IsWindowVisible(Handle_Get());
}

VOID Dialog_Main::Show_Set(BOOL Show) {
	if (Show == TRUE) {
		ShowWindow(Dialog_Main::Handle_Get(), SW_SHOW);
	}
	else {
		ShowWindow(Dialog_Main::Handle_Get(), SW_HIDE);
	}
	if (Dialog_Setting::Handle_Get() != NULL) {
		Dialog_Setting::Refresh_Check_Setting_Show();
	}
}

INT Dialog_Main::Transparency_Get() {
	BYTE Transparency_BYTE;
	GetLayeredWindowAttributes(Dialog_Main::Handle_Get(), NULL, &Transparency_BYTE, NULL);
	INT Transparency_INT = Transparency_BYTE * 100.0 / 255.0 + 0.5;
	return Transparency_INT;
}

VOID Dialog_Main::Transparency_Set(INT Transparency) {
	SetLayeredWindowAttributes(Handle_Get(), 0, (255.0* Transparency) / 100.0 + 0.5, LWA_ALPHA);
}
