#pragma once
#include <WS2tcpip.h>

class NotifyIcon {
public:
	static VOID Init();
	static VOID Delete();
private:
	static NOTIFYICONDATAW data_;
};