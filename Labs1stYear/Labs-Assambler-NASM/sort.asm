section .text
global _start

_start:
	xor esi, esi 		;
	xor edi, edi 		;
	xor eax, eax 		;зануляем регистры
	xor ebx, ebx 		;
	xor edx, edx 		;

	mov ebx, arr 		;Заносим адрес начала массива.
						;В этом регистре будет храниться адрес текущего элемента
	mov  eax, arr 		;Заносим адрес начала массива
	mov cl, [size] 	    ;Задаем количество элементов в массиве
	mov edx, [i]

	cmp_n_and_m:
		mov al, [i]
		mov bl, [size]
		cmp al, bl
		jb step_1
		jg exit

	step_1:
		mov edx, [i]
		mov ecx, [eax + edx*4]
		mov [x], ecx
		xor ecx, ecx
		mov [j], edx
		xor edx, edx
		sub [j], byte 1
		jmp cmp_2_1

	cmp_2_1:
		cmp [j], byte 0
		jge cmp_2_2 
		jb step_3
	cmp_2_2:
		mov edx, [j]
		cmp ecx, [eax + edx*4]
		mov [x], ecx
		xor edx, edx
		xor ecx, ecx
		jg step_2
		jb step_3

	step_2:
		mov edx, [j]
		mov ecx, [eax + edx*4]
		xor edx, edx
		mov edx, [j+1]
		mov [eax + edx*4], ecx
		xor edx, edx
		mov edx, [j]
		sub edx, 1
		mov [j], edx
		jmp cmp_2_1

	step_3:
		mov edx, [j+1]
		mov ecx, [x]
		mov [eax + edx*4], ecx
		xor ecx, ecx
		xor edx, edx
		mov edx, [i]
		inc edx
		mov [i], edx
		xor edx, edx
		jmp cmp_n_and_m

;;;;ТУТ ДОЛЖЕН БЫТЬ ВЫВОД!!!

section .data
    arr dd '5','4','3','2','1','0'
    msgErr db 'multidigit answer';вывод сообщения о многозначном выводе
    lenErr equ $ - msgErr        ;длина сообщения о многозначном выводе
    size dd 6
    i dd 1
    x db 0
    j dd 0
    temp db 0
    simvol    dd 0 
    separator db ',' 