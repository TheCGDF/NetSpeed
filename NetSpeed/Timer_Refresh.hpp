#pragma once
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <strsafe.h>

#include "Dialog/Dialog_Main/Dialog_Main.h"
#include "Registry.hpp"
#include "Resource/resource.h"

class Timer_Refresh {
private:
    Timer_Refresh() {};

public:
    static void Init() {
        Handle_SharedMap_ = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SharedMapSize_, L"NetSpeed");
        if (Handle_SharedMap_ == NULL) {
            MessageBoxW(NULL, I18N.Message_CreateSharedMemoryFail_.c_str(), L"", MB_OK);
        }
        else {
            SharedMapView_ = (LPWSTR)MapViewOfFile(Handle_SharedMap_, FILE_MAP_WRITE, 0, 0, 0);
            if (SharedMapView_ == NULL) {
                MessageBoxW(NULL, I18N.Message_CreateSharedMemoryFail_.c_str(), L"", MB_OK);
            }
        }
    }

    static void Start() {
        CreateTimerQueueTimer(&Handle_, NULL, Timer_Refresh::Process, NULL, 0, 1000, 0);
    }

private:
    static inline DWORD DownloadOld_ = 0L;
    static inline bool First_ = true;
    static inline HANDLE Handle_ = NULL;
    static inline HANDLE Handle_SharedMap_ = NULL;
    static inline const SIZE_T SharedMapSize_ = 0x1000;
    static inline LPWSTR SharedMapView_ = NULL;
    static inline DWORD UploadOld_ = 0L;

    static void WINAPI Process(PVOID Parameter, BOOLEAN) {
        auto PCISet = Registry::PCI_Get();
        DWORD TableSize = sizeof(MIB_IFTABLE);
        auto AdapterTable = (PMIB_IFTABLE)HeapAlloc(GetProcessHeap(), 0, TableSize);
        GetIfTable(AdapterTable, &TableSize, false);
        HeapFree(GetProcessHeap(), 0, AdapterTable);
        AdapterTable = (PMIB_IFTABLE)HeapAlloc(GetProcessHeap(), 0, TableSize);
        GetIfTable(AdapterTable, &TableSize, false);
        auto AdapterNumber = AdapterTable->dwNumEntries;
        HeapFree(GetProcessHeap(), 0, AdapterTable);

        auto AdapterInfo = (PMIB_IFROW)HeapAlloc(GetProcessHeap(), 0, sizeof(MIB_IFROW));
        AdapterInfo->dwIndex = 0;

        DWORD DownloadNew_ = 0L;
        DWORD UploadNew_ = 0L;
        for (auto AdapterIndex = 0L; AdapterIndex < AdapterNumber; AdapterIndex++) {
            AdapterInfo->dwIndex = AdapterIndex;
            if (GetIfEntry(AdapterInfo) != NO_ERROR) {
                continue;
            }
            for (auto PCI : PCISet) {
                if (std::wstring(AdapterInfo->wszName).find(PCI) != std::string::npos) {
                    DownloadNew_ += AdapterInfo->dwInOctets;
                    UploadNew_ += AdapterInfo->dwOutOctets;
                }
            }
        }

        HeapFree(GetProcessHeap(), 0, AdapterInfo);

        const auto DownloadDisparityRaw = DownloadNew_ - DownloadOld_;
        const auto UploadDisparityRaw = UploadNew_ - UploadOld_;
        auto DownloadDisparity = DownloadDisparityRaw / 1024.0;
        auto UploadDisparity = UploadDisparityRaw / 1024.0;
        auto UnitDownload = L"K";
        auto UnitUpload = L"K";
        if (DownloadDisparity > 1024) {
            DownloadDisparity = DownloadDisparity / 1024.0;
            UnitDownload = L"M";
        }
        if (UploadDisparity > 1024) {
            UploadDisparity = UploadDisparity / 1024.0;
            UnitUpload = L"M";
        }

        auto DownloadSpeed = std::to_wstring(DownloadDisparity + 0.05);
        auto UploadSpeed = std::to_wstring(UploadDisparity + 0.05);
        DownloadSpeed.erase(DownloadSpeed.end() - 5, DownloadSpeed.end());
        UploadSpeed.erase(UploadSpeed.end() - 5, UploadSpeed.end());
        DownloadSpeed.append(UnitDownload);
        UploadSpeed.append(UnitUpload);
        DownloadOld_ = DownloadNew_;
        UploadOld_ = UploadNew_;
        auto Handle_DialogMain = Dialog_Main::Handle_Get();
        if (First_ == true) {
            SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, L"...");
            SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, L"...");
            First_ = false;
            return;
        }
        SetDlgItemTextW(Handle_DialogMain, ID_Static_Download, DownloadSpeed.c_str());
        SetDlgItemTextW(Handle_DialogMain, ID_Static_Upload, UploadSpeed.c_str());
        auto SharedMapJson = L"{\"download\":" +
            std::to_wstring(DownloadDisparityRaw) +
            L",\"upload\":" +
            std::to_wstring(UploadDisparityRaw) +
            L"}";
        StringCchCopyW(SharedMapView_, SharedMapSize_, SharedMapJson.c_str());
    }
};