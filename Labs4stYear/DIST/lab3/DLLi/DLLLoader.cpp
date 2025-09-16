
#include <Windows.h>
#include <winerror.h>

int main()
{
    // You also specify the complete path.
    LoadLibrary(L"VirusDLL.dll");
    return 0;
}