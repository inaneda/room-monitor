/*
 * gpio.c
 *
 * Created: 22.03.2026 21:54:25
 *  Author: MSI
 */ 

#include "gpio.h"
#include <util/delay.h>

void gpio_init(void) {
	// LED pinleri cikis
	DDRD |= (1 << LED_RED_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_GREEN_PIN);
	// Buzzer pini cikis
	DDRB |= (1 << BUZZER_PIN);
	// default olarak kapalilar
	PORTD &= ~((1 << LED_RED_PIN) | (1 << LED_YELLOW_PIN) | (1 << LED_GREEN_PIN));
	PORTB &= ~(1 << BUZZER_PIN);
}

void led_set(uint8_t red, uint8_t yellow, uint8_t green) {
	if (red)    PORTD |=  (1 << LED_RED_PIN);
	else        PORTD &= ~(1 << LED_RED_PIN);

	if (yellow) PORTD |=  (1 << LED_YELLOW_PIN);
	else        PORTD &= ~(1 << LED_YELLOW_PIN);

	if (green)  PORTD |=  (1 << LED_GREEN_PIN);
	else        PORTD &= ~(1 << LED_GREEN_PIN);
}

void buzzer_on(void) {
	for (int i = 0; i < 1000; i++) {
		PORTB |=  (1 << BUZZER_PIN);
		_delay_us(500);   // 1kHz kare dalga
		PORTB &= ~(1 << BUZZER_PIN);
		_delay_us(500);
	}
}
void buzzer_off(void) {
	PORTB &= ~(1 << BUZZER_PIN);
}
