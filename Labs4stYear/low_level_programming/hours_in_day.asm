section .bss
    N resb 10
    result resb 10

section .text
    global _start

_start:
    mov eax, 3
    mov ebx, 0
    mov ecx, N
    mov edx, 8
    int 0x80

convert_to_number:
    movzx eax, byte [ecx]
    cmp al, 0x0A
    je calculate_hours
    sub al, '0'
    imul ebx, ebx, 10
    add ebx, eax
    inc ecx
    jmp convert_to_number

calculate_hours:
    mov eax, ebx
    mov ecx, 3600
    mov edx, 0
    div ecx

    mov ecx, result
    add ecx, 7

convert_result:
    mov edx, 0
    mov ebx, 10
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz convert_result

    inc ecx

    mov edx, result + 8
    sub edx, ecx

print_result:
    mov eax, 4
    mov ebx, 1
    int 0x80

print_newline:
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, 1
    mov ebx, 0
    int 0x80

section .data
    newline db 10

;nasm -f elf32 -o hours_in_day.o hours_in_day.asm; ld -m elf_i386 -o hours_in_day hours_in_day.o
