// ======================================================================
// MicroMoo
//
// Copyright 2008 Tim Stegeman
//
// This is free software, licensed under the terms of the GNU General
// Public License as published by the Free Software Foundation.
// ======================================================================

#include <avr/io.h>
#include "usbtiny/usb.h"

#define USB_REQ_ECHO	0
#define USB_REQ_SET	1
#define USB_REQ_GET	2

#define PWM_RED		OCR0A
#define PWM_GREEN	OCR1AL
#define PWM_BLUE	OCR1BL

extern byte_t usb_setup(byte_t data[8]) {
	byte_t req = data[1];
	if (req == USB_REQ_ECHO) {
		return 8;
	}
	if (req == USB_REQ_SET) {
		PWM_RED = data[2];
		PWM_GREEN = data[3];
		PWM_BLUE = data[4];
		return 0;
	}
	if (req == USB_REQ_GET) {
		data[0] = PWM_RED;
		data[1] = PWM_GREEN;
		data[2] = PWM_BLUE;
		return 3;
	}
	return 0;
}

int main(void) {

	// TIMER 0 Fast PWM, clear OC0A on match
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	// TIMER 0 Prescaler ck/8
	TCCR0B = (1 << CS02);
	// TIMER 1 Fast PWM 8-bit, clear OC1A, OC1B on match
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	// TIMER 1 Prescaler ck/8
	TCCR1B = (1 << CS12) | (1 << WGM12);
	//PWM kanalen als uitgang
	DDRB = (1 << PB2) | (1 << PB3) | (1 << PB4); //0C0A, 0C1A, OC1B

	PWM_RED = 0;
	PWM_GREEN = 0;
	PWM_BLUE = 0;

	usb_init();
	for (;;) {
		usb_poll();
	}
	return 0;
}

