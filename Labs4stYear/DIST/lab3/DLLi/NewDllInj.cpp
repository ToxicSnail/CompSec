#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, PVOID fImpLoad) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DWORD dwProcessId = GetCurrentProcessId(); // Получаем текущий PID
            HANDLE hFile = CreateFileA("D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\src\\hi.txt", 
                                       GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
                                       FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                std::string content = "Process ID: " + std::to_string(dwProcessId) + "\r\n";
                DWORD bytesWritten;
                WriteFile(hFile, content.c_str(), content.size(), &bytesWritten, NULL);
                CloseHandle(hFile);
            }
            break;
    }
    return TRUE;
}
