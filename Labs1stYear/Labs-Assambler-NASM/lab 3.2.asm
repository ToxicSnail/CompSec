global _start   
segment .bss
  n resb 4                                      ;введенное число n, в которое будем возводить m
  m resb 4                                      ;введенное число m, которое будет являться степень числа n
                                                ;в лабораторной работе 3_1 использовал много ненужных переменных и строк(res, temp, ...)


segment .data 
  msg db "Переполнение ", 0xA,0xD               ;вывод при переполнении
  len equ $- msg                                ;длина строки для вывода при переполнении
  
  msg_out db 'Output: '                         ;вывод вывода
  len_out equ $ - msg_out                       ;длина строки для вывода
  
  msgErr db 'Error in input'                    ;вывод для некорректного ввода
  lenErr equ $ -msgErr                          ;длина некорректного вывода



section .text
_start:

    xor esi, esi                                ;обнуляем регистр esi
    xor edi, edi
    
      enter_n:
        mov eax, 3                              ;
        mov ebx, 0                              ;
        mov ecx, n                              ;ввод 1 числа(первая цифра) - n
        mov edx, 1                              ;
        int 0x80                                ;
        
        mov eax, [n]                            ;приравнивем регистр eax значению n
        
        cmp eax, byte 32                        ;если у нас число заканчивается и следующий символ равен
        jz cov_n                                ;символу пробела в аски, то прыгаем конвертацию числа из стека
             
        cmp eax, 57                             ;сравниваем значение в регистре eax с 57
        jg error_input                          ;прыгаем на ошибку ввода если eax больше 57 
             
        cmp eax, 48                             ;сравниваем значение в регистре eax с 48
        jl error_input                          ;прыгаем на ошибку ввода, если eax меньше 48
             
        sub eax, 48                             ;отнимаем 48 для конвертации из ASCII в десятичное число
        push eax                                ;добовляем в стек
        inc esi                                 ;увеличиваем значение регистра esi на 1, используя esi, как лину числа
        cmp esi, 8                              ;будем считать переполнением, если у нас длина n, 
        jg error                                ;то будем считать и прыгать на переполнение
        jmp enter_n                             ;прыгаем на enter_n, если до этого не выходили с ошибкой
         
         
         
      cov_n:
          xor eax, eax                          ;обнуляем eax
          mov ecx, 0                            ;обнуляем ecx 
          converter_n:
             pop edx                            ;выносим из стека в edx значение
             mov ebx, ecx                       ;переносим значение из ecx в ebx
             cmp ecx, 0                         ;сравниваем значение в регистре ecx с 0
             je next1                           ;прыгаем на next1 если ecx равно 0 
             power1:
                imul edx, 10                    ;умнажаем регистр edx на 10
                dec ebx                         ;уменьшаем на 1 регистр ebx
                cmp ebx, 0                      ;сравниваем ebx с 0
                jg power1                       ;прыгаем на power1 если ebx больше 0
             next1:
                 add eax, edx                   ;прибавляем к eax edx
                 inc ecx                        ;увеличиваем ecx на 1
                 cmp ecx, esi                   ;сравниваем ecx и esi
                 jl converter_n                 ;прыгаем на converter1 если ecx меньше esi
             
      
      mov [n], eax                              ;перемещаем значение из eax в num1
      mov esi, 0                                ;
      xor eax, eax                              ;зануляем регистры, чтобы избавиться от мусора
      xor ecx, ecx                              ;
    
      enter_m:
        mov eax, 3                              ;
        mov ebx, 0                              ;
        mov ecx, m                              ;ввод 2 числа(первая цифра) - m
        mov edx, 1                              ;
        int 0x80                                ;
         
        mov eax, [m]                            ;приравнивем регистр eax значению num2

        cmp eax, byte 10                        ;если у нас число закончилось и последующий символ равен символу переноса на новую строку(конец строки)
        jz cov_m                                ;ты прыгаем на конвертация числа в регистр
         
        cmp eax, 48                             ;сравниваем значение в регистре eax с 48
        jl error_input                          ;если символ не число, то прыгаем на ошибку ввода
         
        cmp eax, 57                             ;сравниваем значение в регистре eax с 57
        jg error_input                          ;если символ не число, то прыгаем на ошибку ввода 
    
        sub eax, 48                             ;отнимаем 48 для конвертации из ASCII в десятичное число
        push eax                                ;добовляем в стек
        inc esi                                 ;увеличиваем значение регистра esi на 1
        cmp esi, 8                              ;проверяем на переполнение и прыгаем, если переполнение
        jg error
        jmp enter_m                             ;прыгаем на enter_m
         
      cov_m:
          xor eax, eax                          ;обнуляем eax
          mov ecx, 0                            ; обнуляем ecx
          converter_m:
             pop edx                            ; выносим из стека в edx значение
             mov ebx, ecx                       ; переносим значение из ecx в ebx
             cmp ecx, 0                         ;сравниваем значение в регистре ecx с 0
             je next2                           ; прыгаем на next2 если ecx равно 0 
             power2:
                imul edx, 10                    ; умнажаем регистр edx на 10
                dec ebx                         ; уменьшаем на 1 регистр ebx
                cmp ebx, 0                      ;   сравниваем ebx с 0
                jg power2                       ; прыгаем на power2 если ebx больше 0
                
             next2:
                 add eax, edx                   ; прибавляем к eax edx
                 inc ecx                        ; увеличиваем ecx на 1
                 cmp ecx, esi                   ; сравниваем ecx и esi
                 jl converter_m                 ; прыгаем на converter2 если ecx меньше esi
      
    
      mov [m], eax                              ;перемещаем значение из eax в num2 
    
      while_start:
          mov edx, 1                            ;перемещаем в edx 1
          mov ecx, [m]                          ;перемещаем в ecx значение num2
          mov eax, 1                            ;перемещаем в eax 1
          cmp ecx, 0                            ;сравниваем ecx с 0
          je zero_pow                           ;прыгаем на zero если ecx равно 0
          loop_start:
             mov ebx, [n]                       ;перемещаем в ebx значение num1
             cmp ebx, 0
             je zero
             cmp ebx, 1
             je zero_pow
             loop_start_2:
                add edx, eax                    ;прибавляем к edx, eax
                jo error                        ;прыгаем при переполнении
                dec ebx                         ;уменьшаем ebx на 1
                cmp ebx, 1                      ;сравниваем ebx с 1
                jg loop_start_2                 ;прыгаем на loop_start_2 если ebx больше 1
              mov eax, edx                      ;перемещаем в eax edx
              dec ecx                           ;уменьшаем ecx на 1
              cmp ecx, 0                        ;сравниваем ecx с 0
              jg loop_start                     ;прыгаем на loop_start если ecx больше 0
              jmp end_while                     ;прыгаем на end_finding
          
      zero:
          xor eax, eax
          xor esi, esi
    
      zero_pow:
        mov eax, '1'
        mov [n], eax
        mov eax, 4                              ;        
        mov ebx, 1                              ;
        mov ecx, n                              ;вывод единицы      
        mov edx, 1                              ;
        int 0x80                                ;
        jmp exit_Lab3_2
    
          
      end_while:
          xor esi, esi                          ;обнуляем esi
    
      div_stek:                                 ;производим разделение с занесением в стек
        xor edx, edx                            ;обнуляем edx
        mov ecx, 10                             ;перемещаем число 10 в регистр cx
        div ecx                                 ;делим eax на cx
        push edx                                ;записываем edx в стек
        inc esi                                 ;увеличиваем esi на 1
        cmp eax, 0                              ;сравниваем eax с 0
        jne div_stek                            ;если eax не равен 0, то прыгаем на div_stek
        
      cmp edi, 1                                ;если edi не равен 1, прыгаем на выход
      jne output_main
    
      output_main:
        mov eax, 4                              ;
        mov ebx, 1                              ;
        mov ecx, msg_out                        ;вывод сообщения "Output: "
        mov edx, len_out                        ;
        int 0x80                                ;
        jmp output
    
      output:  
          pop edx                               ;выносим из стека в edx значение
          add edx, 48                           ;прибавляем 48 для конвертации из десятичного числа в ASCII
                                                ;перещаем значение edx в переменную res2
          mov [m], edx    
          mov eax, 4                            ;        
          mov ebx, 1                            ;
          mov ecx, m                            ;вывод посимвольно результата из запопнутого стека      
          mov edx, 1                            ;
          int 0x80                              ;

          dec esi                               ;уменьшаем значение регистра esi на 1
          cmp esi, 0                            ;сравниваем регистр esi с 0
          jg output                             ;прыгаем на output если esi больше 0
          jmp exit_Lab3_2
        
      error:
        mov eax, 4                              ;        
        mov ebx, 1                              ;
        mov ecx, msg                            ;вывод об ошибки переполнения     
        mov edx, len                            ;
        int 0x80                                ;
        
      error_input:                              ;
        mov eax, 4                              ;
        mov ebx, 1                              ;
        mov ecx, msgErr                         ;вывод о неверном вводе в программу
        mov edx, lenErr                         ;
        int 0x80                                ;
    
      exit_Lab3_2:    
         mov eax, 1                             ;номер системного вызова (sys_exit)
         xor ebx, ebx                           ;обнуляем ebx
         int 0x80                               ;вызов ядра