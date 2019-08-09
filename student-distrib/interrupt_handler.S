# interrupt_handler.S - Set up assembly linkage of IDT handlers
# vim:ts=4 noexpandtab

#define ASM     1

.text
.globl 	rtc_handler, keyboard_handler, page_fault_test

page_fault_test:
	addl $4, %esp
	iret

rtc_handler:
	#save registers and flags
	pusha
	#call handler
	call	rtc_interrupt
	#restore registers and flags
	popa
	iret

keyboard_handler:
	#save registers and flags
	pusha
	#call handler
	call	keyboard_interrupt
	#restore registers and flags
	popa
	iret
