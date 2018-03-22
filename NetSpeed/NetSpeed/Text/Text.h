#pragma once
#include <WS2tcpip.h>
#include <vector>
class Text;

class Text {
public:
	enum Language {
		English,
		Chinese_Simplified,
		Total
	};
	static VOID			Init();
	static INT			Language_Get(std::wstring Language_Name);
	static std::wstring	Name(INT Language);
public:
	static std::wstring	Button_Clean();
	static std::wstring	Button_CleanAndExit();
	static std::wstring	Check_Show();
	static std::wstring	Check_Startup();
	static std::wstring	Dialog_Caption();
	static std::wstring	Menu_Item_Clean();
	static std::wstring	Menu_Item_Exit();
	static std::wstring	Menu_Item_Hide();
	static std::wstring	Menu_Item_Setting();
	static std::wstring	Menu_Item_Show();
	static std::wstring	Menu_Item_Startup();
	static std::wstring	Message_CleanFinish();
	static std::wstring	Message_AdapterFindFail();
	static std::wstring	Message_RegistryOpenFail();
	static std::wstring	Name();
	static std::wstring	Static_Language();
	static std::wstring	Static_Size();
	static std::wstring	Static_Transparency();
private:
	static std::vector<Text>	Vector_Text_;
	std::wstring				Menu_Item_Clean_ = L"Clean";
	std::wstring				Menu_Item_Exit_ = L"Exit";
	std::wstring				Menu_Item_Hide_ = L"Hide";
	std::wstring				Menu_Item_Setting_ = L"Setting";
	std::wstring				Menu_Item_Show_ = L"Show";
	std::wstring				Menu_Item_Startup_ = L"Startup";
	std::wstring				Message_AdapterFindFail_ = L"Can't find adapter.";
	std::wstring				Message_RegistryOpenFail_ = L"Can't open registry.";
	std::wstring				Message_CleanFinish_ = L"All registry has been cleanned.";
	std::wstring				Name_ = L"English";
	std::wstring				Button_Clean_ = L"Clean";
	std::wstring				Button_CleanAndExit_ = L"Clean And Exit";
	std::wstring				Dialog_Setting_Caption_ = L"Setting";
	std::wstring				Static_Language_ = L"Language";
	std::wstring				Static_Transparency_ = L"Transparency:";
	std::wstring				Static_Size_ = L"Size";
	std::wstring				Check_Show_ = L"Show";
	std::wstring				Check_Startup_ = L"Startup";
};
