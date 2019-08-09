/* system_call_handler.h
 * vim:ts=4 noexpandtab
 */

#ifndef _SYSTEM_HANDLER_H
#define _SYSTEM_HANDLER_H_H

#include "system_call.h"
#include "x86_desc.h"
#ifndef ASM

/* declared in interrupt_handler.S */
extern void system_call_handler();
extern void context_switch(uint32_t page_start,uint32_t* entry_pt);
extern void flush_tlb();
#endif /* ASM */

#endif /* _SYSTEM_HANDLER_H_H */
