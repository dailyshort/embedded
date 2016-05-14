.include "tn4313def.inc"

.org 0x0000
	rjmp SETUP

.org 0x0006 << 1;Timer 0 overflow vector
	push r16
	push r17
	in r16, PORTD
	ldi r17, 1 << PD6
	eor r16, r17
	out PORTD, r16
	pop r17
	pop r16
	reti

.org 0x0015 << 1
SETUP:
	ldi r16, 0x00;Init ports
	out DDRB, r16
	ldi r16, 1 << PD6
	out DDRD, r16

	ldi r17, 0xFF; enable pull-up-resistors
	out PORTB, r17
	ldi r17, (0xFF ^ (1 << PD6))
	out PORTD, r17

	ldi r17, HIGH(RAMEND);Init SP
	out sph, r17
	ldi r17, LOW(RAMEND)
	out spl, r17

	ldi r16, 0x00
	out TCCR0A, r16;set-up timer0
	ldi r16, 1 << TOIE0
	out TIMSK, r16
	ldi r16, 0x00
	out TIFR, r16

	sei;enable interrupts

	ldi r16, 1 << CS02 | 1 << CS00 ;Start timer 0 with 1024 prescale
	out TCCR0B, r16
MAIN:
	sleep	
	rjmp MAIN