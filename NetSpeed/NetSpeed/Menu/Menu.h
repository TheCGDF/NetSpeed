#pragma once
#include <WS2tcpip.h>
#include <string>
class Menu;
#include "../Resource/WM_USER.h"

class Menu {
public:
	static VOID Init();
	static VOID Item_Exit();
	static VOID Item_Setting();
	static VOID Item_Show();
	static VOID Item_Startup();
	static VOID Pop();
private:
	enum Item {
		Show,
		Startup,
		Setting,
		Exit,
		Total
	};
	static HMENU	Handle_;
	static BOOL		Show_;
	static BOOL		Startup_;
};