int result;

proctype fact(int n)
{
    int i = 2;
    result = 1;
    do
    :: (i <= n) -> result = result * i; i++
    :: else -> break
    od;
    printf("fact(%d) = %d\n", n, result)
}

init { run fact(5); }