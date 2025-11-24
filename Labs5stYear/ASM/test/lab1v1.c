char[] flag = "q_vThZdLv@zQ{A~@d\\rRdJ";
short len = strlen(flag) / 2;

void entry()
{
    short cx = len;
    char* di = flag;
    short bx = 0x1337;
    
l:
    //si = di
    short ax = *(short*)(di)[0];    // loadsw
    
    ax ^= bx;
    *(short*)(di)[0] = ax;
    
    di++;
    cx -= 1; 
    if (cx != 0) goto l;    //loop @l

    printf("%s", flag);
}

//грязно и непонятно