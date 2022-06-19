#pragma once
#include <string>
#include <vector>

class Text {
public:
    enum Language {
        English,
        ChineseSimplified,
        Total
    };

    static void Init() {
        TextVector_.resize(Language::Total);
        Text& Text_ChineseSimplified = TextVector_[Language::ChineseSimplified];
        Text_ChineseSimplified.Button_Clean_ = L"清理";
        Text_ChineseSimplified.Button_CleanAndExit_ = L"清理并退出";
        Text_ChineseSimplified.Button_ColorBackground_ = L"背景颜色";
        Text_ChineseSimplified.Button_ColorText_ = L"文字颜色";
        Text_ChineseSimplified.Dialog_Setting_Caption_ = L"设置";
        Text_ChineseSimplified.MenuItem_Clean_ = L"清理注册表";
        Text_ChineseSimplified.MenuItem_Exit_ = L"退出";
        Text_ChineseSimplified.MenuItem_Hide_ = L"隐藏";
        Text_ChineseSimplified.MenuItem_Setting_ = L"设置";
        Text_ChineseSimplified.MenuItem_Show_ = L"显示";
        Text_ChineseSimplified.MenuItem_Startup_ = L"开机自启";
        Text_ChineseSimplified.Message_CreateSharedMemoryFail_ = L"创建共享内存失败。";
        Text_ChineseSimplified.Message_RegistryOpenFail_ = L"打开注册表失败。";
        Text_ChineseSimplified.Message_CleanFinish_ = L"清理注册表完成。";
        Text_ChineseSimplified.Name_ = L"简体中文";
        Text_ChineseSimplified.Static_Language_ = L"语言";
        Text_ChineseSimplified.Static_Size_ = L"大小";
        Text_ChineseSimplified.Static_Transparency_ = L"透明度：";
    }

    static int Language_Get(std::wstring Language_Name) {
        auto Language_Index = 0;
        for (auto Index = 0; Index <= Text::Language::Total - 1; Index++) {
            if (Text::Name(Index) == Language_Name) {
                Language_Index = Index;
                break;
            }
        }
        return Language_Index;
    }

    static std::wstring Name(int Language) {
        return TextVector_[Language].Name_;
    }

public:
    static inline std::vector<Text> TextVector_ = std::vector<Text>(0);

public:
    std::wstring Button_Clean_ = L"Clean";
    std::wstring Button_CleanAndExit_ = L"Clean And Exit";
    std::wstring Button_ColorBackground_ = L"Background Color";
    std::wstring Button_ColorText_ = L"Text Color";
    std::wstring Dialog_Setting_Caption_ = L"Setting";
    std::wstring MenuItem_Clean_ = L"Clean";
    std::wstring MenuItem_Exit_ = L"Exit";
    std::wstring MenuItem_Hide_ = L"Hide";
    std::wstring MenuItem_Setting_ = L"Setting";
    std::wstring MenuItem_Show_ = L"Show";
    std::wstring MenuItem_Startup_ = L"Startup";
    std::wstring Message_CreateSharedMemoryFail_ = L"Can't create shared memory.";
    std::wstring Message_RegistryOpenFail_ = L"Can't open registry.";
    std::wstring Message_CleanFinish_ = L"All registry has been cleanned.";
    std::wstring Name_ = L"English";
    std::wstring Static_Language_ = L"Language";
    std::wstring Static_Transparency_ = L"Transparency:";
    std::wstring Static_Size_ = L"Size";
};

#ifndef I18N
#define I18N Text::TextVector_[Registry::Language_Get()]
#endif // !I18N