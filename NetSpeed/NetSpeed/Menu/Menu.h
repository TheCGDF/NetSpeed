#pragma once
#include <WS2tcpip.h>
#include <string>
class Menu;
#include "../Resource/WM_USER.h"

class Menu {
public:
	static VOID Init();
	static VOID Pop();
public:
	static VOID Item_Exit();
	static VOID Item_Setting();
	static VOID Item_Show();
	static VOID Item_Startup();
private:
	enum Item {
		Show,
		Startup,
		Setting,
		Exit,
		Total
	};
	static HMENU	Handle_;
};
