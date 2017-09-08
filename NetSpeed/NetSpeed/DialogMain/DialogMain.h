#pragma once
#include <WS2tcpip.h>

extern HWND dialog_main;
extern NOTIFYICONDATAW data_;

INT_PTR WINAPI DialogMainProcess(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
