section .bss
    x resd 4
    y resd 4

section .data
    msg_x db 'X: ', 0
    msg_y db 'Y: ', 0
    msg_in db 'in', 0
    msg_out db 'out', 0
    newline db 0xA
    buffer db 32

section .text
    global _start

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_x
    mov edx, 3
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 32
    int 0x80

    call string_to_int
    mov [x], eax

    mov eax, 4
    mov ebx, 1
    mov ecx, msg_y
    mov edx, 3
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, buffer
    mov edx, 32
    int 0x80

    call string_to_int
    mov [y], eax

    mov eax, [x]
    cmp eax, 0 
    jle check_circle
    jmp check_triangle

check_circle:
    ;mov eax, [x]
    ;mov eax, [y]

    mov eax, [x]
    imul eax, eax
    mov ebx, [y]
    imul ebx, ebx
    add eax, ebx
    cmp eax, 4
    jle inside_region
    jmp outside_region

check_triangle:
    mov eax, [x]
    neg eax
    add eax, 2
    cmp [y], eax
    jg outside_region

    mov eax, [x]
    sub eax, 2
    cmp [y], eax
    jl outside_region

    jmp inside_region

inside_region:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_in
    mov edx, 3
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    jmp exit_program

outside_region:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_out
    mov edx, 4
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    jmp exit_program

exit_program:
    mov eax, 1
    xor ebx, ebx
    int 0x80

string_to_int:
    xor eax, eax
    xor ebx, ebx
    mov edx, 0

    mov bl, [ecx]
    cmp bl, '-'
    jne convert_loop
    mov edx, 1
    inc ecx

convert_loop:
    mov bl, [ecx]
    cmp bl, 0xA 
    je end_convert
    sub bl, '0'
    imul eax, eax, 10
    add eax, ebx
    inc ecx
    jmp convert_loop

end_convert:
    cmp edx, 1
    jne no_negative
    neg eax

no_negative:
    ret

;nasm -f elf32 -o simple_geometry.o simple_geometry.asm; ld -m elf_i386 -o simple_geometry simple_geometry.o
