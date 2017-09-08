#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>

class Net {
public:
	static VOID Init();
	static VOID RefreshNetSpeedStart();
private:
	static HANDLE thread_refresh_;
	static PMIB_IFROW adapter_info_;
	static DWORD download_old_;
	static DWORD download_new_;
	static DWORD upload_old_;
	static DWORD upload_new_;
	static BOOL thread_exit_;
	static VOID AdapterGet();
	static DWORD WINAPI RefreshNetSpeed(LPVOID);
};