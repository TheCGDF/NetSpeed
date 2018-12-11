#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <set>
#include <string>
class Timer_Refresh;

class Timer_Refresh {
private:
	Timer_Refresh() {};
public:
	static VOID	Init();
	static VOID Start();
private:
	static VOID WINAPI				Process(PVOID Parameter, BOOLEAN);
	static DWORD					DownloadOld_;
	static BOOL						First_;
	static HANDLE					Handle_;
	static HANDLE					Handle_SharedMap_;
	static const SIZE_T				SharedMapSize_;
	static LPWSTR					SharedMapView_;
	static DWORD					UploadOld_;
};
