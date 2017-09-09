#include "DialogMain.h"
#include <shellapi.h>
#include "../Menu/Menu.h"
#include "../Net/Net.h"
#include "../NotifyIcon/NotifyIcon.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"

HWND dialog_main;

INT_PTR WINAPI DialogMainProcess(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) {
	dialog_main = dialog;
	BOOL handle_result = TRUE;
	switch (message) {
		case WM_CTLCOLORDLG:
		{
			return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
		}
		case WM_CTLCOLORSTATIC:
		{
			SetBkMode((HDC)wparam, TRANSPARENT);
			SetTextColor((HDC)wparam, RGB(255, 255, 255));
			return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wparam)) {
				case WM_MENUCLEAN:
				{
					Menu::Item_Clean();
					break;
				}
				case WM_MENUEXIT:
				{
					Menu::Item_Exit();
					break;
				}
				case WM_MENUSHOW:
				{
					Menu::Item_Show();
					break;
				}
				case WM_MENUSTARTUP:
				{
					Menu::Item_Startup();
					break;
				}
				default:
				{
					handle_result = FALSE;
					break;
				}
			}
			break;
		}
		case WM_INITDIALOG:
		{
			POINT dialog_position= Registry::Position_Get();
			if (dialog_position.x != -1 && dialog_position.y != -1) {
				SetWindowPos(dialog_main, NULL, dialog_position.x, dialog_position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			}
			SetWindowLongW(
				dialog_main,
				GWL_EXSTYLE,
				GetWindowLongW(dialog_main, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW
			);
			SetLayeredWindowAttributes(dialog_main, 0, (255 * 50) / 100, LWA_ALPHA);
			NotifyIcon::Init();
			Net::RefreshNetSpeedStart();
			break;
		}
		case WM_NOTIFYICON:
		{
			switch (lparam) {
				case WM_RBUTTONUP:
				{
					Menu::Pop();
					return TRUE;
				}
				default:
				{
					handle_result = FALSE;
					break;
				}
			}
		}
		case WM_LBUTTONDOWN:
		{
			//It will also influence WM_LBUTTONUP
			SendMessageW(dialog_main, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(LOWORD(lparam), HIWORD(lparam)));
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			Registry::Position_Create();
			break;
		}
		case WM_RBUTTONUP:
		{
			Menu::Pop();
			break;
		}
		default:
		{
			handle_result = FALSE;
		}
	}
	return handle_result;
}
