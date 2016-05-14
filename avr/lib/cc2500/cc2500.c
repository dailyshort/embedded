#include <avr/io.h>
#include "cc2500.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

/* CC2500 private function declarations*/
static void set_chip_select(uint8_t); //SPI chip select logic level
static void wait_rx_pin_low(void);
static void CC2500_burst_access(uint8_t addrAND_mode, uint8_t *buffer, uint8_t bytes);
static inline void chip_reset(void); // CC2500 power on reset procedure
static inline void init_register_settings(void); //initialize cc2500 operation registers

static uint8_t CC2500_single_access(uint8_t addrANDmode, uint8_t data);

/*CC2500 global variables*/
spi_readwrite_cb spi_putANDread; //callback to SPI r+w implementation
spi_sniff_rx_pin_cb spi_rx_sniff; //callback cc2500 ready notify implementation

/* configuration data taken from cc2500.h */
const char configurationData[] PROGMEM = 
{
	IOCFG2,  IOCFG1,  IOCFG0,   FIFOTHR,   SYNC1,
	SYNC0,   PKTLEN,  PKTCTRL1, PKTCTRL0,  ADDR,
	CHANNR,  FSCTRL1, FSCTRL0,  FREQ2,     FREQ1,
	FREQ0,   MDMCFG4, MDMCFG3,  MDMCFG2,   MDMCFG1,
	MDMCFG0, DEVIATN, MCSM2,    MCSM1,     MCSM0,
	FOCCFG,  BSCFG,   AGCCTRL2, AGCCTRL1,  AGCCTRL0,
	WOREVT1, WOREVT0, WORCTRL,  FREND1,    FREND0,
	FSCAL3,  FSCAL2,  FSCAL1,   FSCAL0,	   RCCTRL1,
	RCCTRL0,
};



void CC2500_init(spi_readwrite_cb spi_rw, spi_sniff_rx_pin_cb spi_sniff)
{
	spi_putANDread = spi_rw; //store spi procedure callback
	spi_rx_sniff = spi_sniff; //store spi sniff procedure callback
	
	CC2500_CS_DIR |= (1 << CC2500_CS_PIN); //set CS as output IO
	set_chip_select(1);//pull cs high

	chip_reset();//CC2500 power on reset
	init_register_settings();//write cc2500 register values
}



uint8_t CC2500_write_strobe(uint8_t strobe)
{	
	uint8_t status_frame;
	
	set_chip_select(0); //cs low
		
	/* wait until spi rx pin goes low */
	wait_rx_pin_low();
		
	status_frame = spi_putANDread(strobe); //write strobe
	
	set_chip_select(1);//cs high
	
	return status_frame;
}



void CC2500_write_register(uint8_t addr, uint8_t data)
{
	CC2500_single_access(addr | CC2500_WRITE, data);
}



uint8_t CC2500_read_register(uint8_t addr)
{	
	return CC2500_single_access(addr | CC2500_READ, 0);
}



uint8_t CC2500_read_status_register(uint8_t addr)
{
	return CC2500_single_access(addr | CC2500_BURST | CC2500_READ, 0);
}



void CC2500_write_burst(uint8_t start_addr, uint8_t *buffer, uint8_t bytes)
{	
	 CC2500_burst_access(start_addr | CC2500_WRITE | CC2500_BURST,
						 buffer, bytes);
}



void CC2500_read_burst(uint8_t start_addr, uint8_t *buffer, uint8_t bytes)
{
	CC2500_burst_access(start_addr | CC2500_READ | CC2500_BURST,
						buffer, bytes);
}



void CC2500_sendRF_payload(uint8_t *buffer, uint8_t bytes)
{
	CC2500_write_strobe(CC2500_SIDLE); //set idle
	CC2500_write_strobe(CC2500_SFTX); //flush tx fifo buffer
	CC2500_write_register(CC2500_FIFO, bytes); //packet length
	CC2500_write_burst(CC2500_FIFO, buffer, bytes);  //write data to FIFO				   
	CC2500_write_strobe(CC2500_STX); //send packet
}



static uint8_t CC2500_single_access(uint8_t addrANDmode, uint8_t data)
{
	uint8_t reg_content;
	
	set_chip_select(0); //cs low
	
	/* wait until spi rx pin goes low */
	wait_rx_pin_low();
	
	spi_putANDread(addrANDmode); //write addr
	reg_content = spi_putANDread(data); //read data
	
	set_chip_select(1);//cs high
	
	return reg_content;
}



static void CC2500_burst_access(uint8_t addrAND_mode, uint8_t *buffer, uint8_t bytes)
{
	uint8_t i = 0; //buffer index
	
	set_chip_select(0); //cs low
	
	/* wait until spi rx pin goes low */
	wait_rx_pin_low();
	
	spi_putANDread(addrAND_mode); //write address and mode
	
	if(addrAND_mode & CC2500_READ) //if read mode
	{
		while(i < bytes)
		{
			*(buffer + i) = spi_putANDread(0x00);
			i++;
		}		
	}
	else //write mode
	{
		while(i < bytes)
		{
			spi_putANDread(*(buffer + i));
			i++;	
		}
	}
	
	set_chip_select(1);
}	



static void set_chip_select(uint8_t pin_value)
{
	_delay_us(200);
	
	if(pin_value)
	{
		CC2500_CS_PORT |= (1 << CC2500_CS_PIN); //pull CS high
	}
	else
	{
		CC2500_CS_PORT &= ~(1 << CC2500_CS_PIN); //pull CS low
	}
	
	_delay_us(200);
}


static void wait_rx_pin_low(void)
{
	/* wait until spi rx pin goes low */
	while(spi_rx_sniff())
	{
	}
}



/* CC2500 reset procedure.
   See CC2500 manual, page 40 */
static inline void chip_reset(void)
{
	/* cc2500 reset chip select toggle */
	CC2500_CS_PORT &= ~(1 << CC2500_CS_PIN); //pull CS low
	_delay_us(2);
	CC2500_CS_PORT |= (1 << CC2500_CS_PIN); //pull CS high
	_delay_us(40);
	
	CC2500_write_strobe(CC2500_SRES); //reset chip
	CC2500_write_strobe(CC2500_SIDLE); //set chip in idle state
}



static inline void init_register_settings(void)
{
	uint8_t i = 0, j = sizeof(configurationData);

	set_chip_select(0); //cs low
	
	//wait until spi rx pin goes low
	wait_rx_pin_low();
	
	spi_putANDread(CC2500_IOCFG2 | CC2500_WRITE | CC2500_BURST); //write address in burst mode
	
	//write register contents
	while(i < j)
	{
		spi_putANDread(pgm_read_byte((configurationData + i)));
		i++;
	}
	
	set_chip_select(1);
	
	CC2500_write_register(CC2500_PATABLE, PWR_SELECT);
}
