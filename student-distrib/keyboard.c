#include "keyboard.h"

#define SCAN_CODE_SET_SIZE				96
#define VIDEO       0xB8000
#define NUM_COLS    80
#define NUM_ROWS    25
#define ATTRIB      0x7
static char* video_mem = (char *)VIDEO;
volatile int enter_pressed;
unsigned int scan_code_set[SCAN_CODE_SET_SIZE] = {
													ASCII_0, SCAN_0,
												  ASCII_1, SCAN_1,
												  ASCII_2, SCAN_2,
												  ASCII_3, SCAN_3,
												  ASCII_4, SCAN_4,
												  ASCII_5, SCAN_5,
											   	ASCII_6, SCAN_6,
												  ASCII_7, SCAN_7,
											    ASCII_8, SCAN_8,
												  ASCII_9, SCAN_9,
												  ASCII_a, SCAN_A,
												  ASCII_b, SCAN_B,
												  ASCII_c, SCAN_C,
												  ASCII_d, SCAN_D,
												  ASCII_e, SCAN_E,
												  ASCII_f, SCAN_F,
											   	ASCII_g, SCAN_G,
												  ASCII_h, SCAN_H,
											    ASCII_i, SCAN_I,
												  ASCII_j, SCAN_J,
												  ASCII_k, SCAN_K,
												  ASCII_l, SCAN_L,
												  ASCII_m, SCAN_M,
												  ASCII_n, SCAN_N,
												  ASCII_o, SCAN_O,
												  ASCII_p, SCAN_P,
											   	ASCII_q, SCAN_Q,
												  ASCII_r, SCAN_R,
											    ASCII_s, SCAN_S,
												  ASCII_t, SCAN_T,
												  ASCII_u, SCAN_U,
												  ASCII_v, SCAN_V,
												  ASCII_w, SCAN_W,
												  ASCII_x, SCAN_X,
												  ASCII_y, SCAN_Y,
												  ASCII_z, SCAN_Z,

													ASCII_BTICK, SCAN_BTICK,
													ASCII_MINUS, SCAN_MINUS,
													ASCII_EQUAL, SCAN_EQUAL,
													ASCII_OPEN_B, SCAN_OPEN_B,
													ASCII_CLOSE_B, SCAN_CLOSE_B,
													ASCII_BSLASH, SCAN_BSLASH,
													ASCII_SCOLON, SCAN_SCOLON,
													ASCII_SQUOTE, SCAN_SQUOTE,
													ASCII_COMMA, SCAN_COMMA,
													ASCII_PERIOD, SCAN_PERIOD,
													ASCII_FSLASH, SCAN_FSLASH,
													ASCII_SPACE, SCAN_SPACE
												 };
unsigned int scan_code_set_caps[SCAN_CODE_SET_SIZE] = {
													ASCII_0, SCAN_0,
												 	ASCII_1, SCAN_1,
												 	ASCII_2, SCAN_2,
												 	ASCII_3, SCAN_3,
												 	ASCII_4, SCAN_4,
												 	ASCII_5, SCAN_5,
												 	ASCII_6, SCAN_6,
												 	ASCII_7, SCAN_7,
												 	ASCII_8, SCAN_8,
												 	ASCII_9, SCAN_9,
												 	ASCII_A, SCAN_A,
												 	ASCII_B, SCAN_B,
												 	ASCII_C, SCAN_C,
												 	ASCII_D, SCAN_D,
												 	ASCII_E, SCAN_E,
												 	ASCII_F, SCAN_F,
												 	ASCII_G, SCAN_G,
												 	ASCII_H, SCAN_H,
												 	ASCII_I, SCAN_I,
												 	ASCII_J, SCAN_J,
												 	ASCII_K, SCAN_K,
												 	ASCII_L, SCAN_L,
												 	ASCII_M, SCAN_M,
												 	ASCII_N, SCAN_N,
												 	ASCII_O, SCAN_O,
												 	ASCII_P, SCAN_P,
												 	ASCII_Q, SCAN_Q,
												 	ASCII_R, SCAN_R,
												 	ASCII_S, SCAN_S,
												 	ASCII_T, SCAN_T,
												 	ASCII_U, SCAN_U,
												 	ASCII_V, SCAN_V,
												 	ASCII_W, SCAN_W,
												 	ASCII_X, SCAN_X,
												 	ASCII_Y, SCAN_Y,
												 	ASCII_Z, SCAN_Z,

												 ASCII_BTICK, SCAN_BTICK,
												 ASCII_MINUS, SCAN_MINUS,
												 ASCII_EQUAL, SCAN_EQUAL,
												 ASCII_OPEN_B, SCAN_OPEN_B,
												 ASCII_CLOSE_B, SCAN_CLOSE_B,
												 ASCII_BSLASH, SCAN_BSLASH,
												 ASCII_SCOLON, SCAN_SCOLON,
												 ASCII_SQUOTE, SCAN_SQUOTE,
												 ASCII_COMMA, SCAN_COMMA,
												 ASCII_PERIOD, SCAN_PERIOD,
												 ASCII_FSLASH, SCAN_FSLASH,
												 ASCII_SPACE, SCAN_SPACE
												 };
unsigned int scan_code_set_shift[SCAN_CODE_SET_SIZE] = {
												 ASCII_CLOSE_PAR, SCAN_0,
												 ASCII_EXC, SCAN_1,
												 ASCII_AT, SCAN_2,
												 ASCII_POUND, SCAN_3,
												 ASCII_DOLLAR, SCAN_4,
												 ASCII_PERCENT, SCAN_5,
												 ASCII_CARROT, SCAN_6,
												 ASCII_AND, SCAN_7,
												 ASCII_STAR, SCAN_8,
												 ASCII_OPEN_PAR, SCAN_9,
												 ASCII_A, SCAN_A,
												 ASCII_B, SCAN_B,
												 ASCII_C, SCAN_C,
												 ASCII_D, SCAN_D,
												 ASCII_E, SCAN_E,
												 ASCII_F, SCAN_F,
												 ASCII_G, SCAN_G,
												 ASCII_H, SCAN_H,
												 ASCII_I, SCAN_I,
												 ASCII_J, SCAN_J,
												 ASCII_K, SCAN_K,
												 ASCII_L, SCAN_L,
												 ASCII_M, SCAN_M,
												 ASCII_N, SCAN_N,
												 ASCII_O, SCAN_O,
												 ASCII_P, SCAN_P,
												 ASCII_Q, SCAN_Q,
												 ASCII_R, SCAN_R,
												 ASCII_S, SCAN_S,
												 ASCII_T, SCAN_T,
												 ASCII_U, SCAN_U,
												 ASCII_V, SCAN_V,
												 ASCII_W, SCAN_W,
												 ASCII_X, SCAN_X,
												 ASCII_Y, SCAN_Y,
												 ASCII_Z, SCAN_Z,

												 ASCII_TILDA, SCAN_BTICK,
												 ASCII_UNDER, SCAN_MINUS,
												 ASCII_PLUS, SCAN_EQUAL,
												 ASCII_OPEN_CURLY, SCAN_OPEN_B,
												 ASCII_CLOSE_CURLY, SCAN_CLOSE_B,
												 ASCII_BAR, SCAN_BSLASH,
												 ASCII_COLON, SCAN_SCOLON,
												 ASCII_DQUOTE, SCAN_SQUOTE,
												 ASCII_LTHAN, SCAN_COMMA,
												 ASCII_GTHAN, SCAN_PERIOD,
												 ASCII_QUESTION, SCAN_FSLASH,
												 ASCII_SPACE, SCAN_SPACE
												 };
unsigned int scan_code_set_shift_caps[SCAN_CODE_SET_SIZE] = {
 												 ASCII_CLOSE_PAR, SCAN_0,
 												 ASCII_EXC, SCAN_1,
 												 ASCII_AT, SCAN_2,
 												 ASCII_POUND, SCAN_3,
 												 ASCII_DOLLAR, SCAN_4,
 												 ASCII_PERCENT, SCAN_5,
 												 ASCII_CARROT, SCAN_6,
 												 ASCII_AND, SCAN_7,
 												 ASCII_STAR, SCAN_8,
 												 ASCII_OPEN_PAR, SCAN_9,
												 ASCII_a, SCAN_A,
												 ASCII_b, SCAN_B,
												 ASCII_c, SCAN_C,
												 ASCII_d, SCAN_D,
												 ASCII_e, SCAN_E,
												 ASCII_f, SCAN_F,
												 ASCII_g, SCAN_G,
												 ASCII_h, SCAN_H,
												 ASCII_i, SCAN_I,
												 ASCII_j, SCAN_J,
												 ASCII_k, SCAN_K,
												 ASCII_l, SCAN_L,
												 ASCII_m, SCAN_M,
												 ASCII_n, SCAN_N,
												 ASCII_o, SCAN_O,
												 ASCII_p, SCAN_P,
												 ASCII_q, SCAN_Q,
												 ASCII_r, SCAN_R,
												 ASCII_s, SCAN_S,
												 ASCII_t, SCAN_T,
												 ASCII_u, SCAN_U,
												 ASCII_v, SCAN_V,
												 ASCII_w, SCAN_W,
												 ASCII_x, SCAN_X,
												 ASCII_y, SCAN_Y,
												 ASCII_z, SCAN_Z,

 												 ASCII_TILDA, SCAN_BTICK,
 												 ASCII_UNDER, SCAN_MINUS,
 												 ASCII_PLUS, SCAN_EQUAL,
 												 ASCII_OPEN_CURLY, SCAN_OPEN_B,
 												 ASCII_CLOSE_CURLY, SCAN_CLOSE_B,
 												 ASCII_BAR, SCAN_BSLASH,
 												 ASCII_COLON, SCAN_SCOLON,
 												 ASCII_DQUOTE, SCAN_SQUOTE,
 												 ASCII_LTHAN, SCAN_COMMA,
 												 ASCII_GTHAN, SCAN_PERIOD,
 												 ASCII_QUESTION, SCAN_FSLASH,
 												 ASCII_SPACE, SCAN_SPACE
												 };






int caps_lock_on = 0;
int shift_on = 0;
int ctrl_on = 0;
int alt_on = 0;

/* Initialization of Keyboard */
void keyboard_init(){
	enable_irq(KEYBOARD_IRQ);
	enter_pressed = 0;
	screen_x = 0;
	screen_y = 0;
	update_cursor(0,0);
}
/*
Function:keyboard_interrupt
Description: Keyboard Interrupt Handler
*/
void keyboard_interrupt(){

	send_eoi(KEYBOARD_IRQ);
	//printf("keyboard_interrupt \n");

	//Read from the PS2 data port
	unsigned int temp = inb(PS2_DATA_PORT);

	//check caps_lock, shift, ctrl
	set_keyboard_flags(temp);

		//int i;		//for loop

		if(alt_on ==1 && temp == SCAN_FONE){
			terminalone();
		}
		if(alt_on ==1 && temp == SCAN_FTWO){
			terminaltwo();
			if(terminal_used[1] == 0){
				//printf("terminal_used[1] = %d",terminal_used[1]);
				terminal_used[1] = 1;
				/*
				outb(0x0A, 0x3D4);
				outb((inb(0x3D5) & 0xC0) | 0, 0x3D5);

				outb( 0x0B, 0x3D4);
				outb((inb(0x3D5) & 0xE0) | 8,0x3D5);
				*/
				sys_execute((uint8_t*)"shell");
			}

		}
		if(alt_on == 1 && temp == SCAN_FTHREE){
			terminalthree();
			if(terminal_used[2] == 0){
				//printf("terminal_used[2] = %d",terminal_used[2]);
				terminal_used[2] = 1;
				/*
				outb(0x0A, 0x3D4);
				outb((inb(0x3D5) & 0xC0) | 0, 0x3D5);

				outb( 0x0B, 0x3D4);
				outb((inb(0x3D5) & 0xE0) | 8,0x3D5);
				*/
				sys_execute((uint8_t*)"shell");

			}
		}
	//CTRL + L
	if(ctrl_on == 1 && temp == SCAN_L){
		clear();
		//and move cursor
		screen_x = 0;
		screen_y = 0;
		update_cursor(screen_x,screen_y);
		return;
	}

	//Enter
	if(temp == SCAN_ENTER){
		enter_pressed = 1;
		keyboard_buffer[end_buf_idx] = '\n';
		//UNCOMMENT FOR TERMINAL READ TEST
		//terminal_read(0, last_line_buffer, 128);
		//terminal_write(0, last_line_buffer, 128);
		//scrolling
		if(screen_y == (NUM_ROWS - 1)){			//if at last row
			scroll_up();
			update_cursor(screen_x,screen_y);
			return;
		}
		else{
			screen_y ++;
			screen_x = 0;
			update_cursor(screen_x, screen_y);
		}
		return;
	}

	//Backspace
	if(temp == SCAN_BACKSPACE){
		if (end_buf_idx > 0){
			screen_x--;
			screen_x %= NUM_COLS;
			screen_y = (screen_y + (screen_x / NUM_COLS)) % NUM_ROWS;
			*(uint8_t *)(video_mem + ((NUM_COLS * screen_y + screen_x) << 1)) = 0;
			*(uint8_t *)(video_mem + ((NUM_COLS * screen_y + screen_x) << 1) + 1) = ATTRIB;
			end_buf_idx --;
			keyboard_buffer[end_buf_idx] = 0;
			update_cursor(screen_x,screen_y);
		}
		return;
	}

	//print corresponding character to the screen
	if (temp < DUMMY &&
		 	temp != SCAN_LSHIFT && temp != SCAN_RSHIFT &&
			temp != SCAN_CTRL && temp != SCAN_TAB &&
			temp != SCAN_ESC && temp != SCAN_ALT){
		char tempc = scan_code_to_char(temp);
		if(end_buf_idx < (KEYBOARD_BUFFER_SIZE - 2)){
			keyboard_buffer[end_buf_idx] = tempc;
			end_buf_idx++;
		}
		else if(end_buf_idx == (KEYBOARD_BUFFER_SIZE - 2)){
			keyboard_buffer[end_buf_idx] = tempc;
			end_buf_idx++;			//so end_buf_idx becomes 127, and the buffer does not accept more characters
			keyboard_buffer[end_buf_idx] = '\n';
		}
		if(end_buf_idx > (KEYBOARD_BUFFER_SIZE - 2)){
			return;
		}
		if(screen_x == NUM_COLS - 1 && screen_y == NUM_ROWS - 1){		//if we reach the bottom right of screen
			scroll_up();
		}
		else{
			update_cursor(screen_x + 1,screen_y);
		}
		putc(tempc);
		//terminal_write(0, keyboard_buffer, 128);	//was putc
		//puts(keyboard_buffer);
	}
}


/*
Function:scan_code_to_char
Description: setup of cr4 and cr0 for paging
Input: scan code from PS/2 keyboard
Output: the corresponding ASCII code in type char
*/
char scan_code_to_char(unsigned int scan){
	int i = 0; 			//for loop
	char c;				//return value
	//printf("scan = %x       \n   ", scan);
	for(i = 1; i < SCAN_CODE_SET_SIZE; i+=2){
		if(scan_code_set[i] == scan){

			if(shift_on == 1 && caps_lock_on == 1){
				c = scan_code_set_shift_caps[i - 1];	//assign int to char in order to print ASCII
			}
			else if(shift_on == 0 && caps_lock_on == 1){
				c = scan_code_set_caps[i - 1];	//assign int to char in order to print ASCII
			}
			else if(shift_on == 1 && caps_lock_on == 0){
				c = scan_code_set_shift[i - 1];	//assign int to char in order to print ASCII
			}
			else{
				c = scan_code_set[i - 1];	//assign int to char in order to print ASCII
			}

		}
	}
	return c;
}

/*
Function:set_keyboard_flags
Description: This function set the CTRL, SHIFT and caps_lock flags according to scan code
Input: scan code from PS/2 keyboard
*/
void set_keyboard_flags(unsigned int scan){
	switch (scan){
		case SCAN_CTRL:
			ctrl_on = 1;
			break;
		case SCAN_CTRL_RELEASE:
			ctrl_on = 0;
			break;
		case SCAN_LSHIFT:
			shift_on = 1;
			break;
		case SCAN_LSHIFT_RELEASE:
			shift_on = 0;
			break;
		case SCAN_RSHIFT:
			shift_on = 1;
			break;
		case SCAN_RSHIFT_RELEASE:
			shift_on = 0;
			break;
		case SCAN_CAPS:
			caps_lock_on = 1 -(caps_lock_on);
			break;
		case SCAN_ALT:
			alt_on = 1;
			break;
		case SCAN_ALT_RELEASE:
			alt_on = 0;
			break;
		default:
			break;
	}
	return;
}

/*
Function:scroll_up
Description: This function stores the screen below and including second row, save it to a buffer,
and copy them to the corresponding position in video memory so that the screen is scrolled up by one row.
The function also empties the last row, and move cursor to the bottom left.
*/
void scroll_up(){
	char scroll_buf[(NUM_ROWS - 1) * NUM_COLS];
	int row, col;					//for loop
	int i = 0;						//buffer index
	for(row = 1; row < NUM_ROWS; row++){
		for(col = 0; col < NUM_COLS; col++){
			//copy to buffer
			scroll_buf[i] = *(uint8_t *)(video_mem + ((NUM_COLS * row + col) << 1));
			i++;
		}
	}
	i = 0;								//reset buffer index
	for(row = 0; row < NUM_ROWS - 1; row++){
		for(col = 0; col < NUM_COLS; col++){
			//copy to correct position in video memory
			*(uint8_t *)(video_mem + ((NUM_COLS * row + col) << 1)) = scroll_buf[i];
			i++;
		}
	}
	//empty the last row
	for(col = 0; col < NUM_COLS; col++){
		//copy to correct position in video memory
		*(uint8_t *)(video_mem + ((NUM_COLS * (NUM_ROWS - 1) + col) << 1)) = 0;
	}

	//move cursor to last row
	screen_x = 0;
	screen_y = NUM_ROWS - 1;
	update_cursor(screen_x,screen_y);
	return;
}
/*
Function:terminal_switch
Description: Saves keyboard buffer and video memory in corresponding buffer. also
saves cursor position. Is a helper function for multiple terminals
*/
void terminal_switch(){
	int i;
	//pcb_t * pcb = get_pcb();
	switch(current_terminal){
		case 1:
			screen_x1 = screen_x;
			screen_y1 = screen_y;
			for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
				char temp = keyboard_buffer[i];
				keyboard_buffer1[i] = temp;
			}
			for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
				char character = *(video_mem+i);
				video_buffer1[i] = character;
			}
			//pcb->terminal_num = 1;
			end_buf_idx1 = end_buf_idx;
			break;
		case 2:
			screen_x2 = screen_x;
			screen_y2 = screen_y;
			for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
				char temp = keyboard_buffer[i];
				keyboard_buffer2[i] = temp;
			}
			for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
				char character = *(video_mem+i);
				video_buffer2[i] = character;
			}
			//pcb->terminal_num = 2;
			end_buf_idx2 = end_buf_idx;
			break;
		case 3:
			screen_x3 = screen_x;
			screen_y3 = screen_y;
			for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
				char temp = keyboard_buffer[i];
				keyboard_buffer3[i] = temp;
			}
			for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
				char character = *(video_mem+i);
				video_buffer3[i] = character;
			}
			end_buf_idx3 = end_buf_idx;
			//pcb->terminal_num = 3;
			break;
		default:
			break;
	}
}
//Function to switch to terminal one
void terminalone(){
	int i;
	terminal_switch();
	current_terminal = 1;
	for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
		char character = video_buffer1[i];
		*(video_mem+i) = character;
	}
	screen_x = screen_x1;
	screen_y = screen_y1;
	for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
		char temp = keyboard_buffer1[i];
		keyboard_buffer[i] = temp;
	}
	end_buf_idx = end_buf_idx1;
	update_cursor(screen_x,screen_y);
}
//Function to switch to terminal one
void terminaltwo(){
	int i;
	terminal_switch();
	current_terminal = 2;
	for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
		char character = video_buffer2[i];
		*(video_mem+i) = character;
	}
	screen_x = screen_x2;
	screen_y = screen_y2;
	for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
		char temp = keyboard_buffer2[i];
		keyboard_buffer[i] = temp;
	}
	end_buf_idx = end_buf_idx2;
	update_cursor(screen_x,screen_y);
}
//Function to switch to terminal one
void terminalthree(){
	int i;
	terminal_switch();
	current_terminal = 3;
	for(i = 0; i < VIDEO_BUFFER_SIZE; i++){
		char character = video_buffer3[i];
		*(video_mem+i) = character;
	}
	screen_x = screen_x3;
	screen_y = screen_y3;
	for(i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
		char temp = keyboard_buffer3[i];
		keyboard_buffer[i] = temp;
	}
	end_buf_idx = end_buf_idx3;
	update_cursor(screen_x,screen_y);
}
