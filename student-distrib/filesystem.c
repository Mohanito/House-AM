#include "filesystem.h"
#include "types.h"
#include "lib.h"

inode_t* inode_start;
uint32_t data_block_first;
boot_block_t* boot_block;
dentry_t holder;
dentry_t * holderptr = &holder;

//initialize file system
void fsystem_init(uint32_t file_start){
  boot_block = (boot_block_t*)file_start;
  //this is located one block after boot block
  inode_start = (inode_t*)(file_start + BLOCK_SIZE);
  //data block starts after inode start,but based off inode count in boot block
  data_block_first = file_start + (boot_block->inode_count)*BLOCK_SIZE + BLOCK_SIZE;
  offset = 0;
}

 /*
 *   FUNCTION:read_dentry_by_name
 *   Description: read directory entry by file name
 *	 INPUTS: filename and directory entry as pointers
 *   RETURN VALUE: -1 on failure, bytes_read on success
 *   SIDE EFFECTS: NA
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
  if(fname == NULL){
    return -1;
  }
  int filename_len = 32;
  uint32_t i;
  uint32_t dentry_count = boot_block->dir_count;
  for(i = 0; i < dentry_count; i++){
	int len;
	int8_t* s;
	len = 0;
	s = (int8_t*)boot_block->direntries[i].filename;
	while (s[len] != '\0'){
		len++;
		if(len == 32){
			break;
		}
	}
	if(len >32){
      len = 32;
    }
    if(strlen((int8_t*)fname) == len){
      filename_len = strlen((int8_t*)fname);
    } else {
      continue;
    }

    //compare fname and boot block filename
    //if they're equal in length
    //put that length into strncmp
    //if not, continue
    if(strncmp((int8_t*)fname, (int8_t*)boot_block->direntries[i].filename, len) == 0){
         //strncopy from lib.c, copies n bytes from source string to destination string
        strncpy((int8_t*)dentry->filename, (int8_t*)boot_block->direntries[i].filename, FILENAME_LEN);
        dentry->filetype = boot_block->direntries[i].filetype;
        dentry->inode_num = boot_block->direntries[i].inode_num;
        return 0;
    }
  }
  return -1;
}
/*   FUNCTION:read_dentry_by_index
 *   Description: read directory entry by index
 *	 INPUTS: filename and directory entry
 *   OUTPUT: -1 on failure, 0 on success
 *   SIDE EFFECTS: NA
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
  if(index >= boot_block->dir_count){
    return -1;
  }
  strncpy((int8_t*)dentry->filename, (int8_t*)boot_block->direntries[index].filename, FILENAME_LEN);
  dentry->filetype = boot_block->direntries[index].filetype;
  dentry->inode_num = boot_block->direntries[index].inode_num;
  return 0;
}
/*   FUNCTION:read_data
 *   Description: read directory entry by index
 *	 INPUTS: inode number, offset, buffer, and number of bytes to be read
 *   OUTPUT: -1 on failure, bytes read on success
 *   SIDE EFFECTS: modifies file pointer
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
  uint8_t copy_position;
  uint8_t* copy_pos_ptr = &copy_position;
  uint32_t dblock_begin;
  uint32_t dindex;
  int i;
  dblock_begin = offset/BLOCK_BYTES;
  dindex = offset % BLOCK_BYTES;
  //checking for invalid conditions
  if(inode_start[inode].data_block_num[dblock_begin] >= boot_block->data_count){
    return -1;
  }
  if(inode >= boot_block->inode_count){
    return -1;
  }
  copy_pos_ptr = (uint8_t *)(data_block_first + (inode_start[inode].data_block_num[dblock_begin])*BLOCK_SIZE + dindex);
  for(i = 0; i < length; i++)
  {
    //if index is located past block, move to the next one
    if(dindex >= BLOCK_SIZE)
    {
      dindex = 0;
      dblock_begin = dblock_begin + 1;
      //check if datablock outside the bound for number of data blocks
      if(inode_start[inode].data_block_num[dblock_begin] >= boot_block->data_count){
        return -1;
      }
      //update copy_pos_ptr based on next data block
      copy_pos_ptr = (uint8_t *)(data_block_first + (inode_start[inode].data_block_num[dblock_begin])*BLOCK_SIZE);
    }
    //end of file
    if(inode_start[inode].length <= i + offset)
    {
      return i;
    }
    buf[i] = *copy_pos_ptr;
    //increment counters
    dindex++;
    copy_pos_ptr++;
  }

  return i;
}
/* FUNCTION: file_write
 * DESCRIPTION: read the content of file specified by fd, store length of nbytes data into buf
 * INPUTS: file descriptor, buffer to write into, number of bytes to write into buffer
 * OUTPUTS: none
 */
int32_t file_read (int32_t fd, uint8_t* buf, int32_t nbytes){
  // reads count bytes of data from file into buf
  // uses read_data
  pcb_t* pcb = get_pcb();
  //initialize position to 0
  if((pcb->fd_arr[fd]).flags == 0){
    return -1;
  }
  int position = 0;
  //keep track of position based on bytes read
  position = read_data(pcb->fd_arr[fd].inode, pcb->fd_arr[fd].file_position, buf, nbytes);
  //update file_position
  pcb->fd_arr[fd].file_position = pcb->fd_arr[fd].file_position + position;
  if(position < 0)
  {
    return 0;
  }
  else
  {
    return position;
  }
}
/* FUNCTION:file_write
 * DESCRIPTION: write n-byte data from buf to file specified by fd (you cant)
 * INPUTS: file descriptor, buffer to write into, number of bytes to write into buffer
 * RETURN VALUE: returns -1
 */
int32_t file_write(int32_t fd, uint8_t* buf, int32_t nbytes)
{
    return -1;
}
/* FUNCTION:file_write
 * DESCRIPTION: write n-byte data from buf to file specified by fd (you cant)
 * INPUTS: file name
 * RETURN VALUE: returns 0
 * SIDE EFFECTS: stores Inode position for use in file_read
 */
int32_t file_open(const uint8_t* filename)
{
  if(filename == NULL){
    return -1;
  }
  if(read_dentry_by_name(filename,holderptr) == -1){
      return -1;
  }
  //saving inode position for later use
  inode_count = holderptr->inode_num;
  return 0;
}
/* FUNCTION:file_write
 * DESCRIPTION: does nothing
 * INPUTS: file descriptor
 * RETURN VALUE: returns 0
 */
int32_t file_close(int32_t fd)
{
    return 0;
}

/* FUNCTION:dir_read
 * DESCRIPTION: reads directory/data and loads it into buffer given
 * INPUTS: file descriptor, buffer, number of bytes
 * RETURN VALUE: returns -1 on fail, number of characters written to buffer on
 * success
 */
int32_t dir_read(int32_t fd, uint8_t* buf, int32_t nbytes){
  if((buf == NULL)||(nbytes < 0) || (fd < 0) || (fd > 7)){
    return -1;
  }
  //initialize variables
  int idx;
  int length = 0;
  //grab current PCB
  pcb_t* pcb = get_pcb();
  //grab file position from pcb to use as index
  idx = pcb->fd_arr[fd].file_position;
  pcb->fd_arr[fd].file_position += 1;
  //get dentry using idx
  dentry_t d_entry = boot_block->direntries[idx];
  if(read_dentry_by_index(idx, &d_entry) == -1)
  {
    return 0;
  }else{
    //loop to get length of filename
    while(d_entry.filename[length] != NULL && length < 32)
    {
      length++;
    }
    //copy filename into buffer
    strncpy((int8_t*)buf, d_entry.filename, 32);
    return length;
	}
}
/* FUNCTION:dir_write
 * DESCRIPTION: does nothing
 * INPUTS: file descriptor, buffer, number of bytes
 * RETURN VALUE: returns -1
 */
int32_t dir_write(int32_t fd, uint8_t* buf, int32_t nbytes)
{
    return -1;
}
/* FUNCTION:dir_open
 * DESCRIPTION: does nothing
 * INPUTS: file name
 * RETURN VALUE: returns 0
 */
int32_t dir_open(const uint8_t* filename)
{
  if(filename == NULL){
    return -1;
  }
  if(read_dentry_by_name(filename,holderptr) == -1){
      return -1;
  }
    return 0;
}
/* FUNCTION:dir_close
 * DESCRIPTION: does nothing
 * INPUTS: file descriptor
 * RETURN VALUE: returns 0
 */
int32_t dir_close(int32_t fd)
{
    return 0;
}
