#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>
class Timer_Refresh;

class Timer_Refresh {
public:
	static VOID Init();
	static VOID Start();
private:
	static VOID			Adapter_Get();
	static VOID WINAPI	Process(PVOID lpParameter, BOOLEAN);
	static PMIB_IFROW	Adapter_Info_;
	static DWORD		Download_New_;
	static DWORD		Download_Old_;
	static BOOL			First_;
	static HANDLE		Handle_;
	static BOOL			Thread_Exit_;
	static HANDLE		Thread_Refresh_;
	static DWORD		Upload_Old_;
	static DWORD		Upload_New_;
};