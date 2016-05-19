#ifndef HD44780U_H_
#define HD44780U_H_

//LCD I/O port&pin definitions
#define HD44780U_DATADIR DDRD
#define HD44780U_DATAPORT PORTD
#define HD44780U_DATAREAD PIND

#define HD44780U_COMMANDDIR	DDRD
#define HD44780U_COMMANDPORT PORTD

#define RS 4
#define RW 6
#define EN 7

#define D0 0
#define D1 1
#define D2 2
#define D3 3
//LCD I/O port&pin definitions

/*LCD properties */
#define LCD_ROWS 2
#define LCD_COLUMNS 16

//Commands definitions
#define HD44780U_CLEAR					 0x01
#define HD44780U_RETURN_HOME			 0x02
#define HD44780U_CURSOR_LEFT			 0x04
#define HD44780U_CURSOR_RIGHT			 0x06
#define HD44780U_SHIFT_DISP_RIGHT		 0x05
#define HD44780U_SHIFT_DISP_LEFT		 0x07
#define HD44780U_DISP_OFF_CURSOR_OFF     0x08
#define HD44780U_DISP_OFF_CURSOR_ON      0x0A
#define HD44780U_DISP_ON_CURSOR_OFF      0x0C
#define HD44780U_DISP_ON_CURSOR_BLINK    0x0E
#define HD44780U_DISP_OFF_CURSOR_BLINK   0x0F
#define HD44780U_SHIFT_CURSOR_POS_LEFT   0x10
#define HD44780U_SHIFT_CURSOR_POS_RIGHT  0x14
#define HD44780U_SHIFT_ENTIRE_DISP_LEFT  0x18
#define HD44780U_SHIFT_ENTIRE_DISP_RIGHT 0x1C
//end of LCD commands definitions

/*
* Function: hd44780u_init
* ----------------------------
* Set up hd44780u controller io
* and implement startup command sequence to an module.
* this function must be called in main program before any other usage.
*/
void hd44780u_init(void);

/*
* Function: hd44780u_write
* ----------------------------
* Writes a single raw character controller attached lcd module.
* data: See HD4470 datasheet for available characters.
*/
void hd44780u_write(uint8_t data);

/*
* Function: hd44780u_putString
* ----------------------------
* Writes string of characters to controller attached lcd module.
* strPtr: a null terminated constant string that MUST be pointing to program memory region.
*/
void hd44780u_putString(const char *strPtr);

/*
* Function: hd44780u_clear
* ----------------------------
* Clears controller lcd contents and return cursor to home location.
*/
void hd44780u_clear(void);

/*
* Function: hd44780u_gotoXY
* ----------------------------
* Moves controller lcd cursor to given location.
* x: horizontal cursor pos
* y: vertical cursor pos
*/
void hd44780u_gotoXY(uint8_t x, uint8_t y);

/*
* Function: hd44780u_putHexByte
* ----------------------------
* Writes given byte to controller lcd as hex with format of 0xDD.
* hexByte:
*/
void hd44780u_putHexByte(uint8_t hexByte);

/*
* Function: hd44780u_command
* ----------------------------
* Writes single command byte to the controller command register.
* command: See HD4470 datasheet for available commands.
*/
void hd44780u_command(uint8_t command);

#endif //HD44780U_H_
