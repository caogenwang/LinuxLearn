	.file	"alarm_t.c"
	.text
	.type	alrm_handler, @function
alrm_handler:
.LFB23:
	.cfi_startproc
	movl	$0, loop(%rip)
	ret
	.cfi_endproc
.LFE23:
	.size	alrm_handler, .-alrm_handler
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%lld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	call	alarm
	movl	$alrm_handler, %esi
	movl	$14, %edi
	call	signal
	movl	loop(%rip), %eax
	testl	%eax, %eax
	je	.L5
	movl	$0, %esi
.L4:
	addl	$1, %esi
	movl	loop(%rip), %eax
	testl	%eax, %eax
	jne	.L4
	jmp	.L3
.L5:
	movl	$0, %esi
.L3:
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.data
	.align 4
	.type	loop, @object
	.size	loop, 4
loop:
	.long	1
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
