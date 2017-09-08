#include "Net.h"
#include <string>
#include "../Resource/resource.h"
#include "../DialogMain/DialogMain.h"

HANDLE Net::thread_refresh_;
PMIB_IFROW Net::adapter_info_ = nullptr;
DWORD Net::download_old_ = 0;
DWORD Net::download_new_;
DWORD Net::upload_old_ = 0;
DWORD Net::upload_new_;
BOOL Net::thread_exit_ = FALSE;

VOID Net::Init() {
	adapter_info_ = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
	adapter_info_->dwIndex = 0;
}

VOID Net::RefreshNetSpeedStart() {
	thread_refresh_ = CreateThread(NULL, 0, RefreshNetSpeed, NULL, 0, NULL);
}
VOID Net::AdapterGet() {
	if (GetIfEntry(adapter_info_) == NO_ERROR) {
		if (adapter_info_->dwType == IF_TYPE_IEEE80211) {
			return;
		}
	}

	for (DWORD index = 0; index <= 100; index++) {
		adapter_info_->dwIndex = index;
		if (GetIfEntry(adapter_info_) != NO_ERROR) {
			continue;
		}
		if (adapter_info_->dwType == IF_TYPE_IEEE80211) {
			break;
		}
	}
}

DWORD Net::RefreshNetSpeed(LPVOID) {
	while (TRUE) {
		Sleep(1000);
		AdapterGet();
		download_new_ = adapter_info_->dwInOctets;
		upload_new_ = adapter_info_->dwOutOctets;
		DOUBLE download_disparity = (download_new_ - download_old_) / 1024.0;
		DOUBLE upload_disparity = (upload_new_ - upload_old_) / 1024.0;
		std::wstring unit_download = L"K";
		std::wstring unit_upload = L"K";
		if (download_disparity > 1024) {
			download_disparity = download_disparity / 1024.0;
			unit_download = L"M";
		}
		if (upload_disparity > 1024) {
			upload_disparity = upload_disparity / 1024.0;
			unit_upload = L"M";
		}
		std::wstring download_speed = std::to_wstring(download_disparity + 0.05);
		std::wstring upload_speed = std::to_wstring(upload_disparity + 0.05);
		download_speed.erase(download_speed.end() - 5, download_speed.end());
		upload_speed.erase(upload_speed.end() - 5, upload_speed.end());
		download_speed.append(unit_download);
		upload_speed.append(unit_upload);
		download_old_ = download_new_;
		upload_old_ = upload_new_;
		SetDlgItemTextW(dialog_main, STATIC_DOWNLOAD, download_speed.c_str());
		SetDlgItemTextW(dialog_main, STATIC_UPLOAD, upload_speed.c_str());
	}
	return 0;
}
