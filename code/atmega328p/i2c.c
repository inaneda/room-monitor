/*
 * i2c.c
 *
 * Created: 23.03.2026 22:23:38
 *  Author: MSI
 */ 

#include "i2c.h"
#include <util/twi.h>

void i2c_init(void){
	TWSR = 0x00; // prescaler = 1
	TWBR = ((F_CPU / 100000UL) - 16) / 2; // scl yaklasik 100kHz
	TWCR = (1 << TWEN); // TWI etkinlestir
}

uint8_t i2c_start(uint8_t addr_rw){
	// START
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	// tamamlana kadar bekle
	while ( !(TWCR & (1<<TWINT)) );
	// durum okuma
	uint8_t s = TWSR & 0xF8;
	if (s != TW_START && s != TW_REP_START) return 1;

	// adres gönder
	TWDR = addr_rw;
	TWCR = (1<<TWINT)|(1<<TWEN);
	// gonderimin tamamlanmasini bekle
	while ( !(TWCR & (1<<TWINT)) );
	// slave ack dondu mu
	s = TWSR & 0xF8;
	if (s != TW_MT_SLA_ACK && s != TW_MR_SLA_ACK) return 1;
	return 0; // donus: 0 = basarili, 1 = hata
}

void i2c_stop(void){
	// STOP
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

// slave'e 1 byte veri yazma
uint8_t i2c_write(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ( !(TWCR & (1<<TWINT)) );
	return (TWSR & 0xF8); // donus: TWI durum kodu
}

// 1 byte veri okuma ve ACK gonderme
uint8_t i2c_read_ack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	// yazmanin tamamlanmasini bekle
	while ( !(TWCR & (1<<TWINT)) );
	return TWDR;
}

// 1 byte veri okuma ve NACK gonderme
uint8_t i2c_read_nack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN);
	// okumanin tamamlanmasini bekle
	while ( !(TWCR & (1<<TWINT)) );
	return TWDR;
}