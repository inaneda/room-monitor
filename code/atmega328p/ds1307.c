/*
 * ds1307.c
 *
 * Created: 23.03.2026 23:50:14
 *  Author: MSI
 */ 

#include <util/delay.h>
#include "ds1307.h"
#include "i2c.h"
// #include "lcd.h"

// BCD'den decimal'e cevirme
static uint8_t bcd2dec(uint8_t bcd){
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// decimal'den BCD'ye cevirme
static uint8_t dec2bcd(uint8_t dec){
	return ((dec / 10) << 4) | (dec % 10);
}

void ds1307_set_time(DS1307_Time t){
    // CH bitini temizle
    i2c_start(DS1307_ADDR << 1);
    i2c_write(0x00);	// saniye reg adresi
    i2c_write(0x00);	// saniye=0, CH=0
    i2c_stop();
    
    _delay_ms(10);
    
    // tum zamani yaz
    i2c_start(DS1307_ADDR << 1);
    i2c_write(0x00);	// reg pointer = 0x00
    i2c_write(dec2bcd(t.second) & 0x7F); // CH bitini sifirlama
    i2c_write(dec2bcd(t.minute));
	// saat reg.i:
	// bit6 = 12/24 saat modu (0 = 24 saat)
    i2c_write(dec2bcd(t.hour) & 0x3F);
    i2c_write(0x01);	// haftanin gunu (1-7 arasi)
    i2c_write(dec2bcd(t.day));
    i2c_write(dec2bcd(t.month));
    i2c_write(dec2bcd(t.year));
    i2c_stop();
}

DS1307_Time ds1307_get_time(void){
	DS1307_Time t = {0, 0, 0, 0, 0, 0};
	
	// register pointer'ý 0x00'a set et
	i2c_start(DS1307_ADDR << 1);
	i2c_write(0x00);
	i2c_stop();
	_delay_us(100);
	
	// okuma baslatma
	i2c_start((DS1307_ADDR << 1) | 1);
	uint8_t raw_sec   = i2c_read_ack();
	uint8_t raw_min   = i2c_read_ack();
	uint8_t raw_hour  = i2c_read_ack();
	i2c_read_ack();   // haftanin gunu yok
	uint8_t raw_day   = i2c_read_ack();
	uint8_t raw_month = i2c_read_ack();
	uint8_t raw_year  = i2c_read_nack();
	i2c_stop();
	
	t.second = bcd2dec(raw_sec & 0x7F); // CH bitini maskele
	t.minute = bcd2dec(raw_min);
	t.hour   = bcd2dec(raw_hour & 0x3F);
	t.day    = bcd2dec(raw_day);
	t.month  = bcd2dec(raw_month);
	t.year   = bcd2dec(raw_year);
	
	return t;
}