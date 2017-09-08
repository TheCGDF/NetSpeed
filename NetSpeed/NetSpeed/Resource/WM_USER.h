#pragma once
#include <WS2tcpip.h>

enum {
	WM_NOTIFYICON = WM_USER + 1,
	WM_MENUSHOW,
	WM_MENUSTARTUP,
	WM_MENUCLEAN,
	WM_MENUEXIT
};