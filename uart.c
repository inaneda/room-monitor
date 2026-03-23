/*
 * uart.c
 *
 * Created: 23.03.2026 21:18:40
 *  Author: MSI
 */ 

#include <avr/io.h>
#include "uart.h"

void uart_init(uint16_t ubrr) {
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	// Alýcý ve verici etkinleþtir
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	// 8 bit, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_char(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_send_string(const char *str) {
	while (*str) {
		uart_send_char(*str++);
	}
}

void uart_send_uint16(uint16_t val) {
	char buf[6];
	buf[5] = '\0';
	buf[4] = '0' + (val % 10); val /= 10;
	buf[3] = '0' + (val % 10); val /= 10;
	buf[2] = '0' + (val % 10); val /= 10;
	buf[1] = '0' + (val % 10); val /= 10;
	buf[0] = '0' + (val % 10);
	// Baþtaki sýfýrlarý atla
	char *p = buf;
	while (*p == '0' && *(p+1) != '\0') p++;
	uart_send_string(p);
}