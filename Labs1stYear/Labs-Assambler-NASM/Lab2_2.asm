section .text
global _start

_start:

    mov eax, 3              ;системный вызов ввода (sys_read)
    mov ebx, 0              ;дескриптор потока ввода
    mov ecx, x              ;переменная для ввода
    mov edx, 2              ;длина" переменной (сколько читаем). Пишем двойку, чтобы пробел захватить
    int 0x80                ;прерывание

    mov eax, 3              ;системный вызов ввода (sys_read)
    mov ebx, 0              ;дескриптор потока ввода
    mov ecx, y              ;переменная для ввода
    mov edx, 2              ;длина" переменной (сколько читаем). Пишем двойку, чтобы пробел захватить
    int 0x80                ;прерывание

    mov eax, 3              ;системный вызов ввода (sys_read)
    mov ebx, 0              ;дескриптор потока ввода
    mov ecx, z              ;переменная для ввода
    mov edx, 2              ;длина" переменной (сколько читаем). Пишем двойку, чтобы пробел захватить
    int 0x80                ;прерывание

    cmp [x], byte 48        ;сравниваем значение переменной с "нулем" в системе Аски
    jl error_input          ;если меньше прыгаем на функцию error_input
    cmp [x], byte 57        ;сравниваем значение переменной с "9" в системе Аски
    jg error_input          ;если больше, то прыгаем на функцию error_input    
    cmp [y], byte 48        ;сравниваем значение переменной с "нулем" в системе Аски
    jl error_input          ;если меньше прыгаем на функцию error_input
    cmp [y], byte 57        ;сравниваем значение переменной с "9" в системе Аски
    jg error_input          ;если больше, то прыгаем на функцию error_input
    cmp [z], byte 48        ;сравниваем значение переменной с "нулем" в системе Аски
    jl error_input          ;если меньше прыгаем на функцию error_input
    cmp [z], byte 57        ;сравниваем значение переменной с "9" в системе Аски
    jg error_input          ;если больше, то прыгаем на функцию error_input
 
    mov al, [x]             ;заносим значение х в подрегистр al
    mov bl, [y]             ;заносим значение y в подрегистр bl
    mov cl, [z]             ;заносим значение z в подрегистр cl
    
    
    
    
    cmp al, bl              ;сравниваем x и y
    jge comp1               ;если x >= y, то прыжок на comp1
    jb comp3                ;если x < y, то прыжок на comp1

comp1:
    cmp bl, cl              ;сравниваем y и z, при условии, что x > y
    jge exit1               ;если y >= z, то прыжок на exit1
    jb comp2                ;если y < z, то прыжок на comp2
    
    
exit1:
    add cl, 2               ;
    mov [z], cl             ;предустановка чисел при x > y > z,
    sub al, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [x], al             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;

comp2:
    cmp al, cl              ;сравниваем x и z, при условии что x > y и z > y
    jge exit2               ;если x >= z, то прыжок на exit2
    jb exit3                ;если x < z, то прыжок на exit3

exit2:
    add bl, 2               ;
    mov [y], bl             ;предустановка чисел при x > z > y
    sub al, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [x], al             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;

exit3:
    add bl, 2               ;
    mov [y], bl             ;предустановка чисел при z > x > y
    sub cl, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [z], cl             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;
    
comp3:
    cmp al, cl              ;сравниваем x и z, при условии что x < y
    jge exit4               ;если x >= z, то прыжок на exit4
    jb comp4                ;если x < z, то прыжок на comp4
    
exit4:
    add cl, 2               ;
    mov [z], cl             ;предустановка чисел при y > x > z
    sub bl, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [y], bl             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;

comp4:
    cmp bl, cl              ;сравниваем y и z, при условии что x < y и x < z
    jge exit5               ;если y >= z, то прыжок на exit5
    jb exit6                ;иначе прыжок на exit6

exit5:
    add al, 2               ;
    mov [x], al             ;предустановка чисел при y > z > x 
    sub bl, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [y], bl             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;
    
exit6:
    add al, 2               ;
    mov [x], al             ;предустановка чисел при z > y > x
    sub cl, 2               ;где наибольшее уменьшаем на 2, а наим увеличиваем на 2 
    mov [z], cl             ;заканчиваем с прыжком на exit, где конечный выход из программы
    jmp exit                ;
 

exit:
    mov ecx, msg            ;забор данных на вывод
    mov edx, len            ;длина текщего сообшения len
    mov ebx, 1              ;вывод сообщения в консоль
    mov eax, 4              ;системный вызов вывода
    int 0x80                ;прерывание
 
    mov ecx, x              ;переменная х для ввода
    mov edx, 2              ;указываем длину выводимого сообщения
    mov ebx, 1              ;файловый дескриптор (stdout) - вывод на консоль
    mov eax, 4              ;номер системного вызова (sys_write)
    int 0x80                ;прерывание

    mov ecx, y              ;переменная y для ввода
    mov edx, 2              ;указываем длину выводимого сообщения
    mov ebx, 1              ;вывод значения переменной y в консоль
    mov eax, 4              ;номер системного вызова (sys_write)
    int 0x80                ;прерывание

    mov ecx, z              ;переменная z для ввода
    mov edx, 2              ;указываем длину выводимого сообщения
    mov ebx, 1              ;вывод значения переменной z в консоль
    mov eax, 4              ;номер системного вызова (sys_write)
    int 0x80                ;прерывание
    jmp exit_2              ;прыжок на выход из программы

error_input:
    mov ecx, msgErr         ;забор данных на вывод    
    mov edx, lenErr         ;длина сообщения на вывод msgErr
    mov ebx, 1              ;вывод сообщения об ошибке в консоль
    mov eax, 4              ;системный вызов вывода
    int 0x80                ;прерывание

exit_2:
    mov eax, 0x01           ;нс в занос eax
    mov ebx, 0              ;зануляем регистр ebx
    int 0x80                ;прерывание

section .data
    msg db "Output: "
    len equ $ - msg
    msgErr db "Error in input"
    lenErr equ $ - msgErr
    
section .bss
    x resb 2
    y resb 2
    z resb 2