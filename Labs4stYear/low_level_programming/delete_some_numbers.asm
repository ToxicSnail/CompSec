section .data
    X dd 5, 3, 7, 3, 9, 3, 2, 3, 8, 3     
    len equ 10                            
    b dd 2                                
    newline db 10                             
    space db 32

section .bss
    result resd len                       
    buffer_for_output resb 5                        

section .text
    global _start

_start:
    mov esi, 0                            ; Индекс для исходного массива X
    mov edi, 0                            ; Индекс для массива результата

filter_loop:
    cmp esi, len                          ; Проверка на конец массива
    jge end_filter                        

    mov eax, [X + esi*4]                  
    cmp eax, [b]                          
    je skip_element                       

    mov [result + edi*4], eax             
    inc edi                               

skip_element:
    inc esi                              
    jmp filter_loop

end_filter:
    mov esi, 0                            

print_loop:
    cmp esi, edi                          
    jge end_print                         

    mov eax, [result + esi*4]             
    call print_num                        
    inc esi                               
    jmp print_loop

end_print:
    mov eax, 1                            
    mov ebx, 0
    int 0x80

print_num:
    push edi                           
    push ebx                           

    mov ebx, eax

    ; Конвертируем число в строку
    mov ecx, 10                        
    mov edi, buffer_for_output + 4    
    mov byte [edi], 0                 

print_digit:
    dec edi
    xor edx, edx
    div ecx                            
    add dl, '0'                        
    mov [edi], dl                      
    test eax, eax                      
    jnz print_digit

    mov eax, 4                         
    mov ebx, 1                         
    mov ecx, edi                       
    mov edx, buffer_for_output + 4     
    sub edx, edi                       ; Перерасчет длины
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    pop ebx                            
    pop edi                            
    ret
