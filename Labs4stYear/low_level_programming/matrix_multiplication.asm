section .data

    ROWS_A equ 3
    COLS_A equ 2

    ROWS_B equ 2
    COLS_B equ 2

    A dd 2, 3, 4, 5, 6, 7
    B dd 1, 0, 0, 1

    newline db 10      
    space   db ' '

section .bss
    result resd ROWS_A * COLS_B   ;Матрица результата (2x2)
    buffer resb 12 

section .text
    global _start

_start:
    mov esi, 0  ; Инициализация переменной i = 0

outer_loop_i:
    cmp esi, ROWS_A
    jge end_outer_loop_i

    mov edi, 0  ; Инициализация переменной j = 0

loop_j:
    cmp edi, COLS_B
    jge end_loop_j

    ; result[i][j] = 0
    mov eax, esi
    imul eax, COLS_B           ; eax = i * COLS_B
    add eax, edi               ; eax = i * COLS_B + j
    shl eax, 2                 ; Умножаем на 4 (размер int)
    mov ecx, eax               ; ecx хранит смещение для result[i][j]

    mov dword [result + ecx], 0

    mov ebp, 0   ; Используем ebp как счетчик k

loop_k:
    cmp ebp, COLS_A
    jge end_loop_k

    ; Вычисляем A[i][k] * B[k][j] и добавляем к result[i][j]
    ; Вычисление индекса для A[i][k]
    mov eax, esi
    imul eax, COLS_A           ; eax = i * COLS_A
    add eax, ebp               ; eax = i * COLS_A + k
    shl eax, 2                 ; Умножаем на 4
    mov edx, eax               ; edx хранит смещение для A[i][k]

    ; Загружаем A[i][k] в eax
    mov eax, [A + edx]

    ; Вычисление индекса для B[k][j]
    mov edx, ebp
    imul edx, COLS_B           ; edx = k * COLS_B
    add edx, edi               ; edx = k * COLS_B + j
    shl edx, 2                 ; Умножаем на 4

    ; Загружаем B[k][j] в ebx (временное хранение)
    mov ebx, [B + edx]

    ; Умножаем A[i][k] на B[k][j]
    imul eax, ebx              ; eax = A[i][k] * B[k][j]

    ; Добавляем к result[i][j]
    mov ebx, [result + ecx]
    add ebx, eax
    mov [result + ecx], ebx

    inc ebp ;k++
    jmp loop_k

end_loop_k:
    inc edi ;j++
    jmp loop_j

end_loop_j:
    inc esi ;i++
    jmp outer_loop_i

end_outer_loop_i:
    mov esi, 0  ; i = 0

print_loop_i:
    cmp esi, ROWS_A
    jge end_print_loop_i

    mov edi, 0  ; j = 0

print_loop_j:
    cmp edi, COLS_B
    jge end_print_loop_j

    ; Вычисляем индекс result[i][j]
    mov eax, esi
    imul eax, COLS_B
    add eax, edi
    shl eax, 2
    mov ecx, eax

    mov eax, [result + ecx]

    call print_num

    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

    inc edi    ;j++
    jmp print_loop_j

end_print_loop_j:
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    inc esi ;i++
    jmp print_loop_i

end_print_loop_i:
    mov eax, 1
    xor ebx, ebx
    int 0x80

print_num:
    push ebp
    push ebx
    push ecx
    push edx
    push esi

    mov esi, buffer + 11
    mov byte [esi], 0

    mov ebx, 10

    cmp eax, 0
    jne .print_num_loop
    dec esi
    mov byte [esi], '0'

    jmp .print_num_end

.print_num_loop:
    xor edx, edx
    div ebx
    dec esi
    add dl, '0'
    mov [esi], dl
    test eax, eax
    jnz .print_num_loop

.print_num_end:
    mov eax, 4
    mov ebx, 1
    mov ecx, esi
    mov edx, buffer + 11
    sub edx, esi
    int 0x80

    pop esi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret



;;;C[i][j] = sum(A[i][k] * B[k][j]), from k = 0, to (n - 1)
