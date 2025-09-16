section .data
    prompt_x db "Enter x (|x| < 1): ", 0
    prompt_epsilon db "Enter epsilon (> 0): ", 0
    error_x db "Error: |x| must be < 1.", 10, 0
    error_epsilon db "Error: epsilon must be > 0.", 10, 0
    msg_initial_left db "Initial left_part: ", 0
    msg_initial_right db "Initial right_part: ", 0
    msg_initial_diff db "Initial Difference: ", 0
    msg_iteration db "Iteration %d: Left part = %.10lf, Right part = %.10lf, Difference = %.10lf", 10, 0
    msg_final_result db "Final result:", 10, 0
    msg_final_left db "Left part (log(1 + x)): %.10lf", 10, 0
    msg_final_right db "Right part (series expansion): %.10lf", 10, 0
    msg_iterations db "Iterations: %d", 10, 0
    fmt db "%.10lf", 0

section .bss
    x resq 1
    epsilon resq 1
    left_part resq 1
    right_part resq 1
    term resq 1
    difference resq 1
    iteration_count resd 1
    

section .text
    global _start
    extern printf, scanf, log, fabs

_start:
    mov rdi, prompt_x
    call printf
    mov rdi, fmt
    mov rsi, x
    call scanf

    fld qword [x]
    fabs
    fld1    
    fcomip st(0), st(1)
    jae .error_x

    mov rdi, prompt_epsilon
    call printf
    mov rdi, fmt
    mov rsi, epsilon
    call scanf

    fld qword [epsilon]
    fldz 
    fcomip st(0), st(1)
    jbe .error_epsilon

    ; Вычисление точного значения log(1 + x) (left_part)
    fld qword [x]    ; Загружаем x
    fld1             
    fadd             
    fyl2x         
    fstp qword [left_part]  

    ; Инициализация начальных значений
    mov dword [iteration_count], 0
    fld qword [x]
    fstp qword [term]
    fld qword [term]
    fstp qword [right_part]
    fld qword [left_part]
    fld qword [right_part]
    fsub
    fabs
    fstp qword [difference]

    ; Вывод начальных значений
    mov rdi, msg_initial_left
    call printf
    mov rdi, fmt
    mov rsi, qword [left_part]
    call printf

    mov rdi, msg_initial_right
    call printf
    mov rdi, fmt
    mov rsi, qword [right_part]
    call printf

    mov rdi, msg_initial_diff
    call printf
    mov rdi, fmt
    mov rsi, qword [difference]
    call printf

    ; Итерационный процесс
iteration_loop:
    fld qword [difference]
    fld qword [epsilon]
    fcomip st(0), st(1)
    jb .end_loop

    ; Увеличение счетчика итераций
    inc dword [iteration_count]

    ; term *= -x * iteration_count / (iteration_count + 1)
    fld qword [term]
    fld qword [x]
    fmul
    mov eax, [iteration_count]
    cvtsi2sd xmm0, eax  ; Преобразуем счетчик в число с плавающей точкой
    mov eax, [iteration_count]
    inc eax
    cvtsi2sd xmm1, eax
    divsd xmm0, xmm1
    subsd xmm0, xmm0
    fmul st(0), st(1)
    fstp qword [term]

    ; right_part += term
    fld qword [right_part]
    fld qword [term]
    fadd
    fstp qword [right_part]

    ; difference = fabs(left_part - right_part)
    fld qword [left_part]
    fld qword [right_part]
    fsub
    fabs
    fstp qword [difference]

    ; Вывод промежуточных значений
    mov rdi, msg_iteration
    mov rsi, dword [iteration_count]
    mov rdx, qword [left_part]
    mov rcx, qword [right_part]
    mov r8, qword [difference]
    call printf

    jmp iteration_loop

.end_loop:
    ; Вывод итогов
    mov rdi, msg_final_result
    call printf

    mov rdi, msg_final_left
    mov rsi, qword [left_part]
    call printf

    mov rdi, msg_final_right
    mov rsi, qword [right_part]
    call printf

    mov rdi, msg_iterations
    mov rsi, dword [iteration_count]
    call printf

    mov rax, 60       ; syscall: exit
    xor rdi, rdi      
    syscall

.error_x:
    mov rdi, error_x
    call printf
    jmp .exit

.error_epsilon:
    mov rdi, error_epsilon
    call printf
    jmp .exit

.exit:
    mov rax, 60 
    xor rdi, rdi     
    syscall
