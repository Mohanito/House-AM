#ifndef _RTC_H
#define _RTC_H

#include "i8259.h"
#include "lib.h"

/* RTC has a low priority: irq_num = 8 */
#define RTC_IRQ 				8
/* Port 0x70 is used to specify an index or "register number", and to disable NMI */
#define RTC_SELECT_PORT			0x70
/* Port 0x71 is used to read or write from/to that byte of CMOS configuration space */
#define RTC_RW_PORT				0x71

/* RTC Status Register A, B, and C */
#define RTC_REG_A				0x8A
#define RTC_REG_B				0x8B
#define RTC_REG_C				0x8C

#define HZ_2     2
#define HZ_4     4
#define HZ_8     8
#define HZ_16    16
#define HZ_32    32
#define HZ_64    64
#define HZ_128   128
#define HZ_256   256
#define HZ_512   512
#define HZ_1024  1024

#define SET_2    15
#define SET_4    14
#define SET_8    13
#define SET_16   12
#define SET_32   11
#define SET_64   10
#define SET_128  9
#define SET_256  8
#define SET_512  7
#define SET_1024 6

volatile int32_t int_flag;

void rtc_init();

int32_t rtc_open(const uint8_t *file);
int32_t rtc_read(int32_t fd, uint8_t* buf, int32_t bytes);
int32_t rtc_write(int32_t fd, uint16_t* buf, int32_t bytes);
int32_t rtc_close(int32_t fd);



#endif
