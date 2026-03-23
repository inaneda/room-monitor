/*
 * room-monitor.c
 *
 * Created: 22.03.2026 21:50:13
 * Author : MSI
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

int main(void) {
	gpio_init();

	while (1) {
		led_set(0, 0, 1);   // yeþil
		_delay_ms(1000);
		led_set(0, 1, 0);   // sarý
		_delay_ms(1000);
		led_set(1, 0, 0);   // kýrmýzý + buzzer
		buzzer_on();
		_delay_ms(1000);
		buzzer_off();
		led_set(0, 0, 0);
		_delay_ms(500);
	}
}

