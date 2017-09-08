#include "NotifyIcon.h"
#include "../DialogMain/DialogMain.h"
#include "../Resource/resource.h"
#include "../Resource/WM_USER.h"

NOTIFYICONDATAW NotifyIcon::data_;

VOID NotifyIcon::Init() {
	data_.cbSize = sizeof(data_);
	data_.hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(ICON_MAIN), IMAGE_ICON, 0, 0, LR_SHARED);
	data_.uCallbackMessage = WM_NOTIFYICON;
	data_.hWnd = dialog_main;
	data_.uID = NOTIFYICON_MAIN;
	data_.uVersion = NOTIFYICON_VERSION_4;
	wcscpy_s(data_.szTip, L"NetSpeed");
	data_.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	Shell_NotifyIconW(NIM_ADD, &data_);
}

VOID NotifyIcon::Delete() {
	Shell_NotifyIconW(NIM_DELETE, &data_);
}
