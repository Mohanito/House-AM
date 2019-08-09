#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "system_call.h"

#define KEYBOARD_BUFFER_SIZE    128
#define VIDEO_BUFFER_SIZE       4096

void update_cursor(int x, int y);

void terminal_init();

int32_t terminal_read (int32_t fd, char* buf, int32_t nbytes);  //similar to cin in cpp

int32_t terminal_write (int32_t fd, const char* buf, int32_t nbytes);

int32_t terminal_open (const uint8_t* filename);

int32_t terminal_close (int32_t fd);

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
char keyboard_buffer1[KEYBOARD_BUFFER_SIZE];    //multiple terminal
char keyboard_buffer2[KEYBOARD_BUFFER_SIZE];
char keyboard_buffer3[KEYBOARD_BUFFER_SIZE];

char video_buffer1[VIDEO_BUFFER_SIZE];    //multiple terminal
char video_buffer2[VIDEO_BUFFER_SIZE];
char video_buffer3[VIDEO_BUFFER_SIZE];

int end_buf_idx;
int end_buf_idx1;
int end_buf_idx2;
int end_buf_idx3;

char last_line_buffer[KEYBOARD_BUFFER_SIZE];  //for testing
int current_terminal;
#endif
