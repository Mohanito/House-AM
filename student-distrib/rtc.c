#include "rtc.h"



/*
Funtion:rtc_init
Description: Initialization of Real-time Clock
Input: NA
Output: NA
Side Effect: enable irq line 8, turn on RTC in default mode 1024 Hz
*/
void rtc_init(){

	enable_irq(RTC_IRQ);

	/* Turn on IRQ 8 with the default 1024 Hz rate */
	outb(RTC_REG_B, RTC_SELECT_PORT);
	char regB = inb(RTC_RW_PORT);
	outb(RTC_REG_B, RTC_SELECT_PORT);
	outb(regB | 0x40, RTC_RW_PORT);	//regB OR 0x40 to turn on bit 6
	/*uint16_t buf[1];
  buf[0] = 16;
  rtc_write(0,(uint16_t*)&buf,0);
	*/
}
/*
Funtion:rtc_write
Description: Initialization of Real-time Clock
Input: file descriptor, buffer containing rate, and bytes
Output: 0
Side Effect: writes to RTC
*/
int32_t rtc_write(int32_t fd, uint16_t* buf, int32_t bytes){
	uint32_t clock_rate;
	//based on buffer value, create value we want to write to RTC
	switch(*buf){
		case 2:
			clock_rate = SET_2;
			break;
		case 4:
			clock_rate = SET_4;
			break;
		case 8:
			clock_rate = SET_8;
			break;
		case 16:
			clock_rate = SET_16;
			break;
		case 32:
			clock_rate = SET_32;
			break;
		case 64:
			clock_rate = SET_64;
			break;
		case 128:
			clock_rate = SET_128;
			break;
		case 256:
			clock_rate = SET_256;
			break;
		//case 512:
		//	clock_rate = SET_512;
		//	break;
		//case 1024:
		//	clock_rate = SET_1024;
		//	break;
		default:
			clock_rate = SET_2;
	}
	//prepping and writing clock rate to the rtc. This is very similar to the
	//rtc initialization done above in init()
	clock_rate &= 0x0F;
	outb(RTC_REG_A,RTC_SELECT_PORT);
	char previous = inb(RTC_RW_PORT);
	outb(RTC_REG_A,RTC_SELECT_PORT);
	outb(((previous & 0xF0)|clock_rate), RTC_RW_PORT);
	return 0;
}
/*
Funtion:rtc_open
Description: Initialization of Real-time Clock
Input: file name
Output: 0
Side Effect: calls init
*/
int32_t rtc_open(const uint8_t *file){
	rtc_init();
	return 0;
}
/*
Funtion:rtc_read
Description: Initialization of Real-time Clock
Input: file descriptor, buffer, number of bytes
Output: 0
Side Effect: prevents writes until interrupt flag is reset
*/
int32_t rtc_read(int32_t fd, uint8_t* buf, int32_t bytes){
	//simple interrupt set up
	int_flag = 1;
	while(int_flag);
	return 0;
}
/*
Funtion:rtc_close
Description: close?
Input: file descriptor
Output: 0
Side Effect: none, this literally just returns 0
*/
int32_t rtc_close(int32_t fd){
	return 0;
}
