#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "filesystem.h"
#include "rtc.h"
#include "IDT.h"
#include "types.h"
#include "keyboard.h"
#include "terminal.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	// launch your tests here
	//DIV BY ZERO EXCEPTION TEST
	//int a =5;
	//int b =0;
	//int c = a/b;
	//PAGING TEST
	//asm volatile(
	//	"int $14 \n"
	//);
	//int x = 0;
	//int* d = (int*)7340032;
	//int y = *(int*)d;
	//FILE_SYSTEM test 1
	/*
	uint8_t test_file[32] = "frame1.txt";
	test_file[32] = 0;
	file_open(test_file);
	uint8_t buffer[200];
	file_read(0,buffer,200);
	int i;
	for(i=0;i<200;i++){
		printf("%c", buffer[i]);
	}
	*/
	//FILE_SYSTEM test 2
	/*uint8_t test_file[32] = "frame0.txt";
	test_file[32] = 0;
	file_open(test_file);
	uint8_t buffer[200];
	file_read(0,buffer,20);
	int i;
	for(i=0;i<20;i++){
		printf("%c", buffer[i]);
	}
	printf ("second read on same file \n");
	test_file[32] = 0;
	file_read(0,buffer,40);
	for(i=0;i<40;i++){
		printf("%c", buffer[i]);
	}*/
	//FILE_SYSTEM test 3
    /*uint8_t test_file[34] = "verylargetextwithverylongname.tx";
	test_file[34] = 0;
	file_open(test_file);
	uint8_t buffer[200];
	file_read(0,buffer,200);
	int i;
	for(i=0;i<200;i++){
		printf("%c", buffer[i]);
	}*/
	//DIRECTORY_READ TEST
	/*
	clear();
	int32_t fd, cnt;
	uint8_t buf[33];
	while (0 != (cnt = dir_read (fd, buf, 3))) {
			if (-1 == cnt) {
				printf ("directory entry read failed\n");
				return 3;
		}
		buf[cnt] = '\n';
		buf[cnt+1] = 0;
		int i;
		printf("%s",buf);
	}
	*/
	//RTC_CLOCK tests
	/*tick_counter = 0;
	uint16_t buf[1];
	buf[0] = 16;
	rtc_write(32,buf,2);*/
	//TERMINAL READ and WRITE TEST
	//terminal_read and write is called in keyboard.c ENTER case
}
