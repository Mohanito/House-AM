#include "paging.h"
#include "types.h"
/*
Funtion:page fill
Description: setup of pages for use
Input: NA
Output: NA
Side Effect: writes to both the page directory
and the page table itself
*/
void page_fill(){
  //empty value mask denotes these entries are
  //kernel level, can be read or written to, and pages are present
  memset(page_dir,empty_value,dir_page_table);
  memset(page_table,empty_value,dir_page_table);
  page_table[video_page] = ((uint32_t)video_address | enable_entry);
  page_table[185] = ((uint32_t)video_address1 | enable_entry);
  page_table[186] = ((uint32_t)video_address2 | enable_entry);
  page_table[187] = ((uint32_t)video_address3 | enable_entry);  
  //video memory
  page_dir[0] = ((uint32_t)page_table | enable_entry);
  //second entry has 4mb page
  page_dir[1] = (kernel | enable_4megabyte_page);
  page_dir[1] |= enable_entry;
  asm volatile("pushl %eax \n"
			   "movl $page_dir,%eax \n"
               "movl %eax,%cr3 \n"
			   "movl %cr4,%eax \n"
	           "orl $0x00000010,%eax \n" 	//PSE_MASK 0x00000010
	           "movl %eax, %cr4 \n"
               "movl %cr0, %eax \n"
               "orl $0x80000001,%eax \n"	//PAGING_MASK 0x80000000
               "movl %eax,%cr0 \n"
               "popl %eax \n");


}
