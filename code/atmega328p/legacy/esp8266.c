/*
 * esp8266.c
 *
 * Bu dosya ESP8266 ile yapilan ilk denemeyi icerir.
 *
 * Created: 16.04.2026 11:24:22
 *  Author: MSI
 */ 

/*
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lcd.h"
#include "secrets.h"
#include "esp8266.h"

// AT komutu gönderme
static void esp_send(const char* cmd) {
	uart_send_string(cmd);
	uart_send_string("\r\n");
	_delay_ms(1000);
}

// strlen
static uint8_t strlen_manual(const char *s) {
	uint8_t n = 0;
	while (*s++) n++;
	return n;
}

// ESP8266 ile WiFi baglantisi
uint8_t esp_connect_wifi(void) {

	char resp[128];

	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_print("ESP test...");

	// ESP cevap veriyor mu
	uart_send_string("AT\r\n");
	uart_read_response(resp, sizeof(resp), 2000);

	lcd_clear();
	lcd_set_cursor(0, 0);

	if (!uart_contains(resp, "OK")) {
		lcd_print("ESP cevap yok");
		lcd_set_cursor(1, 0);
		lcd_print("UART/BAUD?");
		_delay_ms(3000);
		return 0;
	}

	lcd_print("ESP OK");
	_delay_ms(1500);

	uart_send_string("AT+CWMODE=1\r\n");
	uart_read_response(resp, sizeof(resp), 3000);

	// WiFi baglan
	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_print("WiFi baglan...");

	uart_send_string("AT+CWJAP=\"");
	uart_send_string(WIFI_SSID);
	uart_send_string("\",\"");
	uart_send_string(WIFI_PASS);
	uart_send_string("\"\r\n");

	uart_read_response(resp, sizeof(resp), 15000);

	lcd_clear();
	lcd_set_cursor(0, 0);

	if (uart_contains(resp, "WIFI GOT IP") || uart_contains(resp, "OK")) {
		lcd_print("WiFi OK");
		_delay_ms(2000);
		return 1;
	}

	lcd_print("WiFi FAIL");
	_delay_ms(3000);
	return 0;
}

// ThingSpeak veri gonderme
void esp_send_data(uint16_t temp, uint16_t hum, uint16_t pres, uint16_t ldr, uint16_t mq) {

	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_print("Sending...");

	// paket uzunlugunu hesaplama
	char b1[6], b2[6], b3[6], b4[6], b5[6];
	uart_uint16_to_str(temp, b1);
	uart_uint16_to_str(hum, b2);
	uart_uint16_to_str(pres, b3);
	uart_uint16_to_str(ldr, b4);
	uart_uint16_to_str(mq, b5);

	// sabit kisim: "GET /update?api_key=XXXXXXXXXXXXXXXX&field1=&field2=&field3=&field4=&field5= HTTP/1.0\r\n\r\n"
	// uzunlugu elle say veya fazladan ver — burada sabit 110 + degisken kisimlar
	uint16_t total = 74 + 16 + // GET + api_key
		strlen_manual(b1) + strlen_manual(b2) +
		strlen_manual(b3) + strlen_manual(b4) +
		strlen_manual(b5) + 4; // \r\n\r\n

	esp_send("AT+CIPMUX=0");
	uart_send_string("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");
	_delay_ms(3000);

	uart_send_string("AT+CIPSEND=");
	uart_send_uint16(total);
	uart_send_string("\r\n");
	_delay_ms(2000);

	uart_send_string("GET /update?api_key=");
	uart_send_string(THINGSPEAK_KEY);

	uart_send_string("&field1="); uart_send_uint16(temp);
	uart_send_string("&field2="); uart_send_uint16(hum);
	uart_send_string("&field3="); uart_send_uint16(pres);
	uart_send_string("&field4="); uart_send_uint16(ldr);
	uart_send_string("&field5="); uart_send_uint16(mq);

	uart_send_string(" HTTP/1.0\r\n\r\n");

	_delay_ms(3000);

	esp_send("AT+CIPCLOSE");
	
	lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_print("Sent!");
	_delay_ms(2000);
}
*/