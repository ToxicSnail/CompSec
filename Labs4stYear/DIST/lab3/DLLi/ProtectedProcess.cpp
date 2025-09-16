#define LOCAL_BLOCKDLLPOLICY
#define STOP_ARG "antihack"

#include <windows.h>
#include <stdio.h>
#include <string.h>

BOOL CreateProcessWithBlockDllPolicy(IN LPSTR lpProcessPath, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {
    STARTUPINFOEXA siEx = {0};
    PROCESS_INFORMATION pi = {0};
    SIZE_T attrListSize = 0;

    if (lpProcessPath == NULL)
        return FALSE;

    siEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);
    siEx.StartupInfo.dwFlags = EXTENDED_STARTUPINFO_PRESENT;

    InitializeProcThreadAttributeList(NULL, 1, 0, &attrListSize);
    LPPROC_THREAD_ATTRIBUTE_LIST pAttrList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, attrListSize);
    if (pAttrList == NULL) {
        return FALSE;
    }

    if (!InitializeProcThreadAttributeList(pAttrList, 1, 0, &attrListSize)) {
        HeapFree(GetProcessHeap(), 0, pAttrList);
        return FALSE;
    }

    DWORD64 dwPolicy = PROCESS_CREATION_MITIGATION_POLICY_BLOCK_NON_MICROSOFT_BINARIES_ALWAYS_ON;
    if (!UpdateProcThreadAttribute(pAttrList, 0, PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY, &dwPolicy, sizeof(dwPolicy), NULL, NULL)) {
        DeleteProcThreadAttributeList(pAttrList);
        HeapFree(GetProcessHeap(), 0, pAttrList);
        return FALSE;
    }

    siEx.lpAttributeList = pAttrList;

    if (!CreateProcessA(
            NULL,
            lpProcessPath,
            NULL,
            NULL,
            FALSE,
            EXTENDED_STARTUPINFO_PRESENT,
            NULL,
            NULL,
            &siEx.StartupInfo,
            &pi))
    {
        DeleteProcThreadAttributeList(pAttrList);
        HeapFree(GetProcessHeap(), 0, pAttrList);
        return FALSE;
    }

    *dwProcessId = pi.dwProcessId;
    *hProcess = pi.hProcess;
    *hThread = pi.hThread;

    DeleteProcThreadAttributeList(pAttrList);
    HeapFree(GetProcessHeap(), 0, pAttrList);

    return TRUE;
}

int main(int argc, char* argv[]) {
    DWORD dwProcessId = 0;
    HANDLE hProcess = NULL, hThread = NULL;

#ifdef LOCAL_BLOCKDLLPOLICY
    if (argc == 2 && (strcmp(argv[1], STOP_ARG) == 0)) {
        // Процесс запущен с защитой, оставляем консоль открытой
        printf("[+] Process Is Now Protected With The Block Dll Policy \n");
        system("pause");
        WaitForSingleObject((HANDLE)-1, INFINITE);
    } else {
        // Перезапускаем себя с защитой
        char filename[MAX_PATH * 2] = {0};
        if (!GetModuleFileNameA(NULL, filename, sizeof(filename))) {
            
            system("pause");
            return -1;
        }
        
        size_t bufferSize = strlen(filename) + strlen(STOP_ARG) + 64;
        char* buffer = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);
        if (buffer == NULL) {
            system("pause");
            return -1;
        }

        sprintf_s(buffer, bufferSize, "%s %s", filename, STOP_ARG);

        if (!CreateProcessWithBlockDllPolicy(buffer, &dwProcessId, &hProcess, &hThread)) {
            HeapFree(GetProcessHeap(), 0, buffer);
            system("pause");
            return -1;
        }
        HeapFree(GetProcessHeap(), 0, buffer);
        system("pause");
    }
#endif

#ifndef LOCAL_BLOCKDLLPOLICY
    if (!CreateProcessWithBlockDllPolicy((LPSTR)"C:\\Windows\\System32\\RuntimeBroker.exe", &dwProcessId, &hProcess, &hThread)) {
        return -1;
    }
    system("pause");
#endif

    return 0;
}
