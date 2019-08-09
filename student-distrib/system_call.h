#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H
#include "types.h"
#include "system_handler.h"
#include "terminal.h"
#include "keyboard.h"

#define PCB_LENGTH  8
#define BUFFER_SIZE  128
#define ONETWOEIGHTMB  0x8000000
#define EIGHTMB      0x800000
#define EIGHTKB      0x2000
#define LENGTH       4
#define bit_mask     0x007FE000
#define PAGE_START   0x83FFFFC
#define VIRTUAL_ADDR 0x08048000
#define VIDEO_PAGE   0xB8000
#define ARG_BUFFER_SIZE 1000
#define SET_ENTRY    0x87
#define HEADER_BUF_SIZE 40
#define ELF_ONE      0x7F
#define ELF_TWO      0x45
#define ELF_THREE      0x4C
#define ELF_FOUR      0x46
#define OFFSET        24
#define READ_SIZE     100
#define PAGE_VALUE    32
#define OR_SIX        6
#define OR_SEVEN      7


typedef int32_t (*read_fn)(int32_t fd, uint8_t* buf, int32_t nbytes);
typedef int32_t (*write_fn)(int32_t fd,const uint8_t* buf, int32_t nbytes);
typedef int32_t (*open_fn)(const uint8_t* filename);
typedef int32_t (*close_fn)(int32_t fd);
//file operations table structure used in fd_array block
typedef struct file_ops{
  read_fn  * read_function;
  write_fn * write_function;
  open_fn  * open_function;
  close_fn * close_function;
}file_ops_t;
//file_descriptor array block structure
typedef struct fd_array{
  int32_t flags;
  int32_t inode;
  int32_t file_position;
  file_ops_t*  file_operations;
}fd_array_t;
//Pcb structure
typedef struct pcb{
  fd_array_t fd_arr[PCB_LENGTH];
  uint8_t argument[BUFFER_SIZE];
  int32_t proc_num;
  int32_t parent_proc_num;
  int32_t ebp;
  int32_t esp;
  int32_t terminal_num;
}pcb_t;


pcb_t * get_pcb();
extern void sys_init();
extern int32_t sys_halt (uint8_t status);
extern int32_t sys_execute (const uint8_t * command);
extern int32_t sys_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t sys_write(int32_t fd,const void* buf, int32_t nbytes);
extern int32_t sys_getargs(uint8_t* buf, int32_t nbytes);
extern int32_t sys_open(const uint8_t* filename);
extern int32_t sys_close(int32_t fd);
extern int32_t sys_vidmap(uint8_t** screen_start);
extern int32_t sys_set_handler (int32_t signum, void* handler_address);
extern int32_t sys_sigreturn (void);
extern int32_t sys_dummy_call();

#endif
