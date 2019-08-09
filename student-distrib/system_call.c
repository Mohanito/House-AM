#include "system_call.h"
#include "filesystem.h"
#include "types.h"
#include "lib.h"
#include "paging.h"
#include "terminal.h"
#include "rtc.h"
#include "x86_desc.h"
#include "system_handler.h"

//global array for processes (2 currently)
int32_t processes[6] = {0,0,0,0,0,0};
file_ops_t std_operations;
file_ops_t dir_operations;
file_ops_t file_operations;
file_ops_t rtc_operations;
pcb_t new_pcb;
pcb_t * new_pcb_ptr = &new_pcb;

/*Function:get_pcb()
*Description:gets current pcb as a pointer by anding esp with a bit mask.
*No inputs or outputs or side effects
*/
pcb_t* get_pcb(){
  pcb_t* pcb_position;
  //bit mask the esp to get the current pcb
  asm volatile("andl %%esp, %%ebx"
              :"=b"(pcb_position)
              :"b"(bit_mask)
              );
  return pcb_position;
}
/*Function:sys_init()
*Description:sets up function jump tables for use in execute, read, write, open and close
* system calls
*No inputs or outputs, but side effects include filling in globally declared function
*jump table structs
*/
void sys_init(){
	std_operations.open_function = (open_fn*)&terminal_open;
	std_operations.close_function = (close_fn*)&terminal_close;
	std_operations.read_function = (read_fn*)&terminal_read;
	std_operations.write_function = (write_fn*)&terminal_write;
	dir_operations.open_function = (open_fn*)&dir_open;
	dir_operations.close_function = (close_fn*)&dir_close;
	dir_operations.read_function = (read_fn*)&dir_read;
	dir_operations.write_function =(write_fn*) &dir_write;
	file_operations.open_function = (open_fn*)&file_open;
	file_operations.close_function = (close_fn*)&file_close;
	file_operations.read_function = (read_fn*)&file_read;
	file_operations.write_function = (write_fn*)&file_write;
	rtc_operations.open_function = (open_fn*)&rtc_open;
	rtc_operations.close_function = (close_fn*)&rtc_close;
	rtc_operations.read_function = (read_fn*)&rtc_read;
	rtc_operations.write_function = (write_fn*)&rtc_write;
	new_pcb_ptr->proc_num = 0;
}
/*Function:sys_halt()
*Description:clears file descriptor array, among other things, jumps to parent process's
*stack using stored esp and ebp in pcb struct.
*Input is status, used as output after bitmask with 0xFF
*Side Effects:clears file descriptor array in current pcb, directly writes to esp and ebp
*registers, flushes TLB,etc.
*/
int32_t sys_halt (uint8_t status){
	cli();
	int32_t ret = (int32_t)(status & 0xFF); //0xFF is used as a mask
	//close process files
	pcb_t* pcb = get_pcb();
	if(pcb == NULL){
		return -1;
	}
	int file_array_idx;
	// close all files, max of 8 files in fd, set fd entry values to null
	for(file_array_idx = 0; file_array_idx < PCB_LENGTH; file_array_idx++) {
		pcb->fd_arr[file_array_idx].inode = -1;
		pcb->fd_arr[file_array_idx].flags = 0;
		pcb->fd_arr[file_array_idx].file_position = 0;
		pcb->fd_arr[file_array_idx].file_operations = NULL;
	}
	processes[pcb->proc_num] = 0;
	if(pcb->proc_num == 0) //if it's base shell
	{
		sys_execute((uint8_t*)"shell");
	}
	//restore ebp and esp
	int32_t esp, ebp;
	esp = pcb->esp;
	ebp = pcb->ebp;
	//restore paging, 32 was picked because why not
	page_dir[32] = ((pcb->parent_proc_num*(EIGHTMB/2)) + EIGHTMB) | SET_ENTRY;
	flush_tlb();
	tss.esp0 = EIGHTMB - (EIGHTKB * (pcb->parent_proc_num)) - 4;
	tss.ss0 = KERNEL_DS;
    //inline assembly to reload values of esp, ebp, and return value acquired from input
	sti();
	asm volatile(
	             "movl %0,%%eax \n"
		         "movl %1,%%esp \n"
		         "movl %2,%%ebp \n"
		         "jmp return_pt    \n"
				 :
				 :"r"(ret), "r"(esp),"r"(ebp)
				 :"eax");
	return ret;
}
/*Function:sys_execute()
*Description:Parses input for first word which is the command word, find the corresponding
*exectuable file, checks to make sure it is elf, then copies the program to a corresponding
*virtual address to be run, then configures the stack to have necessary data for and IRET call
*Input is command, required since its what we are trying to execute
*Side Effects:sets up stack for IRET, sets up paging, loads program file at virtual address offset, modifies TSS,etc.
*/
int32_t sys_execute (const uint8_t * command){
  if(command == NULL){
    return -1;
  }
  char arg_buffer[ARG_BUFFER_SIZE];
	uint8_t process_id;
	uint32_t null_check = 0;
	int i;
	for(process_id=0;process_id<6;process_id++){
		if(processes[process_id]==0){
      processes[process_id] = 1;
      break;
		}
	}
	//printf("%d \n",process_id);
	//processes are full
	if(process_id == 6){
		return -1;
	}
	pcb_t * proc_block = (pcb_t*)(EIGHTMB-EIGHTKB*(process_id+1));
	for(i=0;i<strlen((int8_t*)command);i++){
		if(command[i] != ' '){
			arg_buffer[i] = command[i];
			//putc(arg_buffer[i]);
		}else{
			arg_buffer[i] = NULL;
			null_check = 1;
			break;
		}
	}
	if(null_check == 0){
		arg_buffer[i] = NULL;
	}
	if(null_check !=0){
		int j;
	    i+=1;
		for(j=0;j<(strlen((int8_t*)command)-i);j++){
			proc_block->argument[j] = command[j+i];
		}
		//set last element as null just to be safe
		proc_block->argument[j] = NULL;
	}
	dentry_t holder_dentry;
	uint8_t header_buf[HEADER_BUF_SIZE];
	uint8_t* header_buf_ptr = header_buf;
	uint32_t length;
	uint32_t entry_pt;
	length = 4;
	//make sure our file is valid,and if so, read first 4 bytes of header to check if file is ELF
	if(read_dentry_by_name((uint8_t*)arg_buffer,&holder_dentry) !=0){
		return -1;
	}else{
		read_data(holder_dentry.inode_num,0,header_buf_ptr,HEADER_BUF_SIZE);
		if((header_buf[0] == ELF_ONE ) && (header_buf[1] == ELF_TWO ) && (header_buf[2] == ELF_THREE ) && (header_buf[3] == ELF_FOUR )){
			read_data(holder_dentry.inode_num,OFFSET,(uint8_t*)&entry_pt,length);

		}else{
			return -1;
		}
	}
	//setting up paging
	page_dir[PAGE_VALUE] = ((process_id*(EIGHTMB/2)) + EIGHTMB) | SET_ENTRY;
	int32_t save_value = page_dir[PAGE_VALUE];
	page_dir[PAGE_VALUE] = save_value;
	flush_tlb();
	//writing program image to memory
	int k,l,done_flag;
	done_flag = 0;
	k = read_data(holder_dentry.inode_num,0,(uint8_t*)VIRTUAL_ADDR,READ_SIZE);
	l = k;
	//this loop is based off the return value of read_data, since a return value of 0
	//means we are done
	while(k !=0 || done_flag != 1){
		k = read_data(holder_dentry.inode_num,l,(uint8_t*)(VIRTUAL_ADDR+l),READ_SIZE);
		if(k != READ_SIZE){
			done_flag = 1;
		}
		l+=k;
	}
  proc_block->terminal_num = current_terminal;
	//get info about parent process if applicable
	proc_block->proc_num = process_id;
	if(process_id == 0){

	}else{
		pcb_t* self_pcb = get_pcb();
		proc_block->parent_proc_num = self_pcb->proc_num;
	}
	//stuff required for setting up STD_IN and STD_OUT stuff (terminal read and write)
	proc_block->fd_arr[0].file_operations = &std_operations;
	proc_block->fd_arr[1].file_operations = &std_operations;
	//saving ebp and esp
	asm volatile("mov %%esp, %%eax \n\
                  mov %%ebp, %%ebx \n\
                 "
                 : "=a" (proc_block->esp), "=b" (proc_block->ebp)
    );

	//TSS stuff
	tss.esp0 = EIGHTMB-EIGHTKB*(process_id)-4;
	//assembly stuff
	context_switch(PAGE_START,(uint32_t*)entry_pt);
	return 0;
}
/*Function:sys_read()
*Description:Does necessary checks, then calls correct read function call
*Inputs:file descriptor, buffer to read into, and number of bytes to read
*Ouputs:function pointer corresponding to correct read function
*Side Effects:Obtains correct function pointer to return
*/

int32_t sys_read(int32_t fd, void* buf, int32_t nbytes){
	pcb_t* pcb = get_pcb();
  if(fd<0 || nbytes < 0 || fd>(PCB_LENGTH-1) || fd==1 || buf == NULL){ //error checking
         return -1;
  }
  fd_array_t a = pcb->fd_arr[fd];
	file_ops_t * b = a.file_operations;
	void* c = b->read_function;
  if((c == (read_fn*)&file_read)||(c == (read_fn*)&dir_read)){
    if(pcb->fd_arr[fd].flags == 0){
      return -1;
    }
  }
	int32_t (*fun_ptr)(int32_t, uint8_t*, int32_t) = (void*)c;
	return (*fun_ptr)(fd,buf,nbytes);

}/*Function:sys_write()
*Description:Does necessary checks, then calls correct write function call
*Inputs:file descriptor, buffer to write into, and number of bytes to write
*Ouputs:function pointer corresponding to correct write function
*Side Effects:Obtains correct function pointer to return
*/
int32_t sys_write(int32_t fd,const void* buf, int32_t nbytes){
	pcb_t* pcb = get_pcb();
  if(fd<1 || nbytes<0 || fd>(PCB_LENGTH-1) || buf == NULL)//error checking
		return -1;
	fd_array_t a = pcb->fd_arr[fd];
	file_ops_t * b = a.file_operations;
	void* c = b->write_function;
	int32_t (*fun_ptr)(int32_t, const uint8_t*, int32_t) = (void*)c;
	return (*fun_ptr)(fd,buf,nbytes);
}
/*Function:sys_open()
*Description:Does necessary checks, then calls correct open function call
*Inputs:filename to give file to open
*Ouputs:returns index of file in file descriptor array
*Side Effects:sets flags inode and file position in pcb struct for file opened
*/
int32_t sys_open(const uint8_t* filename){
    if(filename == NULL){
      return -1;
    }
    pcb_t* pcb = get_pcb();
    int32_t i;
    for(i=2;i<PCB_LENGTH;i++){
      if(pcb->fd_arr[i].flags == 0){
          break;
      }
    }
    if(i==PCB_LENGTH){
      return -1;
    }
    if(pcb->fd_arr[i].flags == 1){
        return -1;
    }
    //change depending on if you want dir_ops, rtc_ops, or file_ops
    //can only have one at a time
    dentry_t dir_ent;
    if(read_dentry_by_name(filename, &dir_ent) == -1){
      return -1;
    }
    if(dir_ent.filetype == 0)
    {
        (pcb->fd_arr[i]).file_operations = &rtc_operations;
    }
    else if(dir_ent.filetype == 1)
    {
        (pcb->fd_arr[i]).file_operations = &dir_operations;
    }
    else if(dir_ent.filetype == 2)
    {
        (pcb->fd_arr[i]).file_operations = &file_operations;
    }
    (pcb->fd_arr[i]).inode = dir_ent.inode_num;
    (pcb->fd_arr[i]).file_position = 0;
    (pcb->fd_arr[i]).flags = 1;
	return i;
}/*Function:sys_close()
*Description:Does necessary checks, then calls correct close function call
*Inputs:file descriptor
*Ouputs:function pointer corresponding to correct close function
*Side Effects:Obtains correct function pointer to return
*/
int32_t sys_close(int32_t fd){
  if(fd < 2 || fd>(PCB_LENGTH-1)){
		return -1;
	}
  pcb_t* pcb = get_pcb();
	(pcb->fd_arr[fd]).file_operations = &dir_operations;
  if((pcb->fd_arr[fd]).flags == 0){
    return -1;
  }
	(pcb->fd_arr[fd]).flags = 0;
	fd_array_t a = pcb->fd_arr[fd];
	file_ops_t * b = a.file_operations;
	void* c = b->close_function;
	int32_t (*fun_ptr)(int32_t) = (void*) c;
	return (*fun_ptr)(fd);
}
/*Function:sys_getargs()
*Description:Does nothing for now, returns 0
*/
int32_t sys_getargs(uint8_t* buf, int32_t nbytes){
	if((buf == NULL) || nbytes < 0 ){
		return -1;
	}
	pcb_t * curr_pcb = get_pcb();
	if(curr_pcb->argument[0] == NULL){
		return -1;
	}
	int i;
	for(i=0;i<BUFFER_SIZE;i++){
		buf[i] = curr_pcb->argument[i];
	}
	return 0;
}
/*Function:sys_vidmap()
*Description:Does nothing for now, returns 0
*/
int32_t sys_vidmap(uint8_t** screen_start){
  if (screen_start == NULL){
    return -1;
  }
  //check if given pointer in within bounds
  if(((uint32_t)screen_start < ONETWOEIGHTMB) || ((uint32_t)screen_start >(ONETWOEIGHTMB+(EIGHTMB/2)))){
    return -1;
  }
  int i;
  for(i=0;i<1024;i++){
    video_table[i] = ((i*(EIGHTKB/2))|OR_SIX);
  }
  video_table[0] = (VIDEO_PAGE|OR_SEVEN);
  page_dir[PAGE_VALUE]=((uint32_t)video_table|OR_SEVEN);
  *screen_start = (uint8_t*)(ONETWOEIGHTMB+(EIGHTMB/2));
  return (ONETWOEIGHTMB+(EIGHTMB/2));
}
/*Function:sys_set_handler()
*Description:Does nothing for now, returns 0
*/
int32_t sys_set_handler (int32_t signum, void* handler_address){
	return 0;
}
/*Function:sys_sigreturn()
*Description:Does nothing for now, returns 0
*/
int32_t sys_sigreturn (void){
	return 0;
}
/*Function:dummy_call()
*Description:Does nothing for now, returns 0
*/
int32_t dummy_call(){
	return -1;
}
