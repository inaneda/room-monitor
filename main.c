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
#include "bme280.h"

int main(void) {
	gpio_init();
	adc_init();
	i2c_init();
	lcd_init();
	bme280_init();

	uint16_t ldr_val, mq135_val;
	BME280_Data env;
	while (1) {
		env = bme280_read();
		
		ldr_val   = adc_read(ADC_LDR);
		mq135_val = adc_read(ADC_MQ135);

// LCD - SATIR 1: SICAKLIK NEM BASINC
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_print("T:"); // sicaklik
		lcd_print_uint16((uint16_t)env.temperature);
		lcd_print("C H:"); // celsius sonra nem
		lcd_print_uint16((uint16_t)env.humidity);
		lcd_print("%"); // nemin yuzdesi
		lcd_set_cursor(1, 0);
		lcd_print("P:"); // basinc
		lcd_print_uint16((uint16_t)env.pressure);
		lcd_print("hPa"); // basinc 
		_delay_ms(2000);
		
// LCD - Satir 2: LDR (ISIK) MQ135 (HAVA KALITESI)
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_print("LDR:");
		lcd_print_uint16(ldr_val);
		lcd_set_cursor(1, 0);
		lcd_print("MQ135:");
		lcd_print_uint16(mq135_val);
		_delay_ms(2000);

// LED VE BUZZER MANTIGI
		if (mq135_val > 540 || env.temperature > 26) {
			// R-LED: hava kalitesi kotu veya cok sicak + buzzer
			led_set(1, 0, 0);
			buzzer_on();
		} else if (mq135_val > 450 || env.temperature < 18 || env.humidity > 60 || env.humidity < 30 || ldr_val > 800 || ldr_val < 350) {
			// Y-LED: uyari seviyesi
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