/*
 * uart.c
 *
 * Created: 23.03.2026 21:18:40
 *  Author: MSI
 */ 

#include <avr/io.h>
#include "uart.h"
#include <util/delay.h>

// uint16_t -> string
uint8_t uart_uint16_to_str(uint16_t val, char *buf){
	uint8_t len = 0;
	char tmp[6];
	uint8_t i = 0;
	
	if (val == 0){ 
		buf[0] = '0'; 
		buf[1] = '\0'; 
		return 1; 
	}
	// sirayi tersleme
	while (val > 0){
		tmp[i++] = '0' + (val % 10);
		val /= 10;
	}
	// dogru siraya cevirme
	for (uint8_t j = 0; j < i; j++) buf[j] = tmp[i-1-j];
	
	buf[i] = '\0';
	return i; // donus: strinf uzunlugu
}

// START
void uart_init(uint16_t ubrr) {
	UBRR0H = (ubrr >> 8); // baudrate - UBBR_VAL
	UBRR0L = ubrr;
	// alici ve verici etkinlestirme
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	// 8 bit, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// tek char alma
uint8_t uart_receive_char(void){
	uint32_t timeout = 50000000UL;
	
	// veri gelene veya timeout dolana kadar bekle
	while ( !(UCSR0A & (1 << RXC0)) && --timeout);
	if (timeout == 0) return '?'; // timout icin hata
	return UDR0; // donus: gelen char
}

// tek char gonderme
void uart_send_char(char c){
	// reg bosalana kadar bekle
	while ( !(UCSR0A & (1 << UDRE0)) );
	UDR0 = c;
}

// string atma
void uart_send_string(const char *str){
	while (*str) {
		uart_send_char(*str++);
	}
}

// uin16_t sayi gonderme
void uart_send_uint16(uint16_t val){
	char buf[6];
	buf[5] = '\0';
	buf[4] = '0' + (val % 10); val /= 10;
	buf[3] = '0' + (val % 10); val /= 10;
	buf[2] = '0' + (val % 10); val /= 10;
	buf[1] = '0' + (val % 10); val /= 10;
	buf[0] = '0' + (val % 10);
	// bastaki sifirlar atlansin
	char *p = buf;
	while (*p == '0' && *(p+1) != '\0') p++;
	uart_send_string(p);
}

uint8_t uart_read_response(char *buf, uint8_t max_len, uint16_t wait_ms){ // max_len: buffer boyutu, wait_ms: toplam bekleme suresi
	uint8_t i = 0;

	while (wait_ms--) {
		uint16_t inner = 0;

		while (UCSR0A & (1 << RXC0)){ // veri oldugu surece
			char c = UDR0;
			if (i < (max_len - 1)){ // buff tasmasi
				buf[i++] = c;
			}
		}

		_delay_ms(1);

		// kisa bir sure veri geldiyse devam et
		inner++;
		(void)inner;
	}

	buf[i] = '\0';
	return i; // donus: okunan toplam char sayisi
}

// metinde belirli bir anahtar kelime var mi
uint8_t uart_contains(const char *buf, const char *key) {
	uint8_t i, j;

	for (i = 0; buf[i] != '\0'; i++) {
		j = 0;
		while (key[j] != '\0' && buf[i + j] == key[j]) {
			j++;
		}
		if (key[j] == '\0') {
			return 1;
		}
	}
	return 0; // donus: 1=var, 0=yok
}