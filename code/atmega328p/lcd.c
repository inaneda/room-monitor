/*
 * lcd.c
 *
 * Created: 23.03.2026 22:22:50
 *  Author: MSI
 */ 

#include "lcd.h"
#include "i2c.h"
#include <util/delay.h>

// PCF8574 I2C expander uzerindeki LCD kontrol bitleri
#define LCD_BACKLIGHT 0x08
#define LCD_EN        0x04
#define LCD_RW        0x02
#define LCD_RS        0x01

// mode = 0 : komut gonder, LCD_RS : veri gonder
static void lcd_send_byte(uint8_t data, uint8_t mode) {
	uint8_t high = (data & 0xF0) | LCD_BACKLIGHT | mode;
	uint8_t low  = ((data << 4) & 0xF0) | LCD_BACKLIGHT | mode;

	i2c_start(LCD_ADDR << 1);
	
	i2c_write(high | LCD_EN); // Enable = 1
	_delay_us(1);
	i2c_write(high & ~LCD_EN); // Enable = 0
	_delay_us(50);

	i2c_write(low | LCD_EN); // Enable = 1
	_delay_us(1);
	i2c_write(low & ~LCD_EN); // Enable = 0
	_delay_us(50);

	i2c_stop();
}

// tek karakter yazdirma
void lcd_send_char(char c) {
	lcd_send_byte((uint8_t)c, LCD_RS);
}

// komut gonderme
static void lcd_cmd(uint8_t cmd) {
	lcd_send_byte(cmd, 0);
}

// veri gonderme
static void lcd_data(uint8_t data) {
	lcd_send_byte(data, LCD_RS);
}

//START
void lcd_init(void) {
	_delay_ms(50);
	
	// 4-bit mod
	lcd_cmd(0x33);
	_delay_ms(5);
	lcd_cmd(0x32);
	_delay_ms(1);
	
	lcd_cmd(0x28);  // 4-bit, 2 satir
	lcd_cmd(0x0C);  // display on, cursor off
	// char yazildikca cursor -> saga
	lcd_cmd(0x06);  // entry mode
	lcd_clear();
}

// ekran temizleme
void lcd_clear(void) {
	lcd_cmd(0x01);
	_delay_ms(2);
}

// cursor -> satira-sutuna tasima
void lcd_set_cursor(uint8_t row, uint8_t col) {
	uint8_t addr = (row == 0) ? 0x00 : 0x40;
	lcd_cmd(0x80 | (addr + col));
}

// string yazdirma
void lcd_print(const char *str) {
	while (*str) {
		lcd_data(*str++);
	}
}

// uint16_t sayi yazdirma
void lcd_print_uint16(uint16_t val) {
	char buf[6];
	// sayi -> char dizisi
	buf[5] = '\0';
	buf[4] = '0' + (val % 10); val /= 10;
	buf[3] = '0' + (val % 10); val /= 10;
	buf[2] = '0' + (val % 10); val /= 10;
	buf[1] = '0' + (val % 10); val /= 10;
	buf[0] = '0' + (val % 10);
	char *p = buf; // gereksiz 0'lari kaldir
	while (*p == '0' && *(p+1) != '\0') p++;
	lcd_print(p);
}