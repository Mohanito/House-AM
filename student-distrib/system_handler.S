# interrupt_handler.S - Set up assembly linkage of IDT handlers
# vim:ts=4 noexpandtab

#define NEG_ONE  -1
#define MIN       1
#define MAX      10

.text
.globl 	system_call_handler,context_switch,flush_tlb,return_pt

system_call_handler:
	#pushl %ebp
	pushl %edi
	pushl %esi

	pushl %edx
	pushl %ecx
	pushl %ebx

	cmpl $MIN, %eax
	jl ERROR
	cmpl $MAX, %eax
	jg ERROR
	sti
	call *jump_table(,%eax,4)

	popl %ebx
	popl %ecx
	popl %edx

	popl %esi
	popl %edi
	#popl %ebp
	iret
ERROR:
	popl %ebx
	popl %ecx
	popl %edx

	popl %esi
	popl %edi
	#popl %ebp
	movl $NEG_ONE, %eax
	iret

jump_table:
	.long dummy_call, sys_halt, sys_execute, sys_read, sys_write, sys_open, sys_close, sys_getargs, sys_vidmap, sys_set_handler, sys_sigreturn

context_switch:
		pushl %ebp
		movl %esp, %ebp
		pushl %edi
		pushl %esi


		movl 8(%ebp), %ebx
		movl 12(%ebp), %ecx


		push	$0x002B

		PUSH %ebx 			#bottom of the 4 MB page already holding the executable image 132mb


		#EFLAGS
		pushf				#push eflags onto stack
		pop %eax			#pop it into eax
		orl $0x200, %eax	#mask the register to turn on IF
		pushl %eax			#push again

		push $0x0023

		#read file from file system (at the beginning of every file) GIVEN
		#the entry point from bytes 24-27 of the executable that you have just loaded.
		pushl %ecx			#entry_pt

		iret
return_pt:
		leave
		ret
flush_tlb:
		movl %cr3 , %eax
		movl %eax, %cr3
        ret
