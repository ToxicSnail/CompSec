#include <stdio.h>

int fibon(int arg)
{
    if (arg == 0)
        return 0;
    if (arg == 1)
        return 1;
    return fibon(arg - 1) + fibon(arg - 2);
}

void main()
{
    char mask[69] = {0};
    char flag = "fm`e97mns\020U:\362\333\032W\352^zm\t\372J\304\023s\006uE\201\030\3537\323\201\375\205\033\020\232\252U[\227\350\272m\203q\024\000\266\325\007\177\310\226\220\364\016\344m\352\333\331\245o\302.";

    for (int i = 0; i < 69; i++)
    {
        mask[i] = fibon(i) & 0xFF;  // 0xFF - бит
    }

    for (int i = 0; i < 69; i++)
    {
        flag[i] ^= mask[i];
    }

    puts(flag);
}
