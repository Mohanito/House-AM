#ifndef _PAGING_H
#define _PAGING_H
#include "types.h"
#include "lib.h"

#define dir_page_table 			4096
#define page_table_length       1024
#define fourkilobyte            4096
#define fourmegabyte          	0x400000
#define video_address         	0xB8000
#define video_address1         	0xB9000
#define video_address2         	0xBA000
#define video_address3          0xBB000
#define kernel					0x400000
#define enable_4megabyte_page   0x80
#define enable_entry            3
#define empty_value             0x0
#define video_page              184




uint32_t  page_dir[page_table_length]__attribute__((aligned(fourkilobyte)));
uint32_t  page_table[page_table_length]__attribute__((aligned(fourkilobyte)));
uint32_t  video_table[page_table_length]__attribute__((aligned(fourkilobyte)));
void page_fill();



#endif
