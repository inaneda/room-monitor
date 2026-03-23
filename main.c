/*
 * room-monitor.c
 *
 * Created: 22.03.2026 21:50:13
 * Author : MSI
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"
#include "adc.h"
#include "i2c.h"
#include "lcd.h"

int main(void) {
	gpio_init();
	adc_init();
	i2c_init();
	lcd_init();

	uint16_t ldr_val, mq135_val;

	while (1) {
		ldr_val   = adc_read(ADC_LDR);
		mq135_val = adc_read(ADC_MQ135);

		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_print("LDR:");
		lcd_print_uint16(ldr_val);

		lcd_set_cursor(1, 0);
		lcd_print("MQ:");
		lcd_print_uint16(mq135_val);
		

		if (mq135_val > 470) {
			// R-LED: hava kalitesi kotu + buzzer
			led_set(1, 0, 0);
			buzzer_on();
			} else if (mq135_val > 450) {
			// Y-LED: hava kalitesi uyari seviyesi
				led_set(0, 1, 0);
				buzzer_off();
			} else if (ldr_val > 800) {
			// Y-LED: isik fazla
				led_set(0, 1, 0);
				buzzer_off();
			} else if (ldr_val < 350) {
			// Y-LED: kaarnlik
				led_set(0, 1, 0);
				buzzer_off();
			} else {
			// G-LED: normal
				led_set(0, 0, 1);
				buzzer_off();
		}

		_delay_ms(500);
	}
}