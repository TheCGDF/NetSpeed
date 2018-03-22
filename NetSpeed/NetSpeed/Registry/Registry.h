#pragma once
#include <WS2tcpip.h>
#include <set>
#include <string>
class Registry;

class Registry {
public:
	static VOID	Clean();
public:
	static COLORREF					ColorBackground_Get();
	static VOID						ColorBackground_Set(COLORREF ColorBackground);
	static COLORREF					ColorCustom_Get(INT ColorCustom_Index);
	static VOID						ColorCustom_Set(INT ColorCustom_Index, COLORREF ColorCustom);
	static COLORREF					ColorText_Get();
	static VOID						ColorText_Set(COLORREF ColorText);
	static INT						Language_Get();
	static VOID						Language_Set(INT Language_Index);
	static std::set<std::wstring>	PCI_Get();
	static POINT					Position_Get();
	static VOID						Position_Set(POINT Position);
	static INT						Size_Get();
	static VOID						Size_Set(INT Size);
	static BOOL						Startup_Get();
	static VOID						Startup_Set(BOOL Startup);
	static INT						Transparency_Get();
	static VOID						Transparency_Set(INT Transparency);
private:
	static LSTATUS	Value_Get(std::wstring Path, std::wstring Value, DWORD Flag, PVOID Data);
	static VOID		Value_Set(std::wstring Path, std::wstring Value, DWORD Type, PBYTE Data, DWORD Size);
	static const std::wstring	Path_NetSpeed_;
	static const std::wstring	Path_Network_;
	static const std::wstring	Path_Startup_;
	static const std::wstring	Value_ColorBackground_;
	static const std::wstring	Value_ColorCustom_[16];
	static const std::wstring	Value_ColorText_;
	static const std::wstring	Value_PositionX_;
	static const std::wstring	Value_PositionY_;
	static const std::wstring	Value_Size_;
	static const std::wstring	Value_Transparency_;
	static const std::wstring	Value_Language_;
};
