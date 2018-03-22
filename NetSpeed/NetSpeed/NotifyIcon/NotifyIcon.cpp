#include "NotifyIcon.h"

#include "../Dialog/Dialog_Main/Dialog_Main.h"
#include "../Resource/resource.h"
#include "../Resource/WM_USER.h"

NOTIFYICONDATAW	NotifyIcon::Data_;

//public:

VOID NotifyIcon::Destroy() {
	Shell_NotifyIconW(NIM_DELETE, &Data_);
}

VOID NotifyIcon::Init() {
	Data_.cbSize = sizeof(Data_);
	Data_.hIcon = (HICON)LoadImageW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(ID_Icon_Main), IMAGE_ICON, 0, 0, LR_SHARED);
	Data_.uCallbackMessage = WM_NOTIFYICON;
	Data_.hWnd = Dialog_Main::Handle_Get();
	Data_.uID = ID_NotifyIcon_Main;
	Data_.uVersion = NOTIFYICON_VERSION_4;
	wcscpy_s(Data_.szTip, L"NetSpeed v1.2.0");
	Data_.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	Shell_NotifyIconW(NIM_ADD, &Data_);
}
