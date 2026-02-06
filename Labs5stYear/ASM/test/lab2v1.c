ebx = 0;

.L7:
    edi = ebx;  // аргумент для фибоначи
    _Z9fibonaccii();
    mask[rbx] = al;
    rbx++;

    if (rbx != 69) goto .L7
    eax = 0;

.L8:
    edx = flag[rax]
    dl ^= mask[rax]
    flag[rax] ^= dl
    rax++

    if (rax != 69) goto .L8

    edi = &flag
    puts();
    return 0;
