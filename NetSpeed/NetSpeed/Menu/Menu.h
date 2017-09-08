#pragma once
#include <WS2tcpip.h>
#include <string>
#include "../Resource/WM_USER.h"

class Menu {
public:
	static VOID Pop();
	static VOID Init();
	static VOID Item_Clean();
	static VOID Item_Exit();
	static VOID Item_Show();
	static VOID Item_Startup();
private:
	static BOOL startup_;
	static BOOL show_;
	static HMENU handle_;
};