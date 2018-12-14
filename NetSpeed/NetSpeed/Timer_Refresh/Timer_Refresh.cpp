#include "Timer_Refresh.h"
#include <strsafe.h>
#include <string>

#include "../Dialog/Dialog_Main/Dialog_Main.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

DWORD					Timer_Refresh::DownloadOld_ = 0;
BOOL					Timer_Refresh::First_ = TRUE;
HANDLE					Timer_Refresh::Handle_ = NULL;
HANDLE					Timer_Refresh::Handle_SharedMap_ = NULL;
const SIZE_T			Timer_Refresh::SharedMapSize_ = 0x1000;
LPWSTR					Timer_Refresh::SharedMapView_ = NULL;
DWORD					Timer_Refresh::UploadOld_ = 0;

//public:

VOID Timer_Refresh::Init() {
	Handle_SharedMap_ = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SharedMapSize_, L"NetSpeed");
	if (Handle_SharedMap_ == NULL) {
		MessageBoxW(NULL, Text::Message_CreateSharedMemoryFail().c_str(), L"", MB_OK);
	}
	else {
		SharedMapView_ = (LPWSTR)MapViewOfFile(Handle_SharedMap_, FILE_MAP_WRITE, 0, 0, 0);
		if (SharedMapView_ == NULL) {
			MessageBoxW(NULL, Text::Message_CreateSharedMemoryFail().c_str(), L"", MB_OK);
		}
	}
}

VOID Timer_Refresh::Start() {
	CreateTimerQueueTimer(&Handle_, NULL, Timer_Refresh::Process, NULL, 0, 1000, 0);
}

//private:

VOID WINAPI Timer_Refresh::Process(PVOID Parameter, BOOLEAN) {
	Dialog_Main::Topmost_Check();

	std::set<std::wstring> PCISet = Registry::PCI_Get();
	DWORD TableSize = sizeof(MIB_IFTABLE);
	PMIB_IFTABLE AdapterTable = (PMIB_IFTABLE)HeapAlloc(GetProcessHeap(), 0, TableSize);
	GetIfTable(AdapterTable, &TableSize, FALSE);
	HeapFree(GetProcessHeap(), 0, AdapterTable);
	AdapterTable = (PMIB_IFTABLE)HeapAlloc(GetProcessHeap(), 0, TableSize);
	GetIfTable(AdapterTable, &TableSize, FALSE);
	DWORD AdapterNumber = AdapterTable->dwNumEntries;
	HeapFree(GetProcessHeap(), 0, AdapterTable);

	PMIB_IFROW AdapterInfo = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
	AdapterInfo->dwIndex = 0;

	DWORD DownloadNew_ = 0;
	DWORD UploadNew_ = 0;
	for (DWORD AdapterIndex = 0; AdapterIndex <= AdapterNumber - 1; AdapterIndex++) {
		AdapterInfo->dwIndex = AdapterIndex;
		if (GetIfEntry(AdapterInfo) != NO_ERROR) {
			continue;
		}
		for (std::wstring PCI : PCISet) {
			if (std::wstring(AdapterInfo->wszName).find(PCI) != std::string::npos) {
				DownloadNew_ += AdapterInfo->dwInOctets;
				UploadNew_ += AdapterInfo->dwOutOctets;
			}
		}
	}

	HeapFree(GetProcessHeap(), 0, AdapterInfo);

	CONST DWORD DownloadDisparityRaw = DownloadNew_ - DownloadOld_;
	CONST DWORD UploadDisparityRaw = UploadNew_ - UploadOld_;
	DOUBLE DownloadDisparity = DownloadDisparityRaw / 1024.0;
	DOUBLE UploadDisparity = UploadDisparityRaw / 1024.0;
	std::wstring UnitDownload = L"K";
	std::wstring UnitUpload = L"K";
	if (DownloadDisparity > 1024) {
		DownloadDisparity = DownloadDisparity / 1024.0;
		UnitDownload = L"M";
	}
	if (UploadDisparity > 1024) {
		UploadDisparity = UploadDisparity / 1024.0;
		UnitUpload = L"M";
	}

	std::wstring DownloadSpeed = std::to_wstring(DownloadDisparity + 0.05);
	std::wstring UploadSpeed = std::to_wstring(UploadDisparity + 0.05);
	DownloadSpeed.erase(DownloadSpeed.end() - 5, DownloadSpeed.end());
	UploadSpeed.erase(UploadSpeed.end() - 5, UploadSpeed.end());
	DownloadSpeed.append(UnitDownload);
	UploadSpeed.append(UnitUpload);
	DownloadOld_ = DownloadNew_;
	UploadOld_ = UploadNew_;
	HWND Handle_DialogMain = Dialog_Main::Handle_Get();
	if (First_ == TRUE) {
		SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, L"...");
		SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, L"...");
		First_ = FALSE;
		return;
	}
	SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, DownloadSpeed.c_str());
	SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, UploadSpeed.c_str());
	std::wstring SharedMapJson =
		L"{\"download\":" +
		std::to_wstring(DownloadDisparityRaw) +
		L",\"upload\":" +
		std::to_wstring(UploadDisparityRaw) +
		L"}";
	StringCchCopyW(SharedMapView_, SharedMapSize_, SharedMapJson.c_str());
}
