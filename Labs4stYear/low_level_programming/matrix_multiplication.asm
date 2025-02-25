section .data
    n dd 3
    a dd 4
    b dd 7

    msg_ascend db "a -> b", 0
    msg_not_comparable db "uncomparable", 0
    newline db 0xA

section .bss
    result resd 1

section .text
    global _start

_start:
    mov esi, [a]
    mov edi, [b]

    mov ecx, [n]
    call are_comparable

    cmp eax, 1
    jne comparable

    mov eax, 4
    mov ebx, 1
    mov ecx, msg_not_comparable
    mov edx, 12
    int 0x80

    jmp exit

comparable:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_ascend
    mov edx, 6
    int 0x80

exit:

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80

are_comparable:
    mov edx, 0      ; 0 - сравнимы, 1 - несравнимы

    .check_bits:
        test esi, 1
        jz .next_bit_a      ; если 0, переходим к следующему биту
        test edi, 1         
        jnz .next_bit_a     ; если b[i] == 1, продолжаем

        ; Если a[i] == 1, а b[i] == 0, то несравнимы
        mov edx, 1         
        jmp .done_check_bits

    .next_bit_a:
        shr esi, 1 
        shr edi, 1         
        loop .check_bits

    .done_check_bits:
        mov eax, edx       ; возвращаем флаг
        ret
