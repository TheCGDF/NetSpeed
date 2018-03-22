#include "Timer_Refresh.h"
#include <string>

#include "../Dialog/Dialog_Main/Dialog_Main.h"
#include "../Registry/Registry.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

DWORD					Timer_Refresh::Download_New_;
DWORD					Timer_Refresh::Download_Old_;
BOOL					Timer_Refresh::First_;
HANDLE					Timer_Refresh::Handle_ = NULL;
std::set<DWORD>			Timer_Refresh::Set_Adapter_;
std::set<std::wstring>	Timer_Refresh::Set_PCI_;
DWORD					Timer_Refresh::Upload_New_;
DWORD					Timer_Refresh::Upload_Old_;

//public:

VOID Timer_Refresh::Init() {
	Set_PCI_ = Registry::PCI_Get();
	Set_Adapter_.clear();

	DWORD Size_Table = sizeof(MIB_IFTABLE);
	MIB_IFTABLE *Table_Adapter = (MIB_IFTABLE *)HeapAlloc(GetProcessHeap(), 0, Size_Table);
	GetIfTable(Table_Adapter, &Size_Table, FALSE);
	HeapFree(GetProcessHeap(), 0, Table_Adapter);
	Table_Adapter = (MIB_IFTABLE *)HeapAlloc(GetProcessHeap(), 0, Size_Table);
	GetIfTable(Table_Adapter, &Size_Table, FALSE);
	DWORD Adapter_Number = Table_Adapter->dwNumEntries;
	HeapFree(GetProcessHeap(), 0, Table_Adapter);

	static PMIB_IFROW Adapter_Info = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
	Adapter_Info->dwIndex = 0;

	for (DWORD Index_Adapter = 0; Index_Adapter <= Adapter_Number - 1; Index_Adapter++) {
		Adapter_Info->dwIndex = Index_Adapter;
		if (GetIfEntry(Adapter_Info) != NO_ERROR) {
			continue;
		}
		for (std::wstring PCI : Set_PCI_) {
			if (std::wstring(Adapter_Info->wszName).find(PCI) != std::wstring::npos) {
				Set_Adapter_.insert(Adapter_Info->dwIndex);
			}
		}
	}

	HeapFree(GetProcessHeap(), 0, Adapter_Info);
	if (Set_Adapter_.empty()) {
		MessageBoxW(NULL, Text::Message_AdapterFindFail().c_str(), L"", MB_OK);
		EndDialog(Dialog_Main::Handle_Get(), 0);
	}

	First_ = TRUE;
	Download_Old_ = 0;
	Upload_Old_ = 0;
}

VOID Timer_Refresh::Start() {
	if (Set_Adapter_.empty()) {
		return;
	}
	CreateTimerQueueTimer(&Handle_, NULL, Process, NULL, 0, 1000, 0);
}

//private:

VOID WINAPI Timer_Refresh::Process(PVOID Parameter, BOOLEAN) {
	PMIB_IFROW Adapter_Info_ = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
	for (DWORD Index_Adapter : Set_Adapter_) {
		Adapter_Info_->dwIndex = Index_Adapter;
		if (GetIfEntry(Adapter_Info_) != NO_ERROR) {
			HeapFree(GetProcessHeap(), 0, Adapter_Info_);
			Timer_Refresh::Init();
			return;

		}
		Download_New_ = Adapter_Info_->dwInOctets;
		Upload_New_ = Adapter_Info_->dwOutOctets;
	}
	HeapFree(GetProcessHeap(), 0, Adapter_Info_);

	DOUBLE Download_Disparity = (Download_New_ - Download_Old_) / 1024.0;
	DOUBLE Upload_Disparity = (Upload_New_ - Upload_Old_) / 1024.0;
	std::wstring Unit_Download = L"K";
	std::wstring Unit_Upload = L"K";
	if (Download_Disparity > 1024) {
		Download_Disparity = Download_Disparity / 1024.0;
		Unit_Download = L"M";
	}
	if (Upload_Disparity > 1024) {
		Upload_Disparity = Upload_Disparity / 1024.0;
		Unit_Upload = L"M";
	}

	std::wstring Download_Speed = std::to_wstring(Download_Disparity + 0.05);
	std::wstring Upload_Speed = std::to_wstring(Upload_Disparity + 0.05);
	Download_Speed.erase(Download_Speed.end() - 5, Download_Speed.end());
	Upload_Speed.erase(Upload_Speed.end() - 5, Upload_Speed.end());
	Download_Speed.append(Unit_Download);
	Upload_Speed.append(Unit_Upload);
	Download_Old_ = Download_New_;
	Upload_Old_ = Upload_New_;
	HWND Handle_DialogMain = Dialog_Main::Handle_Get();
	if (First_ == TRUE) {
		SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, L"...");
		SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, L"...");
		First_ = FALSE;
		return;
	}
	SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, Download_Speed.c_str());
	SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, Upload_Speed.c_str());
}
