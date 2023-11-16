global _start
section .bss
    n resd 2                       ;введенное число n, в которое будем возводить m
    m resb 1                       ;введенное число m, которое будет являться степень числа n
    res resb 1                     ;результат, которым мы проходим и считаем степень
    list_out resb 255              ;строка(массив) для вывода результата

section .data
    repet db 0                ; для внешнего цикла, чтобы пройтись m-1 раз
    res_len db 0              ; длина результата
    temp db 0                 ; временная переменная для хранения символа, который мы выведем
    
    msgErr db 'Error in input'      ;
    lenErr equ $ -msgErr            ;
    
    msg_out db 'Output: '        ;
    len_out equ $ - msg_out   ;
    
section .text
    _start:
     mov eax, 3                 ;
     mov ebx, 0                 ;
     mov ecx, n                 ; ввод n
     mov edx, 2                 ;
     int 0x80                   ;
    
     mov eax, 3                 ;
     mov ebx, 0                 ;
     mov ecx, m                 ;ввод m
     mov edx, 1                 ;
     int 0x80                   ;
     
     cmp [n], byte 48           ;сравниваем значение переменной с "нулем" в системе Аски
     jl error_input             ;если меньше прыгаем на функцию error_input
     cmp [n], byte 57           ;сравниваем значение переменной с "9" в системе Аски
     jg error_input             ;если больше, то прыгаем на функцию error_input    
     cmp [m], byte 48           ;сравниваем значение переменной с "нулем" в системе Аски
     jl error_input             ;если меньше прыгаем на функцию error_input
     cmp [m], byte 57           ;сравниваем значение переменной с "9" в системе Аски
     jg error_input             ;если больше, то прыгаем на функцию error_input
     
     mov al, [n]                ;
     mov bl, [m]                ;Вычитаем '0' в системе АСКИ, для того чтобы получить
     sub al, '0'                ;численное значение n и m
     sub bl, '0'                ;
     
     mov [res], al              ;заносим значение n в res
     mov [repet], bl            ;заносим значение m в repet для
     xor ecx, ecx                   ;указания кол-во проходов - итераций
     xor edx, edx               ;зануляем регистры ecx и edx
     
     while_start:                   ;внешний цикл while
         mov edx, [res]             ;заносим в регистр обновляемое значение результата
         cmp [repet], byte 1        ;постусловие на больше 1, после каждой итерации
         jle stop_loop_start        ;если меньше или равно, то прыгаем на прекращение while
         sub [repet], byte 1        ;иначе уменьшаем итерации на 1
         mov cl, al                 ;заводим счетчик для цикла for(loop)
         sub cl, byte 1             ;уменьшаем внешний счетчик
         loop_start:                ;цикл for
             add [res], edx         ;выполняем сложение
             loop loop_start        ;вызываем for столько раз сколько задали на итераций 
             jmp while_start        ;прыгаем на while пока не выполнится все итерации и мы не получим нужное нам res = n ** m
     stop_loop_start:               ;окончание 
        xor ecx, ecx                ;очищаем ecx
        xor edx, edx                ;очищаем edx
        mov eax, [res]              ;записываем наш результат в регистр eax
        mov ebx, 10                 ;в ebx заносим 10, чтобы делить на 10, и по цифре "отбивать" число
        
    stack_res:                      ;занесение в стек
        div ebx                     ;выполняем деление и заносим остаток в edx
        inc ecx                     ;считываение длину стека(числа), увеличением на 1
        mov [list_out + ecx], edx   ;заносим в массив остаток от дел. под индексом
        xor edx,edx                 ;зануляем edx, где хранился рез-т от деления
        cmp eax, 0                  ;проверка на то, что 
        je output                   ;
        jmp stack_res               ;в противном случае выполняем деление занесением в стек
        
    output:                         ; вывод 
        xor esi, esi                ; зануление регистра общего назначения
        mov esi, ecx                ;запись в esi ecx, где хранится длина нашего результата возведения
        
        mov eax, 4                  ;
        mov ebx, 1                  ;
        mov ecx, msg_out            ;вывод сообщения "Output: "
        mov edx, len_out            ;
        int 0x80                    ;
         
        xor eax, eax                ;зануление регистров после вывода сообщения
        xor ecx, ecx                ;
        
    loop_out:                       ; вывод из массива
        mov edx, [list_out + esi]   ;в list_out у нас хранится значение степени, обращаемся к последнему символу, так как у нас 
;число степени перевернуто, и мы выводим его с конца
        add edx, byte 48            ;добавляем к числу значение 0, чтобы привести к АСКИ
        mov [temp], edx             ;перемещаем в temp, чтобы вывести

        mov eax, 4                  ;
        mov ebx, 1                  ;
        mov ecx, temp               ;вывод цифры из массива
        mov edx, 1                  ;
        int 0x80                    ;
        
        cmp esi, 1                  ;если длина равна 1, то перепрыгиваем на выход из программы
        je exit_Lab3_1              ;
        dec esi                     ;иначе уменьшаем длину на 1
        
        jmp loop_out                ;после уменьшения прыгаем на повторный вывод числа из массива
        
    error_input:                    ;
        mov eax, 4                  ;
        mov ebx, 1                  ;
        mov ecx, msgErr             ;вывод о неверном вводе в программу
        mov edx, lenErr             ;
        int 0x80                    ;
     
    exit_Lab3_1:                    ;
        mov eax, 0x01               ;
        mov ebx, 0                  ;выход из нашей программы Lab3_1
        int 0x80                    ;