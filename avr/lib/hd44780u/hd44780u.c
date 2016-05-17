#include <avr/io.h>
#include "myLCD.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

//myLCD.c private function prototypes
uint8_t LCD_setPortData(uint8_t nibble);
uint8_t LCD_isBusy(void);
void LCD_toggleEnable(void);
//end of myLCD.c private function prototypes


/*
* LCD_init set up avr io initalization defined in myLCD.h.
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
	//END LCD init procedure. check flowchart at HD4470 datasheet Figure 24 4-Bit Interface page 46
}

/*
* LCD_toggleEnable is function for toggling LCD enable line
*/
void LCD_toggleEnable(void)
{
	_delay_us(1);//wait for signals to set up
	LCD_COMMANDPORT |= (1 << EN);// 1 for enable
	_delay_us(2);//wider pulse
	LCD_COMMANDPORT &= ~ (1 << EN);//Enable 0
	_delay_us(1);//wait for LCD to acknowledge disable
}

/*
* LCD_read is function to read LCD module
* memory contents
*/
uint8_t LCD_isBusy(void)
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
* LCD_command function writes data to LCD
* command register
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
* LCD_write write a single data byte to lcd module.
* All write operation take place as raw datawrite.
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
* LCD_setPort data is a private function
* which manipulates nibble to match actual
* i/o port pins. nibble parameter must be sent
* upper 4 bits as 0 and lower 4 bits as data
*/
uint8_t LCD_setPortData(uint8_t nibble)
{
	uint8_t returnable = 0;
	
	if(nibble & 0x01) returnable |= (1 << D0);
	if(nibble & 0x02) returnable |= (1 << D1);
	if(nibble & 0x04) returnable |= (1 << D2);
	if(nibble & 0x08) returnable |= (1 << D3);
	
	return returnable;
}

/*
* LCD_putString writes constant string character data
* from progmem. Function calls LCD_write sizeof(string) times.
*/
void LCD_putString(const char *strPtr)
{
	while(pgm_read_byte(strPtr))
	{
		LCD_write(pgm_read_byte(strPtr++));
	}
}


/*
* LCD_Clear clears LCD screen
* and return cursor to home location
*/
void LCD_Clear(void)
{
	LCD_command(CLEAR);
	LCD_command(RETURN_HOME);
}


/*
* LCD_gotoXY moves lcd cursor to location given
* by x and y
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
* LCD_putHexByte writes number to
* LCD screen in hex format
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