#include "lib.h"
#include "terminal.h"

#define VGA_CONTROL_REG							0x3D4
#define VGA_CONTROL_REG2						0x3D5
#define VGA_MASK										0xFF
/*
Function:update_cursor
Description: This function moves the position of the cursor by sending
data to VGA control registers.
Input: the x and y position we want to move the cursor to
*/
void update_cursor(int x, int y)
{
	uint16_t pos = y * NUM_COLS + x;

	//ask VGA control register (0x3d4) for bytes.
	outb(0x0F, VGA_CONTROL_REG);
	outb((uint8_t) (pos & VGA_MASK), VGA_CONTROL_REG2);
	outb(0x0E, VGA_CONTROL_REG);
	outb((uint8_t) ((pos >> 8) & VGA_MASK), VGA_CONTROL_REG2);
}

/* void terminal_init()
 * Clears keyboard_buffer and reset end_buf_idx */
void terminal_init(){
	int i = 0;		//for loop
	for(; i < KEYBOARD_BUFFER_SIZE; i++){
		keyboard_buffer[i] = 0;

		keyboard_buffer1[i] = 0;
		keyboard_buffer2[i] = 0;	//multiple terminal
		keyboard_buffer3[i] = 0;

	}
	for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
		video_buffer1[i] = 0;
		video_buffer2[i] = 0;	//multiple terminal
		video_buffer3[i] = 0;

	}
	//not sure if necessary
	screen_x1 = 0;
	screen_y1 = 0;
	screen_x2 = 0;
	screen_y2 = 0;
	screen_x3 = 0;
	screen_y3 = 0;

	end_buf_idx = 0;
	end_buf_idx1 = 0;
	end_buf_idx2 = 0;
	end_buf_idx3 = 0;
	current_terminal = 1;
	terminal_used[0] = 1;
	terminal_used[1] = 0;
	terminal_used[2] = 0;
	sys_execute((uint8_t*)"shell");
}

/*
Function:terminal_read
Description: In the case of the keyboard, read should return data from one line that has been terminated by pressing
enter, or as much as fits in the buffer from one such line. The line returned should include the line feed character.
Input: fd is file descriptor, buf is a buffer that we store data to, nbytes is the number of bytes we want to read.
Output: returns the number of bytes read.
*/
int32_t terminal_read (int32_t fd, char* buf, int32_t nbytes){
	int i = 0;
	while(!enter_pressed){
	}
	enter_pressed = 0;

	int count = 0;
	for(; count < nbytes; count++){
		if(keyboard_buffer[count] == 0){
			break;
		}
		buf[count] = keyboard_buffer[count];
	}
	for(; i < KEYBOARD_BUFFER_SIZE; i++){
		keyboard_buffer[i] = 0;
	}
	end_buf_idx = 0;
	return count;
}

/*
Function:terminal_write
Description: The write system call writes data to the terminal. In the case of the terminal, all data should
be displayed to the screen immediately.
Input: fd is file descriptor, buf is buffer of the data to be written to the screen, nbytes is the number of bytes we want to read.
Output: The call returns the number of bytes written, or -1 on failure.
*/
int32_t terminal_write (int32_t fd, const char* buf, int32_t nbytes){
	int i = 0; 	//for loop
	cli();
	for(; i < nbytes; i++){
		putc(buf[i]);
	}
	sti();
	return nbytes;
}
int32_t terminal_open (const uint8_t* filename){
	return 0;
}

int32_t terminal_close (int32_t fd){
	return 0;
}
