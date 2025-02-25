section .data
    table_title db "x y", 10, 0
    format db "%.2lf %.6lf", 10, 0          ;x с точностью до 2 знаков, y — до 6 знаков
    h dq 0.05                                
    start_x dq 0.1                           
    end_x dq 1.1                             

section .bss
    result resq 1                            
    x_value resq 1                           
    one resq 1                               

section .text
    global _start
    extern printf, exp, pow                  

_start:
    ; Инициализация
    mov rdi, table_title                     
    call printf

    fld qword [start_x]                      
    fstp qword [x_value]                     ; Сохраняем его в x_value

.loop:
    ; Вычисляем 1/x
    fld1                                     
    fld qword [x_value]                      
    fdiv st1, st0                            ; 1 / x
    fstp qword [one]                         

    ; Вычисляем exp(1/x)
    mov rdi, one                             ; Передаём 1/x в exp
    call exp                                 ; Вызываем exp(1/x)
    fstp qword [result]                      

    ; Умножаем на x^2
    fld qword [x_value]                      
    fld st0                                  ; Дублируем x
    fmul st0, st1                            ; x^2
    fmul qword [result]                      ; x^2 * exp(1/x)
    fstp qword [result]                      

    ; Вывод значения в формате
    lea rdi, [format]                        ; Формат строки
    mov rsi, qword [x_value]                 
    mov rdx, qword [result]                  
    call printf                              

    ; Увеличиваем x на шаг h
    fld qword [x_value]                      
    fld qword [h]                            
    faddp st1, st0                           ; x + h
    fstp qword [x_value]                     

    ; Проверка выхода за пределы end_x
    fld qword [x_value]                      
    fld qword [end_x]                        
    fcomi st0, st1                           ; Сравниваем x и end_x
    jb .loop                                

    ; Завершение программы
    mov eax, 60                              
    xor edi, edi                             
    syscall
