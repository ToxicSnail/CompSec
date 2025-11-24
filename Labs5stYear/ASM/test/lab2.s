main:
.LFB13:
	xor	ebx, ebx
.L7:
	mov	edi, ebx
	call	_Z9fibonaccii
	mov	BYTE PTR mask[rbx], al
	add	rbx, 1
	cmp	rbx, 69
	jne	.L7
	mov	eax, 0
.L8:
	movzx	edx, BYTE PTR flag[rax]
	xor	dl, BYTE PTR mask[rax]
	mov	BYTE PTR flag[rax], dl
	add	rax, 1
	cmp	rax, 69
	jne	.L8
	mov	edi, OFFSET FLAT:flag
	call	puts
	mov	eax, 0
	pop	rbx
	ret
.LFE13:
	.size	main, .-main
	.globl	mask
	.bss
	.align 32
	.type	mask, @object
	.size	mask, 69
mask:
	.zero	69
	.globl	flag
	.data
	.align 32
	.type	flag, @object
	.size	flag, 70
flag:
	.string	"fm`e97mns\020U:\362\333\032W\352^zm\t\372J\304\023s\006uE\201\030\3537\323\201\375\205\033\020\232\252U[\227\350\272m\203q\024\000\266\325\007\177\310\226\220\364\016\344m\352\333\331\245o\302."

