/*
 * adc.c
 *
 * Created: 23.03.2026 21:12:52
 *  Author: MSI
 */ 

#include "adc.h"

void adc_init(void) {
	// AVcc referans voltajý seç
	ADMUX = (1 << REFS0);
	// ADC'yi etkinleþtir, prescaler 128 (16MHz/128 = 125kHz)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
	// Kanalý seç (alt 4 bit)
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// Dönüþümü baþlat
	ADCSRA |= (1 << ADSC);
	// Dönüþüm bitene kadar bekle
	while (ADCSRA & (1 << ADSC));
	return ADC;
}