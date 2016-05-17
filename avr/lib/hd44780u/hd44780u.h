#ifndef MYLCD_H_
#define MYLCD_H_

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

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

//function prototypes
void LCD_init(void);
void LCD_write(uint8_t data);
void LCD_putString(const char *strPtr);
void LCD_Clear(void);
void LCD_gotoXY(uint8_t x, uint8_t y);
void LCD_putHexByte(uint8_t hexByte);
void LCD_command(uint8_t command);
//end of function prototypes

#endif /* MYLCD_H_ */