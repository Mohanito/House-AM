#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "i8259.h"
#include "lib.h"
#include "terminal.h"
#include "system_call.h"
#include "paging.h"

#define KEYBOARD_IRQ 			1

#define PS2_DATA_PORT			0x60

#define VIDEO       0xB8000
#define NUM_COLS    80
#define NUM_ROWS    25
#define ATTRIB      0x7


//ASCII Values

#define ASCII_SPACE				32
#define ASCII_EXC					33
#define ASCII_DQUOTE					34
#define ASCII_POUND					35
#define ASCII_DOLLAR					36
#define ASCII_PERCENT					37
#define ASCII_AND					38
#define ASCII_SQUOTE					39
#define ASCII_OPEN_PAR			40
#define ASCII_CLOSE_PAR			41
#define ASCII_STAR					42
#define ASCII_PLUS					43
#define ASCII_COMMA					44
#define ASCII_MINUS					45
#define ASCII_PERIOD					46
#define ASCII_FSLASH					47

#define ASCII_0					48
#define ASCII_1					49
#define ASCII_2					50
#define ASCII_3					51
#define ASCII_4					52
#define ASCII_5					53
#define ASCII_6					54
#define ASCII_7					55
#define ASCII_8					56
#define ASCII_9					57

#define ASCII_COLON					58
#define ASCII_SCOLON					59
#define ASCII_LTHAN					60
#define ASCII_EQUAL					61
#define ASCII_GTHAN					62
#define ASCII_QUESTION					63
#define ASCII_AT					64

#define ASCII_A					65
#define ASCII_B					66
#define ASCII_C					67
#define ASCII_D					68
#define ASCII_E					69
#define ASCII_F					70
#define ASCII_G					71
#define ASCII_H					72
#define ASCII_I					73
#define ASCII_J					74
#define ASCII_K					75
#define ASCII_L					76
#define ASCII_M					77
#define ASCII_N					78
#define ASCII_O					79
#define ASCII_P					80
#define ASCII_Q					81
#define ASCII_R					82
#define ASCII_S					83
#define ASCII_T					84
#define ASCII_U					85
#define ASCII_V					86
#define ASCII_W					87
#define ASCII_X					88
#define ASCII_Y					89
#define ASCII_Z					90

#define ASCII_OPEN_B					91
#define ASCII_BSLASH					92
#define ASCII_CLOSE_B					93
#define ASCII_CARROT					94
#define ASCII_UNDER					95
#define ASCII_BTICK					96

#define ASCII_a					97
#define ASCII_b					98
#define ASCII_c					99
#define ASCII_d					100
#define ASCII_e					101
#define ASCII_f					102
#define ASCII_g					103
#define ASCII_h					104
#define ASCII_i					105
#define ASCII_j					106
#define ASCII_k					107
#define ASCII_l					108
#define ASCII_m					109
#define ASCII_n					110
#define ASCII_o					111
#define ASCII_p					112
#define ASCII_q					113
#define ASCII_r					114
#define ASCII_s					115
#define ASCII_t					116
#define ASCII_u					117
#define ASCII_v					118
#define ASCII_w					119
#define ASCII_x					120
#define ASCII_y					121
#define ASCII_z					122

#define ASCII_OPEN_CURLY		123
#define ASCII_BAR					124
#define ASCII_CLOSE_CURLY		125
#define ASCII_TILDA					126







//scan code set 1


#define SCAN_0					0x0B
#define SCAN_1					0x02
#define SCAN_2					0x03
#define SCAN_3					0x04
#define SCAN_4					0x05
#define SCAN_5					0x06
#define SCAN_6					0x07
#define SCAN_7					0x08
#define SCAN_8					0x09
#define SCAN_9					0x0A

#define SCAN_A					0x1E
#define SCAN_B					0x30
#define SCAN_C					0x2E
#define SCAN_D					0x20
#define SCAN_E					0x12
#define SCAN_F					0x21
#define SCAN_G					0x22
#define SCAN_H					0x23
#define SCAN_I					0x17
#define SCAN_J					0x24
#define SCAN_K					0x25
#define SCAN_L					0x26
#define SCAN_M					0x32
#define SCAN_N					0x31
#define SCAN_O					0x18
#define SCAN_P					0x19
#define SCAN_Q					0x10
#define SCAN_R					0x13
#define SCAN_S					0x1F
#define SCAN_T					0x14
#define SCAN_U					0x16
#define SCAN_V					0x2F
#define SCAN_W					0x11
#define SCAN_X					0x2D
#define SCAN_Y					0x15
#define SCAN_Z					0x2C

#define SCAN_BTICK					0x29
#define SCAN_CAPS					  0x3A
#define SCAN_LSHIFT					0x2A
#define SCAN_LSHIFT_RELEASE	0xAA
#define SCAN_RSHIFT					0x36
#define SCAN_RSHIFT_RELEASE	0xB6
#define SCAN_CTRL					  0x1D
#define SCAN_CTRL_RELEASE	  0x9D
#define SCAN_MINUS					0x0C
#define SCAN_EQUAL					0x0D
#define SCAN_BACKSPACE			0x0E
#define SCAN_OPEN_B					0x1A
#define SCAN_CLOSE_B				0x1B
#define SCAN_BSLASH					0x2B
#define SCAN_SCOLON					0x27
#define SCAN_SQUOTE					0x28
#define SCAN_ENTER					0x1C
#define SCAN_COMMA					0x33
#define SCAN_PERIOD					0x34
#define SCAN_FSLASH					0x35
#define SCAN_SPACE				  0x39
#define SCAN_TAB            0x0F
#define SCAN_ESC            0x01
#define SCAN_ALT            0x38
#define SCAN_ALT_RELEASE    0xB8
#define SCAN_FONE           0x3B
#define SCAN_FTWO           0x3C
#define SCAN_FTHREE         0x3D

#define DUMMY               0x3A    //CapsLock

void keyboard_init();
char scan_code_to_char(unsigned int scan);
extern volatile int enter_pressed;
void keyboard_interrupt();
void set_keyboard_flags(unsigned int scan);
void scroll_up();
void terminal_switch();
int terminal_used[3];
void terminalone();
void terminaltwo();
void terminalthree();

#endif
