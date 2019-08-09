/* interrupt_handler.h
 * vim:ts=4 noexpandtab
 */

#ifndef _INTERRUPT_HANDLER_H
#define _INTERRUPT_HANDLER_H

#include "IDT.h"

#ifndef ASM

/* declared in interrupt_handler.S */
extern void rtc_handler();
extern void keyboard_handler();

void page_fault_test();


#endif /* ASM */

#endif /* _INTERRUPT_HANDLER_H */
