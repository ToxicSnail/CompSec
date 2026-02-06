char[] flag = "q_vThZdLv@zQ{A~@d\\rRdJ";
short len = strlen(flag) / 2;

void main()
{
    short* di = (short*) flag;
  
    for (int i = 0; i < len; ++i)
    {
        // loadsw
        di[i] ^= 0x1337;
    }

    printf("%s", flag);

    return;
}

// чистый и лучше чем декомпилятор