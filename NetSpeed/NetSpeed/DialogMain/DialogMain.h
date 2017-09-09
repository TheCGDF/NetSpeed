#pragma once
#include <WS2tcpip.h>

extern HBRUSH color_background;
extern HWND dialog_main;

INT_PTR WINAPI DialogMainProcess(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
