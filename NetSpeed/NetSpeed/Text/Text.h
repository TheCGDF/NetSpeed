#pragma once
#include <WS2tcpip.h>
#include <string>
#include <vector>
class Text;

class Text {
public:
	enum Language {
		English,
		ChineseSimplified,
		Total
	};
	static VOID			Init();
	static INT			Language_Get(std::wstring LanguageName);
	static std::wstring	Name(INT Language);
public:
	static std::wstring	Button_Clean();
	static std::wstring	Button_CleanAndExit();
	static std::wstring	Button_ColorBackground();
	static std::wstring	Button_ColorText();
	static std::wstring	Check_Show();
	static std::wstring	Check_Startup();
	static std::wstring	Dialog_Caption();
	static std::wstring	MenuItem_Clean();
	static std::wstring	MenuItem_Exit();
	static std::wstring	MenuItem_Hide();
	static std::wstring	MenuItem_Setting();
	static std::wstring	MenuItem_Show();
	static std::wstring	MenuItem_Startup();
	static std::wstring	Message_CleanFinish();
	static std::wstring	Message_CreateSharedMemoryFail();
	static std::wstring	Message_RegistryOpenFail();
	static std::wstring	Name();
	static std::wstring	Static_Language();
	static std::wstring	Static_Size();
	static std::wstring	Static_Transparency();
private:
	static std::vector<Text>	TextVector_;
private:
	std::wstring				Button_Clean_ = L"Clean";
	std::wstring				Button_CleanAndExit_ = L"Clean And Exit";
	std::wstring				Button_ColorBackground_ = L"Background Color";
	std::wstring				Button_ColorText_ = L"Text Color";
	std::wstring				Check_Show_ = L"Show";
	std::wstring				Check_Startup_ = L"Startup";
	std::wstring				Dialog_Setting_Caption_ = L"Setting";
	std::wstring				MenuItem_Clean_ = L"Clean";
	std::wstring				MenuItem_Exit_ = L"Exit";
	std::wstring				MenuItem_Hide_ = L"Hide";
	std::wstring				MenuItem_Setting_ = L"Setting";
	std::wstring				MenuItem_Show_ = L"Show";
	std::wstring				MenuItem_Startup_ = L"Startup";
	std::wstring				Message_CreateSharedMemoryFail_ = L"Can't create shared memory.";
	std::wstring				Message_RegistryOpenFail_ = L"Can't open registry.";
	std::wstring				Message_CleanFinish_ = L"All registry has been cleanned.";
	std::wstring				Name_ = L"English";
	std::wstring				Static_Language_ = L"Language";
	std::wstring				Static_Transparency_ = L"Transparency:";
	std::wstring				Static_Size_ = L"Size";
};
