#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#define FILENAME_LEN 32
#define BLOCK_SIZE 4096
#define BLOCK_BYTES 0x1000
#define DIR_SIZE 64
#define DENTRY_RESERVED 24
#define BUF_MAX 32
#define BOOT_RESERVED 52


#include "types.h"
#include "system_call.h"
#include "lib.h"

typedef struct dentry{
  int8_t filename[FILENAME_LEN];
  int32_t filetype;
  int32_t inode_num;
  int8_t reserved[DENTRY_RESERVED];
}dentry_t;

typedef struct boot_block{
  int32_t dir_count;
  int32_t inode_count;
  int32_t data_count;
  int8_t reserved[BOOT_RESERVED];
  dentry_t direntries[DIR_SIZE-1]; //double check this
}boot_block_t;

typedef struct inode{
  int32_t length;
  int32_t data_block_num[(BLOCK_SIZE/4)-1];
}inode_t;

void fsystem_init(uint32_t fsystem_start); //initialize file system
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int32_t offset;
int32_t inode_count;
int32_t file_read(int32_t fd, uint8_t* buf, int32_t nbytes); //reads 'count' bytes of data from file into 'buf'
int32_t file_write(int32_t fd, uint8_t* buf, int32_t nbytes); //does nothing
int32_t file_open(const uint8_t* fname);  //initialize any temporary structures
int32_t file_close(int32_t fd); //undo what you did in open() function

int32_t dir_read(int32_t fd, uint8_t* buf, int32_t nbytes); //read files filename by filename, include "."
int32_t dir_write(int32_t fd, uint8_t* buf, int32_t nbytes); //does nothing
int32_t dir_open(const uint8_t* fname);  //opens a directory file (note file types)
int32_t dir_close(int32_t fd); //"probably does nothing"


#endif
