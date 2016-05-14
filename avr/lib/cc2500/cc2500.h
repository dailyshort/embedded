#ifndef CC2500_H_
#define CC2500_H_

/******************THIS BLOCK DEFINE HOW DEVICE SHOULD OPERATE***************************/
/* MCU cpu settings */
#define F_CPU 1000000UL

/* CC2500 SPI CS */
#define CC2500_CS_PIN      PORTB4
#define CC2500_CS_DIR      DDRB
#define CC2500_CS_PORT	   PORTB

/* CC2500 register content. */
#define IOCFG2     0x29       
#define IOCFG1     0x2E     
#define IOCFG0     0x07
#define FIFOTHR    0x07
#define SYNC1      0xD3     
#define SYNC0      0x91  
#define PKTLEN	   0xFF
#define PKTCTRL1   0x04	  
#define PKTCTRL0   0x05	  
#define ADDR	   0x01  
#define CHANNR	   0x00 
#define FSCTRL1	   0x07
#define FSCTRL0	   0x00
#define FREQ2	   0x5D
#define FREQ1	   0x93
#define FREQ0	   0xB1
#define MDMCFG4	   0x2D
#define MDMCFG3    0x3B   
#define MDMCFG2    0x73
#define MDMCFG1    0x22  
#define MDMCFG0    0xF8   
#define DEVIATN    0x00   
#define MCSM2      0x07   
#define MCSM1      0x3F   
#define MCSM0      0x18   
#define FOCCFG     0x1D   
#define BSCFG      0x1C   
#define AGCCTRL2   0xC7
#define AGCCTRL1   0x00   
#define AGCCTRL0   0xB2 
#define WOREVT1    0x87    
#define WOREVT0	   0x6B
#define WORCTRL	   0xF8   
#define FREND1	   0xB6
#define FREND0	   0x10 
#define FSCAL3	   0xEA
#define FSCAL2	   0x0A
#define FSCAL1	   0x00 
#define FSCAL0	   0x11  
#define RCCTRL1	   0x41	  
#define RCCTRL0	   0x00
#define PWR_SELECT 0xFF
			
/* SPI operation callback*/
typedef uint8_t (*spi_readwrite_cb)(uint8_t data);

/* SPI rx pin sniff callback. */
typedef uint8_t (*spi_sniff_rx_pin_cb)(void);
/****************************************************************************************/


/*--------CC2500 register definitions--------*/
#define CC2500_IOCFG2         0x00 // GDO2 output pin configuration
#define CC2500_IOCFG1         0x01 // GDO1 output pin configuration
#define CC2500_IOCFG0         0x02 // GDO0 output pin configuration
#define CC2500_FIFOTHR        0x03 // RX FIFO and TX FIFO thresholds
#define CC2500_SYNC1          0x04 // Sync word, high byte
#define CC2500_SYNC0          0x05 // Sync word, low byte
#define CC2500_PKTLEN		  0x06 // Packet length
#define CC2500_PKTCTRL1		  0x07 // Packet automation control
#define CC2500_PKTCTRL0		  0x08 // Packet automation control
#define CC2500_ADDR			  0x09 // Device address
#define CC2500_CHANNR		  0x0A // Channel number
#define CC2500_FSCTRL1		  0x0B // Frequency synthesizer control
#define CC2500_FSCTRL0		  0x0C // Frequency synthesizer control
#define CC2500_FREQ2		  0x0D // Frequency control word, high byte
#define CC2500_FREQ1		  0x0E // Frequency control word, middle byte
#define CC2500_FREQ0		  0x0F // Frequency control word, low byte
#define CC2500_MDMCFG4		  0x10 // Modem configuration
#define CC2500_MDMCFG3        0x11 // Modem configuration
#define CC2500_MDMCFG2        0x12 // Modem configuration
#define CC2500_MDMCFG1        0x13 // Modem configuration
#define CC2500_MDMCFG0        0x14 // Modem configuration
#define CC2500_DEVIATN        0x15 // Modem deviation setting
#define CC2500_MCSM2          0x16 // Main Radio Control State Machine configuration
#define CC2500_MCSM1          0x17 // Main Radio Control State Machine configuration
#define CC2500_MCSM0          0x18 // Main Radio Control State Machine configuration
#define CC2500_FOCCFG         0x19 // Frequency Offset Compensation configuration
#define CC2500_BSCFG          0x1A // Bit Synchronization configuration
#define CC2500_AGCCTRL2       0x1B // AGC control
#define CC2500_AGCCTRL1       0x1C // AGC control
#define CC2500_AGCCTRL0       0x1D // AGC control
#define CC2500_WOREVT1        0x1E // High byte Event 0 timeout
#define CC2500_WOREVT0		  0x1F // Low byte Event 0 timeout
#define CC2500_WORCTRL	      0x20 // Wake On Radio control
#define CC2500_FREND1		  0x21 // Front end RX configuration
#define CC2500_FREND0		  0x22 // Front end TX configuration
#define CC2500_FSCAL3		  0x23 // Frequency synthesizer calibration
#define CC2500_FSCAL2		  0x24 // Frequency synthesizer calibration
#define CC2500_FSCAL1		  0x25 // Frequency synthesizer calibration
#define CC2500_FSCAL0		  0x26 // Frequency synthesizer calibration
#define CC2500_RCCTRL1		  0x27 // RC oscillator configuration
#define CC2500_RCCTRL0		  0x28 // RC oscillator configuration
#define CC2500_FSTEST		  0x29 // Frequency synthesizer calibration control
#define CC2500_PTEST		  0x2A // Production test
#define CC2500_AGCTEST		  0x2B // AGC test
#define CC2500_TEST2		  0x2C // Various test settings
#define CC2500_TEST1		  0x2D // Various test settings
#define CC2500_TEST0	      0x2E // Various test settings


/*--------CC2500 Status register definitions--------*/
#define CC2500_PARTNUM        0x30 // CC2500 part number
#define CC2500_VERSION        0x31 // Current version number
#define CC2500_FREQEST        0x32 // Frequency offset estimate
#define CC2500_LQI            0x33 // Demodulator estimate for Link Quality
#define CC2500_RSSI           0x34 // Received signal strength indication
#define CC2500_MARCSTATE      0x35 // Control state machine state
#define CC2500_WORTIME1       0x36 // High byte of WOR timer
#define CC2500_WORTIME0       0x37 // Low byte of WOR timer
#define CC2500_PKTSTATUS      0x38 // Current GDOx status and packet status
#define CC2500_VCO_VC_DAC     0x39 // Current setting from PLL calibration module
#define CC2500_TXBYTES        0x3A // Underflow and number of bytes in the TX FIFO
#define CC2500_RXBYTES        0x3B // Overflow and number of bytes in the RX FIFO
#define CC2500_RCCTRL1_STATUS 0x3C // Last RC oscillator calibration result
#define CC2500_RCCTRL0_STATUS 0x3D // Last RC oscillator calibration result


/*--------CC2500 other access definitions--------*/
#define CC2500_PATABLE		  0x3E //patable access
#define CC2500_FIFO			  0x3F //FIFO region access

/*--------CC2500 Command Strobes--------*/
#define CC2500_SRES           0x30 // Reset chip.
#define CC2500_SFSTXON        0x31 // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
								   // If in RX/TX: Go to a wait state where only the synthesizer is
								   // running (for quick RX / TX turnaround).
#define CC2500_SXOFF          0x32 // Turn off crystal oscillator.
#define CC2500_SCAL           0x33 // Calibrate frequency synthesizer and turn it off
								   // (enables quick start).
#define CC2500_SRX            0x34 // Enable RX. Perform calibration first if coming from IDLE and
								   // MCSM0.FS_AUTOCAL=1.
#define CC2500_STX            0x35 // In IDLE state: Enable TX. Perform calibration first if
								   // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
								   // Only go to TX if channel is clear.
#define CC2500_SIDLE          0x36 // Exit RX / TX, turn off frequency synthesizer and exit
								   // Wake-On-Radio mode if applicable.
#define CC2500_SAFC           0x37 // Perform AFC adjustment of the frequency synthesizer
#define CC2500_SWOR           0x38 // Start automatic RX polling sequence (Wake-on-Radio)
#define CC2500_SPWD           0x39 // Enter power down mode when CSn goes high.
#define CC2500_SFRX           0x3A // Flush the RX FIFO buffer.
#define CC2500_SFTX           0x3B // Flush the TX FIFO buffer.
#define CC2500_SWORRST        0x3C // Reset real time clock.
#define CC2500_SNOP           0x3D // No operation. May be used to pad strobe commands to two
								   // bytes for simpler software.
													   
/*--------CC2500 header transmit/receive mask settings--------*/
#define CC2500_READ   0x80 //read mode
#define CC2500_WRITE  0x00 //write mode 
#define CC2500_BURST  0x40 //burst mode


/*--------CC2500 chip status header info--------*/

/* Idle state
(Also reported for some transitional states instead of SETTLING or CALIBRATE)*/
#define STATUS_IDLE(H)                 ((H & 0x70) == 0x00) 

/* The number of bytes available in the RX FIFO or free bytes in the TX FIFO
 (See data sheet, page 44)*/
#define STATUS_FIFO_BYTES_AVAILABLE(H) ((H & 0x08)	

 /* Receive mode */		
#define STATUS_RX(H)				   ((H & 0x70) == 0x10)

 /* Transmit mode */
#define STATUS_TX(H)                   ((H & 0x70) == 0x20)

/* Frequency synthesizer is on, ready to start transmitting */
#define STATUS_FSTXON(H)               ((H & 0x70) == 0x30)

/* Frequency synthesizer calibration is running */
#define STATUS_CALIBRATE(H)            ((H & 0x70) == 0x40)

/* PLL is settling */
#define STATUS_SETTLING(H)             ((H & 0x70) == 0x50)

/* RX FIFO has overflowed. Read out any useful data, then flush the FIFO with SFRX */
#define STATUS_RXFIFO_OVERFLOW(H)      ((H & 0x70) == 0x60)

/* TX FIFO has underflowed. Acknowledge with SFTX */
#define STATUS_TXFIFO_OVERFLOW(H)      ((H & 0x70) == 0x70)

/* Stays high until power and crystal have stabilized. Should always be low when using
the SPI interface*/
#define STATUS_CHIP_RDY(H)             ((H & 0x80) == 0x80)

								   
/*--------CC2500 function declarations--------*/
extern void CC2500_init(spi_readwrite_cb, spi_sniff_rx_pin_cb); //initialize CC2500 chip
extern void CC2500_write_register(uint8_t addr, uint8_t data); //write single register
extern void CC2500_write_burst(uint8_t start_addr, uint8_t *buffer, uint8_t bytes); //write multiple registers
extern void CC2500_read_burst(uint8_t start_addr, uint8_t *buffer, uint8_t bytes); //read multiple registers
extern void CC2500_sendRF_payload(uint8_t *buffer, uint8_t bytes);

extern uint8_t CC2500_write_strobe(uint8_t strobe); //write strobe command
extern uint8_t CC2500_read_register(uint8_t addr); //read single register
extern uint8_t CC2500_read_status_register(uint8_t addr); //read status register

#endif /* CC2500_H_ */
