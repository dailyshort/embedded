#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hd44780u.h"

//private function prototypes
static uint8_t LCD_setPortData(uint8_t nibble);
static uint8_t LCD_isBusy(void);
static void LCD_toggleEnable(void);

/*
* Function: LCD_init
* ----------------------------
* Set up avr io initalization.
* As well function implement startup command sequence to an lcd module.
* this function must be called in main program before any other function in this file
* can be executed.
*/
void LCD_init(void)
{
	//port init
	LCD_DATAPORT    &= ~((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0));
	LCD_COMMANDPORT &= ~((1 << RS) | (1 << RW) | (1 << EN));
	
	LCD_DATADIR    |= (1 << D3) | (1 << D2) | (1 << D1) | (1 << D0);
	LCD_COMMANDDIR |= (1 << RS) | (1 << RW) | (1 << EN);
	//end of port init
	
	//START LCD init procedure. check flowchart at HD4470 datasheet Figure 24 4-Bit Interface page 46
	_delay_ms(50);
	LCD_command(0x33);//lcd init
	LCD_command(0x32);//lcd init
	LCD_command(0x28);//4-bit bus mode + 2 line/5*8dots display set
	LCD_command(0x0E);//Display on, cursor on, cursor blink off
	LCD_command(0x01);//clear display
	LCD_command(0x06);//cursor advances right
}

/*
* Function: LCD_command
* ----------------------------
* Writes command to module command register
* command: a valid command(see HD4470 datasheet for available commands).
*/
void LCD_command(uint8_t command)
{
	while(LCD_isBusy())//wait while LCD device is busy
	{
		_delay_us(40);
	}	
	
	LCD_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	LCD_DATAPORT |= LCD_setPortData((command & 0xF0) >> 4);//send High nibble

	LCD_COMMANDPORT	&= ~ (1 << RS);// 0 for command
	LCD_COMMANDPORT &= ~ (1 << RW);// 0 for write
	
	LCD_toggleEnable();
	
	LCD_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	LCD_DATAPORT |= LCD_setPortData((command & 0x0F));//send low nibble
	
	LCD_toggleEnable();
}

/*
* Function: LCD_write
* ----------------------------
* Writes a single raw data byte to lcd module.
* data: a raw data byte that content us printed to LCD. See HD4470 datasheet for character lookup table.
*/
void LCD_write(uint8_t data)
{
	while(LCD_isBusy())//wait while LCD device is busy
	{
		_delay_us(40);
	}		
	
	LCD_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	LCD_DATAPORT |= LCD_setPortData((data & 0xF0) >> 4);//send High nibble

	LCD_COMMANDPORT	|= (1 << RS);// 1 for data
	LCD_COMMANDPORT &= ~ (1 << RW);// 0 for write
	
	LCD_toggleEnable();
	
	LCD_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	LCD_DATAPORT |= LCD_setPortData(data & 0x0F);//send low nibble
	
	LCD_toggleEnable();
}

/*
* Function: LCD_putString
* ----------------------------
* Writes constant string character data to LCD from progmem.
* strPtr: a null terminated string that MUST be pointing to program memory region.
*/
void LCD_putString(const char *strPtr)
{
	while(pgm_read_byte(strPtr))
	{
		LCD_write(pgm_read_byte(strPtr++));
	}
}

/*
* Function: LCD_clear
* ----------------------------
* Clears LCD screen and return cursor to home location.
*/
void LCD_clear(void)
{
	LCD_command(CLEAR);
	LCD_command(RETURN_HOME);
}

/*
* Function: LCD_gotoXY
* ----------------------------
* Moves LCD cursor to given location.
* x: horizontal cursor pos
* y: vertical cursor pos
*/
void LCD_gotoXY(uint8_t x, uint8_t y)
{
	const uint8_t lines[] = { 0x80, 0xC0, 0x94, 0xD4 };//0x80 + LCD line start address code
	#if LCD_ROWS > 4 
	#error "over 4 row lcd not supported" 
	#endif
	LCD_command(lines[y] + x);		
}

/*
* Function: LCD_putHexByte
* ----------------------------
* Writes given byte to lcd as hex with format of 0xdd.
* hexByte:
*/
void LCD_putHexByte(uint8_t hexByte)
{
	uint8_t upper = ((hexByte & 0xF0) >> 4);
	uint8_t lower = (hexByte & 0x0F);
	
	if(upper < 0x0A) upper += ASCII_NUMBER_OFFSET;//ascii number offset
	else upper += ASCII_LETTER_OFFSET;//ascii capital letter offset
	
	if(lower < 0x0A) lower += ASCII_NUMBER_OFFSET;//ascii number offset
	else lower += ASCII_LETTER_OFFSET;//ascii capital letter offset
	
	LCD_write('0');
	LCD_write('x');
	LCD_write(upper);
	LCD_write(lower);
}

/*
* Function: LCD_toggleEnable
* ----------------------------
* Toggles LCD enable line.
*/
static void LCD_toggleEnable(void)
{
	_delay_us(1);//wait for signals to set up
	LCD_COMMANDPORT |= (1 << EN);// 1 for enable
	_delay_us(2);//wider pulse
	LCD_COMMANDPORT &= ~ (1 << EN);//Enable 0
	_delay_us(1);//wait for LCD to acknowledge disable
}

/*
* Function: LCD_isBusy
* ----------------------------
* Check if module is busy and cannot accept new messages.
* returns: 0 if not busy, 1 if busy.
*/
static uint8_t LCD_isBusy(void)
{
	uint8_t returnable = 0;
	
	LCD_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	LCD_DATADIR  &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0));//set data to input for read operation
	
	LCD_COMMANDPORT	&= ~ (1 << RS);// 0 for command
	LCD_COMMANDPORT |=   (1 << RW);// 1 for read
	
	//capture high nibble
	_delay_us(1);//wait for data to set up
	LCD_COMMANDPORT |= (1 << EN);// 1 for enable
	_delay_us(2);//wider pulse
	if(LCD_DATAREAD & (1 << D3)) returnable = 1;//latch BusyFlag
	LCD_COMMANDPORT &= ~ (1 << EN);//Enable 0
	_delay_us(1);//wait for data to set up
	//end of capture high nibble
	
	//discard low nibble
	LCD_toggleEnable();
		
	LCD_DATADIR  |= (1 << D3) | (1 << D2) | (1 << D1) | (1 << D0);//set data output for write read operation
	
	return returnable;
}

/*
* Function: LCD_setPortData
* ----------------------------
* LCD_setPortData used to set data line nibble to match actual i/o port pins.
* nibble: nibble parameter must be sent upper 4 bits as 0 and lower 4 bits as line data.
*/
static uint8_t LCD_setPortData(uint8_t nibble)
{
	uint8_t returnable = 0;
	
	if(nibble & 0x01) returnable |= (1 << D0);
	if(nibble & 0x02) returnable |= (1 << D1);
	if(nibble & 0x04) returnable |= (1 << D2);
	if(nibble & 0x08) returnable |= (1 << D3);
	
	return returnable;
}
