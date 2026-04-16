/*
 * adc.c
 *
 * Created: 23.03.2026 21:12:52
 *  Author: MSI
 */ 

#include "adc.h"

void adc_init(void){
	// VCC referans voltaji
	ADMUX = (1 << REFS0);
	// ADC'yi etkinlestirme, prescaler 128 (16MHz/128 = 125kHz)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel){
	// kanal secimi
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// donusumu baslat
	ADCSRA |= (1 << ADSC);
	// donusumun bitmesini bekle
	while (ADCSRA & (1 << ADSC));
	return ADC;
}