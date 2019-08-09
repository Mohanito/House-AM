/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
/* ACTIVE LOW */
uint8_t master_mask = 0xFF; /* IRQs 0-7: 7 6 5 4 3 2 1 0(not sure) */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
	//Initialize all masks (may be unnecessary)
	outb(master_mask, MASTER_8259_PORT_DATA);
	outb(slave_mask, SLAVE_8259_PORT_DATA);
	
	//Send 4 initialization control words (ICWs) to the 8259A
	outb(ICW1, MASTER_8259_PORT_COMMAND);
	outb(ICW1, SLAVE_8259_PORT_COMMAND);
	outb(ICW2_MASTER, MASTER_8259_PORT_DATA);
	outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA);
	outb(ICW3_MASTER, MASTER_8259_PORT_DATA);
	outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA);
	outb(ICW4, MASTER_8259_PORT_DATA);
	outb(ICW4, SLAVE_8259_PORT_DATA);
	
	//Initialize all masks to 0xFF because nothing is connected for now
	outb(master_mask, MASTER_8259_PORT_DATA);
	outb(slave_mask, SLAVE_8259_PORT_DATA);
	
	enable_irq(SLAVE_IRQ);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
	uint8_t temp = 0xFE;
	int i = 0;	//for loop
	
	/* Changes one bit in Master/Slave mask to from 1 to 0 */
	if(irq_num >= 0 && irq_num <= 7){
		for(;i < irq_num; i++){
			temp = (temp << 1) + 1;
		}
		master_mask &= temp;
		outb(master_mask, MASTER_8259_PORT_DATA);
	}
	else if(irq_num >= 8 && irq_num <= 16){
		for(;i < (irq_num - 8); i++){
			temp = (temp << 1) + 1;
		}
		slave_mask &= temp;
		outb(slave_mask, SLAVE_8259_PORT_DATA);
	}
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
	
	uint8_t temp = 0x01;
	int i = 0;	//for loop
	
	/* Changes one bit in Master/Slave mask to from 0 to 1 */
	if(irq_num >= 0 && irq_num <= 7){
		for(;i < irq_num; i++){
			temp = (temp << 1);
		}
		master_mask |= temp;
		outb(master_mask, MASTER_8259_PORT_DATA);
	}
	else if(irq_num >= 8 && irq_num <= 16){
		for(;i < (irq_num - 8); i++){
			temp = (temp << 1);
		}
		slave_mask |= temp;
		outb(slave_mask, SLAVE_8259_PORT_DATA);
	}
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
	if(irq_num >= 0 && irq_num <= 7)
		outb(EOI | irq_num, MASTER_8259_PORT_COMMAND);		//Dont use data port (fixed by TA)
	else if(irq_num >= 8 && irq_num <= 15) {
		outb(EOI | (irq_num - 8), SLAVE_8259_PORT_COMMAND);
		outb(EOI | SLAVE_IRQ, MASTER_8259_PORT_COMMAND);
	}
}
