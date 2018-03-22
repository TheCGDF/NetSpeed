#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <set>
#include <string>
class Timer_Refresh;

class Timer_Refresh {
public:
	static VOID	Init();
	static VOID Start();
private:
	static VOID WINAPI				Process(PVOID lpParameter, BOOLEAN);
	static DWORD					Download_New_;
	static DWORD					Download_Old_;
	static BOOL						First_;
	static HANDLE					Handle_;
	static std::set<DWORD>			Set_Adapter_;
	static std::set<std::wstring>	Set_PCI_;
	static DWORD					Upload_New_;
	static DWORD					Upload_Old_;
};
