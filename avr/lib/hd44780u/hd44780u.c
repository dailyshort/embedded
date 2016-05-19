#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hd44780u.h"

#define ASCII_NUMBER_OFFSET 0x30
#define ASCII_LETTER_OFFSET 0x37

static uint8_t hd44780u_prepareIOContents(uint8_t nibble);
static uint8_t hd44780u_isBusy(void);
static void hd44780u_toggleEnable(void);

/*
* Function: hd44780u_init
* ----------------------------
* Set up hd44780u controller io
* and implement startup command sequence to an module.
* this function must be called in main program before any other usage.
*/
void hd44780u_init(void)
{
	//port init
	HD44780U_DATAPORT    &= ~((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0));
	HD44780U_COMMANDPORT &= ~((1 << RS) | (1 << RW) | (1 << EN));

	HD44780U_DATADIR    |= (1 << D3) | (1 << D2) | (1 << D1) | (1 << D0);
	HD44780U_COMMANDDIR |= (1 << RS) | (1 << RW) | (1 << EN);

	//START controller init procedure. check flowchart at HD4470 datasheet Figure 24 4-Bit Interface page 46
	_delay_ms(50);
	hd44780u_command(0x33);//lcd init
	hd44780u_command(0x32);//lcd init
	hd44780u_command(0x28);//4-bit bus mode + 2 line/5*8dots display set
	hd44780u_command(0x0E);//Display on, cursor on, cursor blink off
	hd44780u_command(0x01);//clear display
	hd44780u_command(0x06);//cursor advances right
}

/*
* Function: hd44780u_command
* ----------------------------
* Writes single command byte to the controller command register.
* command: See HD4470 datasheet for available commands.
*/
void hd44780u_command(uint8_t command)
{
	while(hd44780u_isBusy())
	{
		_delay_us(40);
	}

	HD44780U_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear io datalines
	HD44780U_DATAPORT |= hd44780u_prepareIOContents((command & 0xF0) >> 4);//send High nibble

	HD44780U_COMMANDPORT &= ~ (1 << RS);// 0 for command
	HD44780U_COMMANDPORT &= ~ (1 << RW);// 0 for write

	hd44780u_toggleEnable();

	HD44780U_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear io datalines
	HD44780U_DATAPORT |= hd44780u_prepareIOContents((command & 0x0F));//send low nibble

	hd44780u_toggleEnable();
}

/*
* Function: hd44780u_write
* ----------------------------
* Writes a single raw character controller attached lcd module.
* data: See HD4470 datasheet for available characters.
*/
void hd44780u_write(uint8_t data)
{
	while(hd44780u_isBusy())
	{
		_delay_us(40);
	}

	HD44780U_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear io datalines
	HD44780U_DATAPORT |= hd44780u_prepareIOContents((data & 0xF0) >> 4);//send High nibble

	HD44780U_COMMANDPORT |= (1 << RS);// 1 for data
	HD44780U_COMMANDPORT &= ~ (1 << RW);// 0 for write

	hd44780u_toggleEnable();

	HD44780U_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear LCD datalines
	HD44780U_DATAPORT |= hd44780u_prepareIOContents(data & 0x0F);//send low nibble

	hd44780u_toggleEnable();
}

/*
* Function: hd44780u_putString
* ----------------------------
* Writes string of characters to controller attached lcd module.
* strPtr: a null terminated constant string that MUST be pointing to program memory region.
*/
void hd44780u_putString(const char *strPtr)
{
	while(pgm_read_byte(strPtr))
	{
		hd44780u_write(pgm_read_byte(strPtr++));
	}
}

/*
* Function: hd44780u_clear
* ----------------------------
* Clears controller lcd contents and return cursor to home location.
*/
void hd44780u_clear(void)
{
	hd44780u_command(CLEAR);
	hd44780u_command(RETURN_HOME);
}

/*
* Function: hd44780u_gotoXY
* ----------------------------
* Moves controller lcd cursor to given location.
* x: horizontal cursor pos
* y: vertical cursor pos
*/
void hd44780u_gotoXY(uint8_t x, uint8_t y)
{
	const uint8_t lines[] = { 0x80, 0xC0, 0x94, 0xD4 };//0x80 + module line start address code
	#if LCD_ROWS > 4
	#error "over 4 row lcd not supported"
	#endif
	hd44780u_command(lines[y] + x);
}

/*
* Function: hd44780u_putHexByte
* ----------------------------
* Writes given byte to controller lcd as hex with format of 0xDD.
* hexByte:
*/
void hd44780u_putHexByte(uint8_t hexByte)
{
	uint8_t upper = ((hexByte & 0xF0) >> 4);
	uint8_t lower = (hexByte & 0x0F);

	if(upper < 0x0A) upper += ASCII_NUMBER_OFFSET;//ascii number offset
	else upper += ASCII_LETTER_OFFSET;//ascii capital letter offset

	if(lower < 0x0A) lower += ASCII_NUMBER_OFFSET;//ascii number offset
	else lower += ASCII_LETTER_OFFSET;//ascii capital letter offset

	hd44780u_write('0');
	hd44780u_write('x');
	hd44780u_write(upper);
	hd44780u_write(lower);
}

/*
* Function: hd44780u_toggleEnable
* ----------------------------
* Toggles enable line.
*/
static void hd44780u_toggleEnable(void)
{
	_delay_us(1);//wait for signals to set up
	LCD_COMMANDPORT |= (1 << EN);// 1 for enable
	_delay_us(2);//wider pulse
	HD44780U_COMMANDPORT &= ~ (1 << EN);//Enable 0
	_delay_us(1);//wait for LCD to acknowledge disable
}

/*
* Function: hd44780u_isBusy
* ----------------------------
* Check if module is busy and cannot accept new messages.
* returns: 0 if not busy, 1 if busy.
*/
static uint8_t hd44780u_isBusy(void)
{
	uint8_t returnable = 0;

	HD44780U_DATAPORT &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0)); //clear io datalines
	HD44780U_DATADIR  &= ~ ((1 << D3) | (1 << D2) | (1 << D1) | (1 << D0));//set data to input for read operation

	HD44780U_COMMANDPORT	&= ~ (1 << RS);// 0 for command
	HD44780U_COMMANDPORT |=   (1 << RW);// 1 for read

	//capture high nibble
	_delay_us(1);//wait for data to set up
	HD44780U_COMMANDPORT |= (1 << EN);// 1 for enable
	_delay_us(2);//wider pulse
	if(HD44780U_DATAREAD & (1 << D3))
        returnable = 1;//set BusyFlag
	HD44780U_COMMANDPORT &= ~ (1 << EN);//Enable 0
	_delay_us(1);//wait for data to set up
	//end of capture high nibble

	//discard low nibble
	hd44780u_toggleEnable();

	HD44780U_DATADIR  |= (1 << D3) | (1 << D2) | (1 << D1) | (1 << D0);//set data output for write read operation

	return returnable;
}

/*
* Function: hd44780u_prepareIOContents
* ----------------------------
* Used to set data line nibble to match actual i/o port pins.
* nibble: nibble parameter must be sent upper 4 bits as 0 and lower 4 bits as line data.
* returns: a nibble repositioned according to lcd module data line pins.
*/
static uint8_t hd44780u_prepareIOContents(uint8_t nibble)
{
	uint8_t returnable = 0;

	if(nibble & 0x01) returnable |= (1 << D0);
	if(nibble & 0x02) returnable |= (1 << D1);
	if(nibble & 0x04) returnable |= (1 << D2);
	if(nibble & 0x08) returnable |= (1 << D3);

	return returnable;
}
