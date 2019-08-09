#include "IDT.h"
#include "interrupt_handler.h"
#include "rtc.h"
#include "keyboard.h"
//each of these functions are exception handlers that output output
//a message to the screen in the case their respective handle is called

void division_by_zero_int(){
	cli();
	printf("division by zero exception");
	while(1){
	}
}
void single_step_int(){
	cli();
	printf("single step exception");
	while(1){
	}
}

void NMI_int(){
	cli();
	printf("NMI exception");
	while(1){
	}
}
void breakpoint_int(){
	cli();
	printf("breakpoint exception");
	while(1){
	}
}

void overflow_int(){
	cli();
	printf("overflow exception");
	while(1){
	}
}

void bounds_int(){
	cli();
	printf("bounds exception");
	while(1){
	}
}

void invalid_opcode_int(){
	cli();
	printf("invalid opcode exception");
	while(1){
	}
}

void coprocessor_unavailable_int(){
	cli();
	printf("coprocessor unavailable exception");
	while(1){
	}
}

void double_fault_int(){
	cli();
	printf("double fault exception");
	while(1){
	}
}


void coprocessor_segment_overrun_int(){
	cli();
	printf("coprocessor segment overrun exception");
	while(1){
	}
}

void invalid_task_state_segment_int(){
	cli();
	printf("invalid task state segment exception");
	while(1){
	}
}

void segment_not_present_int(){
	cli();
	printf("segment not present exception");
	while(1){
	}
}

void stack_fault_int(){
	cli();
	printf("stack fault exception");
	while(1){
	}
}

void general_protection_fault_int(){
	cli();
	printf("general protection fault exception");
	while(1){
	}
}

void page_fault_int(){
	cli();
	printf("page fault exception");
	while(1){
	}
}

void math_fault_int(){
	cli();
	printf("math fault exception");
	while(1){
	}
}

void alignment_check_int(){
	cli();
	printf("alignment check exception");
	while(1){
	}
}

void machine_check_int(){
	cli();
	printf("machine check exception");
	while(1){
	}
}

void simd_floating_point_exception_int(){
	cli();
	printf("SIMD floating point exception");
	while(1){
	}
}

void virtualization_exception_int(){
	cli();
	printf("virtualization exception");
	while(1){
	}
}

void control_protection_exception_int(){
	cli();
	printf("control protection exception");
	while(1){
	}
}

void rtc_interrupt(){
	int_flag = 0;
	send_eoi(RTC_IRQ);
	//UNCOMMENT THIS LINE FOR RTC TEST
	//printf("%d \n",tick_counter);
	outb(RTC_REG_C, RTC_SELECT_PORT);	// select register C
  inw(RTC_RW_PORT);
	tick_counter++;
}

/*
*Function:set_idt();
*Description: initializes the IDT for use
*Inputs:NA
*Outputs:NA
*Side effects:modifies the contents of the idt array and calls
*the set_idt_entry function to set up exception handlers
*/

void set_idt(){
	int i;
	i = 0;
	while(i<NUM_VEC){
		//reserved entry in IDT, so skip
		if(i ==15){
			i++;
			continue;
		}
		idt[i].seg_selector = KERNEL_CS;
		idt[i].dpl = 0;
		idt[i].reserved4 = 0;
		idt[i].reserved3 = 0;
		idt[i].reserved2 = 1;
		idt[i].reserved1 = 1;
		idt[i].reserved0 = 0;
		idt[i].present = 1;
		idt[i].size = 1;
		//we're still filling in trap vectors
		if(i < RESERVED_FOR_VEC){
			idt[i].reserved3 = 1;
		}
		//pic stuff (i think?)
		if((i>=32)&&(i<=40)){
			idt[i].seg_selector = KERNEL_CS;
			idt[i].present = 1;
			idt[i].dpl = 0;
			idt[i].size = 1;
			idt[i].reserved3 = 1;
			idt[i].reserved2 = 1;
			idt[i].reserved1 = 1;
			idt[i].reserved0 = 0;
			idt[1].reserved4 = 0;

		}
		//special case for the system call entery (0x80)
		if(i == SYSTEMCALLVECTOR){
			idt[i].dpl = 3;
			idt[i].reserved3 = 1;
			idt[i].reserved2 = 1;
			idt[i].reserved1 = 1;
			idt[i].size = 1;
		}
		i++;
	}
	//filling in entries of IDT using set_idt_entry function
	//if these exceptions are encountered, they will call the
	//functions defined in the top of the file
	SET_IDT_ENTRY(idt[0],division_by_zero_int);
	SET_IDT_ENTRY(idt[1],single_step_int);
	SET_IDT_ENTRY(idt[2],NMI_int);
	SET_IDT_ENTRY(idt[3],breakpoint_int);
	SET_IDT_ENTRY(idt[4],overflow_int);
	SET_IDT_ENTRY(idt[5],bounds_int);
	SET_IDT_ENTRY(idt[6],invalid_opcode_int);
	SET_IDT_ENTRY(idt[7],coprocessor_unavailable_int);
	SET_IDT_ENTRY(idt[8],double_fault_int);
	SET_IDT_ENTRY(idt[9],coprocessor_segment_overrun_int);
	SET_IDT_ENTRY(idt[10],invalid_task_state_segment_int);
	SET_IDT_ENTRY(idt[11],segment_not_present_int);
	SET_IDT_ENTRY(idt[12],stack_fault_int);
	SET_IDT_ENTRY(idt[13],general_protection_fault_int);
	SET_IDT_ENTRY(idt[14],page_fault_int);
	SET_IDT_ENTRY(idt[16],math_fault_int);
	SET_IDT_ENTRY(idt[17],alignment_check_int);
	SET_IDT_ENTRY(idt[18],machine_check_int);
	SET_IDT_ENTRY(idt[19],simd_floating_point_exception_int);
	SET_IDT_ENTRY(idt[20],virtualization_exception_int);
	SET_IDT_ENTRY(idt[21],control_protection_exception_int);
	//rtc_handler and keyboard_handler are defined in interrupt_handler.S
	SET_IDT_ENTRY(idt[RTC_ENTRY],rtc_handler);
	SET_IDT_ENTRY(idt[KEYBOARD_ENTRY],keyboard_handler);
	SET_IDT_ENTRY(idt[SYSTEMCALLVECTOR],system_call_handler);


}
