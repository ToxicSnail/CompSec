
#include <Windows.h>
#include <winerror.h>

#include <stdio.h>

int main()
{
    int i = 0;
    while (true)
    {
        printf("Processing - %d\n", i++);
        Sleep(1000);
    }
    return 0;
}