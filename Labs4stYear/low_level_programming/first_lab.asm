section .bss
    input_buffer resb 10       
    sum resb 1                 
    result_buffer resb 10      

section .text
    global _start

_start:
    mov eax, 3                 
    mov ebx, 0                
    mov ecx, input_buffer      
    mov edx, 10                
    int 0x80                   

    xor eax, eax               
    mov esi, input_buffer      
    mov byte [sum], 0          

sum_loop:
    lodsb
    cmp al, 0xA                
    je convert_to_string       
    sub al, '0'                
    add al, [sum]              
    mov [sum], al              
    jmp sum_loop               

convert_to_string:
    xor ecx, ecx               
    mov eax, [sum]
    mov edi, result_buffer

convert_loop:
    xor edx, edx               
    mov ebx, 10                
    div ebx
    add dl, '0'                
    dec edi                    
    mov [edi], dl
    inc ecx                    
    test eax, eax
    jnz convert_loop

print_result:
    mov eax, 4                 
    mov ebx, 1                 
    lea ecx, [edi]
    mov edx, ecx               
    sub edx, result_buffer     
    neg edx                    
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline           
    mov edx, 1
    int 0x80                   


    mov eax, 1                 
    xor ebx, ebx               
    int 0x80                   

section .data
    newline db 0xA            


;   nasm -f elf32 -o first_lab.o first_lab.asm; ld -m elf_i386 -o first_lab first_lab.o
;   ./first_lab
