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
#include "uart.h"
#include "config.h"
#include "secrets.h"
#include "ds1307.h"

#define BAUD 9600
#define UBRR_VAL (F_CPU/16/BAUD - 1)

#define PAGE_HOLD_MS 4000 // bir sayfanin lcd'de bulunma suresi
#define PAGE_REFRESH_MS 250 // ayni sayfa icinde veri yenileme (gercek zamanli veri aktarimi icin)
#define PAGE_COUNT 4

/* ESP32'ye UART uzerinden veri gonderme */
void esp32_send_data(uint16_t temp, uint16_t hum, uint16_t pres, uint16_t ldr, uint16_t mq){
	// veri formati: T:24,H:51,P:1008,L:300,G:420
	uart_send_string("T:");
	uart_send_uint16(temp);
	uart_send_string(",H:");
	uart_send_uint16(hum);
	uart_send_string(",P:");
	uart_send_uint16(pres);
	uart_send_string(",L:");
	uart_send_uint16(ldr);
	uart_send_string(",G:");
	uart_send_uint16(mq);
	uart_send_string("\r\n");
}

// ortam durumuna gore LED ve buzzer
void update_status(BME280_Data *d, uint16_t mq){
	// kirmizi: hava kalitesi kotu
	if (mq > MQ135_THRESHOLD){
		led_set(1, 0, 0);
		buzzer_on();
		return;
	}
	// sair: sicaklik veya nem sinir disi
	if (d->temperature > TEMP_MAX || d->temperature < TEMP_MIN || d->humidity > HUM_MAX || d->humidity < HUM_MIN){
		led_set(0, 1, 0);
		buzzer_off();
		return;
	}
	// yesil: her sey normal
	led_set(0, 0, 1);
	buzzer_off();
}

// LCD'de tek sayfa sensor verisi yazma
void lcd_show_page(uint8_t page, BME280_Data *d, uint16_t ldr, uint16_t mq, DS1307_Time *t){
	
	lcd_clear();
	switch (page){
		case 0:
		// Ekran 1: Tarih + Saat
		lcd_set_cursor(0, 0);
		lcd_print_uint16(t->day);   lcd_print("/");
		lcd_print_uint16(t->month); lcd_print("/");
		lcd_print_uint16(t->year);

		lcd_set_cursor(1, 0);
		lcd_print_uint16(t->hour);   lcd_print(":");
		lcd_print_uint16(t->minute); lcd_print(":");
		lcd_print_uint16(t->second);
		break;

		case 1:
		// Ekran 2: Sicaklik + Nem ; Saat
		lcd_set_cursor(0, 0);
		lcd_print("T:");
		lcd_print_uint16((uint16_t)d->temperature);
		lcd_print("C H:");
		lcd_print_uint16((uint16_t)d->humidity);
		lcd_print("%");

		lcd_set_cursor(1, 0);
		lcd_print_uint16(t->hour);   lcd_print(":");
		lcd_print_uint16(t->minute); lcd_print(":");
		lcd_print_uint16(t->second);
		break;

		case 2:
		// Ekran 3: Isik seviyesi + Hava kalitesi
		lcd_set_cursor(0, 0);
		lcd_print("Isik:");
		lcd_print_uint16(ldr);

		lcd_set_cursor(1, 0);
		lcd_print("Hava:");
		lcd_print_uint16(mq);
		break;

		case 3:
		// Ekran 4: Sistemin durumu ; Saat
		lcd_set_cursor(0, 0);
		if (mq > MQ135_THRESHOLD){
			lcd_print("ALARM");
		}
		else if (d->temperature > TEMP_MAX || d->temperature < TEMP_MIN || d->humidity > HUM_MAX     || d->humidity < HUM_MIN){
			lcd_print("UYARI");
		}
		else {
			lcd_print("NORMAL");
		}

		lcd_set_cursor(1, 0);
		lcd_print_uint16(t->hour);   lcd_print(":");
		lcd_print_uint16(t->minute); lcd_print(":");
		lcd_print_uint16(t->second);
		break;

		default:
		lcd_set_cursor(0, 0);
		lcd_print("Sayfa Hatasi");
		break;
	}
}


int main(void) {
	gpio_init();
	adc_init();
	i2c_init();
	lcd_init();
	uart_init(UBRR_VAL);
	_delay_ms(500);

	// DS1307 ilk kurulum: bir kere yapildiktan sonra yorum satiri yapilir
						  // sn:dk:sa:gun:ay:yil
	//DS1307_Time init_time = {0, 45, 16, 14, 4, 26};
	//ds1307_set_time(init_time);

	// BME280 kontrol
	if (!bme280_init()) {
		lcd_clear();
		lcd_set_cursor(0, 0);
		lcd_print("BME280 HATA!");
		while(1);
	}
	
	uint8_t current_page = 0;
	uint8_t send_counter = 0;
	
	// sistem baslangici tarih ve saat ile
	DS1307_Time t_init = ds1307_get_time();
	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_print_uint16(t_init.day);   lcd_print("/");
	lcd_print_uint16(t_init.month); lcd_print("/");
	lcd_print_uint16(t_init.year);
	
	lcd_set_cursor(1, 0);
	lcd_print_uint16(t_init.hour);   lcd_print(":");
	lcd_print_uint16(t_init.minute); lcd_print(":");
	lcd_print_uint16(t_init.second);
	_delay_ms(4000);


	while(1){
		uint16_t elapsed = 0;

		while (elapsed < PAGE_HOLD_MS) {
			// sensor okuma
			BME280_Data bme = bme280_read();
			uint16_t ldr = adc_read(ADC_LDR);
			uint16_t mq  = adc_read(ADC_MQ135);
			DS1307_Time t = ds1307_get_time();
			
			// lcd guncelleme
			lcd_show_page(current_page, &bme, ldr, mq, &t);
			update_status(&bme, mq);
			
			// tekrar guncelleme (gercek zamanli)
			_delay_ms(PAGE_REFRESH_MS);
			elapsed += PAGE_REFRESH_MS;
		}

		// sayfa degistirme
		current_page++;
		if (current_page >= PAGE_COUNT) {
			current_page = 0;
		}

		// yaklasik 16 saniyede bir gonderim
		send_counter++;
		if (send_counter >= 4) {
			BME280_Data bme = bme280_read();
			uint16_t ldr = adc_read(ADC_LDR);
			uint16_t mq  = adc_read(ADC_MQ135);

			esp32_send_data(
			(uint16_t)bme.temperature,
			(uint16_t)bme.humidity,
			(uint16_t)bme.pressure,
			ldr,
			mq
			);
			send_counter = 0;
		}
	}
}
