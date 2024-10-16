section .bss
    m resd 1
    n resd 1
    gcd_value resd 1
    divisor_value resd 1
    buffer resb 12

section .data
    newline db 10
    space_dash db ' -', 0
    input_m db 'm: ', 0
    input_n db 'n: ', 0
    gcd_msg db 'GCD: ', 0
    div_msg db 'Divisors: ', 0

section .text
    global _start

_start:
    mov eax, 4
    mov ebx, 1
    mov ecx, input_m
    mov edx, 3
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, m
    mov edx, 10
    int 0x80

    mov ecx, m
    call str_to_num_with_sign
    test eax, eax
    jge .skip_neg_m
    neg eax
.skip_neg_m:
    mov [m], eax

    mov eax, 4
    mov ebx, 1
    mov ecx, input_n
    mov edx, 3
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, n
    mov edx, 10
    int 0x80

    mov ecx, n
    call str_to_num_with_sign
    test eax, eax
    jge .skip_neg_n
    neg eax
.skip_neg_n:
    mov [n], eax

    mov eax, [m]
    mov ebx, [n]
    call gcd
    mov [gcd_value], eax

    mov eax, 4
    mov ebx, 1
    mov ecx, gcd_msg
    mov edx, 5
    int 0x80

    mov eax, [gcd_value]
    call print_number

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, div_msg
    mov edx, 10
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    mov eax, [gcd_value]
    call print_divisors

    mov eax, 1
    xor ebx, ebx
    int 0x80

gcd:
    push ebx
.loop:
    test ebx, ebx
    jz .done
    xor edx, edx
    div ebx
    mov eax, ebx
    mov ebx, edx
    jmp .loop
.done:
    pop ebx
    ret

print_divisors:
    push eax
    mov eax, [gcd_value]
    mov ebx, eax

    mov dword [divisor_value], 1

.div_loop:
    mov eax, [divisor_value]
    cmp eax, [gcd_value]
    jg .done_divisors

    mov eax, [gcd_value]
    cdq
    div dword [divisor_value]
    test edx, edx
    jnz .next_div

    mov eax, [divisor_value]
    call print_number

    mov eax, 4
    mov ebx, 1
    mov ecx, space_dash
    mov edx, 2
    int 0x80

    mov eax, [divisor_value]
    call print_number

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

.next_div:
    inc dword [divisor_value]
    jmp .div_loop

.done_divisors:
    pop eax
    ret

str_to_num_with_sign:
    xor eax, eax
    xor edx, edx

    mov bl, [ecx]
    cmp bl, '-'
    jne convert_loop
    inc ecx
    inc edx

convert_loop:
    mov bl, [ecx]
    cmp bl, 10
    je convert_done
    cmp bl, 13
    je convert_done
    cmp bl, '0'
    jb invalid_input
    cmp bl, '9'
    ja invalid_input
    sub bl, '0'
    imul eax, eax, 10
    add eax, ebx
    inc ecx
    jmp convert_loop

convert_done:
    cmp edx, 1
    jne done_neg_check
    neg eax

done_neg_check:
    ret

invalid_input:
    mov eax, 1
    mov ebx, 1
    int 0x80

print_number:
    push eax
    mov ecx, buffer + 12
    mov ebx, 10

convert_loop_print:
    xor edx, edx
    div ebx
    add dl, '0'
    dec ecx
    mov [ecx], dl
    test eax, eax
    jnz convert_loop_print

    lea edi, [buffer + 12]
    sub edi, ecx

    mov esi, ecx

    mov eax, 4
    mov ebx, 1
    mov ecx, esi
    mov edx, edi
    int 0x80

    pop eax
    ret

;nasm -f elf32 -o divisors_numbers.o divisors_numbers.asm; ld -m elf_i386 -o divisors_numbers divisors_numbers.o
