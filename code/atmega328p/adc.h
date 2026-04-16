/*
 * adc.h
 *
 * Created: 23.03.2026 21:12:41
 *  Author: MSI
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

// kanal tanimlari
#define ADC_LDR     0   // A0
#define ADC_MQ135   1   // A1

// fonksiyonlar
void adc_init(void);
uint16_t adc_read(uint8_t channel);

#endif /* ADC_H_ */