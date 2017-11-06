#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>
class Net;

class Net {
public:
	static VOID Init();
	static VOID Refresh_Start();
private:
	static VOID			Adapter_Get();
	static DWORD WINAPI	NetSpeed_Refresh(LPVOID);
	static PMIB_IFROW	Adapter_Info_;
	static DWORD		Download_New_;
	static DWORD		Download_Old_;
	static BOOL			Thread_Exit_;
	static HANDLE		Thread_Refresh_;
	static DWORD		Upload_Old_;
	static DWORD		Upload_New_;
};