#ifndef HD44780U_H_
#define HD44780U_H_

//LCD I/O port&pin definitions
#define LCD_DATADIR		DDRD
#define LCD_DATAPORT    PORTD
#define LCD_DATAREAD	PIND

#define LCD_COMMANDDIR	DDRD
#define LCD_COMMANDPORT PORTD

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

//LCD commands definitions
#define CLEAR					0x01
#define RETURN_HOME				0x02
#define CURSOR_LEFT				0x04	
#define CURSOR_RIGHT			0x06
#define SHIFT_DISP_RIGHT		0x05
#define SHIFT_DISP_LEFT			0x07
#define DISP_OFF_CURSOR_OFF     0x08
#define DISP_OFF_CURSOR_ON      0x0A
#define DISP_ON_CURSOR_OFF      0x0C
#define DISP_ON_CURSOR_BLINK    0x0E
#define DISP_OFF_CURSOR_BLINK   0x0F
#define SHIFT_CURSOR_POS_LEFT   0x10
#define SHIFT_CURSOR_POS_RIGHT  0x14
#define SHIFT_ENTIRE_DISP_LEFT  0x18
#define SHIFT_ENTIRE_DISP_RIGHT 0x1C
//end of LCD commands definitions

/*other definitions*/
#define ASCII_NUMBER_OFFSET 0x30
#define ASCII_LETTER_OFFSET 0x37


/*
* Function: LCD_init
* ----------------------------
* Set up avr io initalization.
* As well function implement startup command sequence to an lcd module.
* this function must be called in main program before any other function in this file
* can be executed.
*/
void LCD_init(void);

/*
* Function: LCD_write
* ----------------------------
* Writes a single raw data byte to lcd module.
* data: a raw data byte that content us printed to LCD. See HD4470 datasheet for character lookup table.
*/
void LCD_write(uint8_t data);

/*
* Function: LCD_putString
* ----------------------------
* Writes constant string character data to LCD from progmem.
* strPtr: a null terminated string that MUST be pointing to program memory region.
*/
void LCD_putString(const char *strPtr);

/*
* Function: LCD_clear
* ----------------------------
* Clears LCD screen and return cursor to home location.
*/
void LCD_clear(void);

/*
* Function: LCD_gotoXY
* ----------------------------
* Moves LCD cursor to given location.
* x: horizontal cursor pos
* y: vertical cursor pos
*/
void LCD_gotoXY(uint8_t x, uint8_t y);

/*
* Function: LCD_putHexByte
* ----------------------------
* Writes given byte to lcd as hex with format of 0xdd.
* hexByte:
*/
void LCD_putHexByte(uint8_t hexByte);

/*
* Function: LCD_command
* ----------------------------
* Writes command to module command register
* command: a valid command(see HD4470 datasheet for available commands).
*/
void LCD_command(uint8_t command);

#endif