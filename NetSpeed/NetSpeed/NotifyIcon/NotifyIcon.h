#pragma once
#include <WS2tcpip.h>
class NotifyIcon;

class NotifyIcon {
private:
	NotifyIcon() {};
public:
	static VOID Destroy();
	static VOID Init();
private:
	static NOTIFYICONDATAW Data_;
};
