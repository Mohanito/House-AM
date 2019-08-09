#ifndef _IDT_H
#define _IDT_H
#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "keyboard.h"
#include "system_handler.h"

#define SYSTEMCALLVECTOR    	128
#define RESERVED_FOR_VEC 		32

#define RTC_ENTRY				0x28
#define KEYBOARD_ENTRY			0x21
int tick_counter;

void set_idt();
void division_by_zero_int();
void single_step_int();
void NMI_int();
void breakpoint_int();
void overflow_int();
void bounds_int();
void invalid_opcode_int();
void coprocessor_unavailable_int();
void double_fault_int();
void coprocessor_segment_overrun_int();
void invalid_task_state_segment_int();
void segment_not_present_int();
void stack_fault_int();
void general_protection_fault_int();
void page_fault_int();
void math_fault_int();
void alignment_check_int();
void machine_check_int();
void simd_floating_point_exception_int();
void virtualization_exception_int();
void control_protection_exception_int();
void rtc_interrupt();
#endif
