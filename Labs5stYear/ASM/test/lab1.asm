.model tiny
.code
org 100h

entry:  mov cx, len
        lea di, flag
        mov bx, 01337h
@l:     lodsw
        xor bx, ax
        stosw
        loop @l
        mov ah, 09h
        lea dx, flag
        int 21h
        ret
flag db "q_vThZdLv@zQ{A~@d\rRdJ"
db '$'
len dw ($-flag-1)/2
end entry
