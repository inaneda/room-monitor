/*
 * ds1307.c
 *
 * Created: 23.03.2026 23:50:14
 *  Author: MSI
 */ 

#include "ds1307.h"
#include "i2c.h"

// BCD'den decimal'e cevirme
static uint8_t bcd2dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Decimal'den BCD'ye cevirme
static uint8_t dec2bcd(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
}

void ds1307_set_time(DS1307_Time t) {
	i2c_start();
	i2c_write(DS1307_ADDR << 1);
	i2c_write(0x00);  // register 0'dan basla
	i2c_write(dec2bcd(t.second) & 0x7F);  // CH bit = 0 (saat calissin)
	i2c_write(dec2bcd(t.minute));
	i2c_write(dec2bcd(t.hour));
	i2c_write(0x01);  // day of week (kullanmiyoruz)
	i2c_write(dec2bcd(t.day));
	i2c_write(dec2bcd(t.month));
	i2c_write(dec2bcd(t.year));
	i2c_stop();
}

DS1307_Time ds1307_get_time(void) {
	DS1307_Time t;
	i2c_start();
	i2c_write(DS1307_ADDR << 1);
	i2c_write(0x00);
	i2c_start();
	i2c_write((DS1307_ADDR << 1) | 1);
	t.second = bcd2dec(i2c_read_ack() & 0x7F);
	t.minute = bcd2dec(i2c_read_ack());
	t.hour   = bcd2dec(i2c_read_ack());
	i2c_read_ack();  // day of week (atla)
	t.day    = bcd2dec(i2c_read_ack());
	t.month  = bcd2dec(i2c_read_ack());
	t.year   = bcd2dec(i2c_read_nack());
	i2c_stop();
	return t;
}