SECTION .data
err_input db 'Нужно ввести число (натуральное).', 0xa
err_input_len equ $ - err_input

star db '*'

len equ 255
case_len equ 2

SECTION .bss
number resb len
digit resb 1

SECTION .text
global _start


isnumber:
    .Digit:
        ;проверяем посимвольно, обращаемся к элементам по индексу eax - 1
        movzx edx, byte [number + eax - 1] 
        cmp edx, '0' ;если не цифра - выводим сообщение об ошибке
        jb Err_num
        cmp edx, '9'
        ja Err_num

        dec eax
        cmp eax, 0
        jne .Digit
    ret

;превращает строку number длины есх в число в еах
Str_to_num:
    xor esi, esi
    .Digit:
        xor ebx, ebx
        mov bl, byte [number + esi] ;берем цифры по индексу esi
        mul ebp ;добавляем разряд

        sub bl, '0'
        add eax, ebx
        inc esi
        cmp esi, ecx
        jl .Digit

    cmp eax, 0              ;Если мы вдруг получили ноль
    je .Zero_input

    ret

    .Zero_input:

        mov eax, 1
        mov ebx, 0
        int 0x80

; Делим eax на ebx, пока не равно 1
Factorization:
    .process:
        xor edx, edx 
        cmp eax, 1   ;если число равно 1, то прекращаем
        je .process_end

        mov esi, eax ;сохраняем старое число
        div ebx      ;делим на множитель
        cmp edx, 0   ;Если остаток равен 0, то число поделилось, тогда запоминаем множитель
        je .divisor

        mov eax, esi ;если остаток не равен 0, возвращаем старое число и переключаем мн-ль
        inc ebx
        jmp .process

    .divisor:
        push eax
        call Print_factor
        pop eax
        mov ebx, 2 ;Новый перебор делителей (множителей)
        jmp .process

    .process_end:
        ret

;выводи множитель edx и star
Print_factor:
    xor esi, esi
    xor eax, eax

    mov eax, ebx
    xor ebx, ebx
    mov ebp, esp
    xor ecx, ecx

    .Factor:
        xor edx, edx
        mov ebx, 10 ;подбираем по цифре
        div ebx
        add edx, '0'
        push edx    ;пушим на стек цифры
        inc esi
        cmp eax, 0
        jne .Factor

    .Output:
        pop edx
        mov [digit], dl

        mov eax, 4
        mov ebx, 1
        mov ecx, digit
        mov edx, 1
        int 0x80

        dec esi
        cmp esi, 0
        jne .Output

    
    .Star:
        mov eax, 4
        mov ebx, 1
        mov ecx, star
        mov edx, 1
        int 0x80
            
    mov esp, ebp
    ret

;Если ввели не натуральное число
Err_num:
    mov eax, 4
    mov ebx, 1
    mov ecx, err_input
    mov edx, err_input_len
    int 0x80

    mov eax, 1
    mov ebx, 0
    int 0x80



_start:
    mov eax, 3 ;ввод числа
    mov ebx, 0
    mov ecx, number
    mov edx, len
    int 0x80

    dec eax         ;Попадаем с терминирующего нуля на строку
    push eax
    call isnumber   ;Если не натуральное число, то советуем ввести его
    pop eax

    mov ecx, eax    ;Перемещаем длину строки в есх (счётчик)
    xor eax, eax
    mov ebp, 10     ;Основание системы счисления ( 10^0 -> 10^1 -> ... -> 10^n )
    call Str_to_num ;Конвертируем строку в число
    mov ebx, 2      ;Два-наименьшее число (делитель)
    call Factorization

    mov eax, 1
    mov ebx, 0
    int 0x80