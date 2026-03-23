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
#include "uart.h"

#define BAUD 9600
#define UBRR_VAL (F_CPU/16/BAUD - 1)

int main(void) {
	gpio_init();
	adc_init();
	uart_init(UBRR_VAL);

	uint16_t ldr_val, mq135_val;

	while (1) {
		ldr_val   = adc_read(ADC_LDR);
		mq135_val = adc_read(ADC_MQ135);

		uart_send_string("LDR: ");
		uart_send_uint16(ldr_val);
		uart_send_string(" | MQ135: ");
		uart_send_uint16(mq135_val);
		uart_send_string("\r\n");

		if (ldr_val < 512) {
			led_set(0, 0, 1);
			} else {
			led_set(1, 0, 0);
		}

		if (mq135_val > 700) {
			buzzer_on();
		}

		_delay_ms(500);
	}
}

