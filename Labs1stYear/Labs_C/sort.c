#include <stdio.h>

void main()
{
    int a = 0, b = 0, c = 0;
    scanf("%d%d%d", &a, &b, &c);
    if (a > b)
    {
        if (b > c)
        {
            a = a - 2;
            c = c + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - a; min - c
        }
        else if (a > c)
        {
            a = a - 2;
            b = b + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - c; min - b
        }
        else
        {
            c = c - 2;
            b = b + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - a; min - b
        }
    }
    else
    {
        if (a > c)
        {
            b = b - 2;
            c = c + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - b; min - c
        }
        else if (b < c)
        {
            c = c - 2;
            a = a + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - c; min - a
        }
        else
        {
            b = b - 2;
            a = a + 2;
            printf("%d%s%d%s%d", a, " ", b, " ", c); //max - b; min - a
        }
    }
}
