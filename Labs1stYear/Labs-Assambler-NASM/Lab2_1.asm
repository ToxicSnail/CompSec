section .text

global _start

_start:

 mov eax, 3	;}
 mov ebx, 0	;}
 mov ecx, x	;} чтение переменной x с клавиатуры
 mov edx, 2	;}
 int 0x80	;}

 mov eax, 3	;}
 mov ebx, 0	;}
 mov ecx, y	;} чтение переменной y с клавиатуры
 mov edx, 2	;}
 int 0x80	;}

 mov eax, 3	;}
 mov ebx, 0	;}
 mov ecx, z	;} чтение переменной z с клавиатуры
 mov edx, 2	;}
 int 0x80	;}

 cmp [x], byte 48	;}
 jl error			;}
 cmp [x], byte 57	;} смотрим, лежит ли переменная x в диапазоне цифр из ASCII
 jg error			;}

 cmp [y], byte 48	;}
 jl error			;}
 cmp [y], byte 57	;} смотрим, лежит ли переменная y в диапазоне цифр из ASCII
 jg error			;}

 cmp [z], byte 48	;}
 jl error			;}
 cmp [z], byte 57	;} смотрим, лежит ли переменная z в диапазоне цифр из ASCII
 jg error			;}
 
 mov al, [x]	;}
 mov bl, [y]	;} заносим значения переменных x, y, z в младшие части регистров
 mov cl, [z]	;}

swap1:
 cmp al, bl		; сравниваем x и y (или x и z, если вернулись после swap2)
 jbe swap2		; если x ≤ y, прыгаем на сравнение y и z
 xchg al, bl	;
 mov [x], al	; иначе меняем значения в регистрах местами
 mov [y], bl	; и заносим их обратно в переменные

swap2:
 cmp bl, cl		; сравниваем переменную на втором месте (x или y) с z
 jbe output		; если она ≤ z, значит, всё уже стоит на своих местах
 xchg bl, cl	; и выполняем прыжок на вывод в консоль и выход из программы
 mov [z], cl	; 
 mov [y], bl	; иначе меняем значения в регистрах местами, заносим обратно
 jmp swap1		; и прыгаем на первый обмен 

output:
 mov ecx, msg	;}
 mov edx, len	;}
 mov ebx, 1		;} вывод сообщения в консоль 
 mov eax, 4		;} 
 int 0x80		;}

 mov ecx, x	; заносим в регистр адрес x
 mov edx, 2	; указываем длину выводимого сообщения
 mov ebx, 1	; файловый дескриптор (stdout) - вывод на консоль
 mov eax, 4	; номер системного вызова (sys_write)
 int 0x80	; вызов ядра

 mov ecx, y	;}
 mov edx, 2	;}
 mov ebx, 1	;} вывод значения переменной y в консоль
 mov eax, 4	;}
 int 0x80 	;}

 mov ecx, z	;}
 mov edx, 2	;}
 mov ebx, 1	;} вывод значения переменной z в консоль
 mov eax, 4	;}
 int 0x80	;}

 jmp exit	; прыжок на выход из программы

error:
 mov ecx, msgErr	;}	
 mov edx, lenErr	;}
 mov ebx, 1			;} вывод сообщения об ошибке в консоль
 mov eax, 4			;}
 int 0x80			;}

exit:
 mov eax, 0x01;}
 mov ebx, 0	  ;} выход из программы
 int 0x80	  ;}

section .data

msg db "Sorted numbers : "
len equ $ - msg
msgErr db "Bad input"
lenErr equ $ - msgErr
x db 0, 0
y db 0, 0
z db 0, 0
