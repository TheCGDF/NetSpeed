#pragma once
#include <WS2tcpip.h>
#include <string>
class Registry;

class Registry {
public:
	static VOID			Clean();
	static std::wstring	Language_Get();
	static VOID			Language_Save();
	static POINT		Position_Get();
	static VOID			Position_Save();
	static BOOL			Startup_Get();
	static VOID			Startup_Set(BOOL Startup);
	static INT			Transparency_Get();
	static VOID			Transparency_Save();
private:
	static LSTATUS	Value_Get(std::wstring Value, DWORD Flag, PVOID Data);
	static VOID		Value_Set(std::wstring Value, DWORD Type, PBYTE Data, DWORD Size);
	static const std::wstring	Path_NetSpeed_;
	static const std::wstring	Path_Startup_;
	static const std::wstring	Value_Position_X_;
	static const std::wstring	Value_Position_Y_;
	static const std::wstring	Value_Transparency_;
	static const std::wstring	Value_Language_;
private:
};