#include "Net.h"
#include <string>
#include "../Dialog_Main/Dialog_Main.h"
#include "../Resource/resource.h"
#include "../Text/Text.h"

HANDLE		Net::Thread_Refresh_;
PMIB_IFROW	Net::Adapter_Info_ = nullptr;
DWORD		Net::Download_Old_ = 0;
DWORD		Net::Download_New_;
DWORD		Net::Upload_Old_ = 0;
DWORD		Net::Upload_New_;
BOOL		Net::Thread_Exit_ = FALSE;

//public:

VOID Net::Init() {
	Adapter_Info_ = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
	Adapter_Info_->dwIndex = 0;
}

VOID Net::Refresh_Start() {
	Thread_Refresh_ = CreateThread(NULL, 0, NetSpeed_Refresh, NULL, 0, NULL);
}

//private:

VOID Net::Adapter_Get() {
	if (GetIfEntry(Adapter_Info_) == NO_ERROR) {
		if (Adapter_Info_->dwType == IF_TYPE_IEEE80211) {
			return;
		}
	}
	DWORD Size_Table = sizeof(MIB_IFTABLE);
	MIB_IFTABLE *Adapter_Table = (MIB_IFTABLE *)HeapAlloc(GetProcessHeap(), 0, Size_Table);
	GetIfTable(Adapter_Table, &Size_Table, FALSE);
	HeapFree(GetProcessHeap(), 0, Adapter_Table);
	Adapter_Table = (MIB_IFTABLE *)HeapAlloc(GetProcessHeap(), 0, Size_Table);
	GetIfTable(Adapter_Table, &Size_Table, FALSE);
	DWORD Adapter_Number = Adapter_Table->dwNumEntries;
	HeapFree(GetProcessHeap(), 0, Adapter_Table);
	for (DWORD Index = 0; Index <= Adapter_Number - 1; Index++) {
		Adapter_Info_->dwIndex = Index;
		if (GetIfEntry(Adapter_Info_) != NO_ERROR) {
			continue;
		}
		if (
			Adapter_Info_->dwType == IF_TYPE_IEEE80211&&
			Adapter_Info_->dwSpeed != 0 &&
			Adapter_Info_->dwOperStatus == IF_OPER_STATUS_OPERATIONAL
			) {
			return;
		}
	}
	MessageBoxW(NULL, Text::Message_AdapterFindFail().c_str(), L"", MB_OK);
	EndDialog(Dialog_Main::Handle_Get(), 0);
}

DWORD Net::NetSpeed_Refresh(LPVOID) {
	while (TRUE) {
		Adapter_Get();
		Download_New_ = Adapter_Info_->dwInOctets;
		Upload_New_ = Adapter_Info_->dwOutOctets;
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
		SetDlgItemTextW(Dialog_Main::Handle_Get(), ID_Static_Download, Download_Speed.c_str());
		SetDlgItemTextW(Dialog_Main::Handle_Get(), ID_Static_Upload, Upload_Speed.c_str());
		Sleep(1000);
	}
	return 0;
}
