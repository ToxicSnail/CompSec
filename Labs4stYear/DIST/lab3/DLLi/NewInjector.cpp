#include <windows.h>
#include <iostream>

int main(int argc, char* argv[]) {
    HANDLE processHandle;
    PVOID remoteBuffer;
    wchar_t dllPath[] = L"D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\src\\NewDllInj.dll"; // Путь к DLL
    DWORD pid;
    
    std::cout << "Enter PID: ";
    std::cin >> pid;

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (processHandle == NULL) {
        std::cerr << "OpenProcess failed: " << GetLastError() << std::endl;
        return -1;
    }

    remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof(dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (remoteBuffer == NULL) {
        std::cerr << "VirtualAllocEx failed: " << GetLastError() << std::endl;
        CloseHandle(processHandle);
        return -1;
    }

    if (!WriteProcessMemory(processHandle, remoteBuffer, (LPVOID)dllPath, sizeof(dllPath), NULL)) {
        std::cerr << "WriteProcessMemory failed: " << GetLastError() << std::endl;
        VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return -1;
    }

    PTHREAD_START_ROUTINE threadStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
    if (threadStartRoutineAddress == NULL) {
        std::cerr << "GetProcAddress failed: " << GetLastError() << std::endl;
        VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return -1;
    }

    HANDLE threadHandle = CreateRemoteThread(processHandle, NULL, 0, threadStartRoutineAddress, remoteBuffer, 0, NULL);
    if (threadHandle == NULL) {
        std::cerr << "CreateRemoteThread failed: " << GetLastError() << std::endl;
        VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
        CloseHandle(processHandle);
        return -1;
    }

    std::cout << "DLL injected successfully!" << std::endl;

    WaitForSingleObject(threadHandle, INFINITE);

    CloseHandle(threadHandle);
    VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
    CloseHandle(processHandle);

    return 0;
}
