/*
 * bme280.c
 *
 * Created: 23.03.2026 23:09:23
 *  Author: MSI
 */ 

#include "bme280.h"
#include "i2c.h"
#include <util/delay.h>

// kalibrasyon verileri
static uint16_t dig_T1;
static int16_t  dig_T2, dig_T3;
static uint16_t dig_P1;
static int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static uint8_t  dig_H1, dig_H3;
static int16_t  dig_H2, dig_H4, dig_H5;
static int8_t   dig_H6;
static int32_t  t_fine; // bosch algoritmasinda ara sicaklik duzeltme degeri

// tek register okuma
static uint8_t bme280_read_reg(uint8_t reg) {
	i2c_start(BME280_ADDR << 1);
	i2c_write(reg);
	i2c_start((BME280_ADDR << 1) | 1);
	uint8_t val = i2c_read_nack();
	i2c_stop();
	return val;
}

// tek register yazma
static void bme280_write_reg(uint8_t reg, uint8_t val) {
	i2c_start(BME280_ADDR << 1);
	i2c_write(reg);
	i2c_write(val);
	i2c_stop();
}

static void bme280_read_calibration(void) {
	dig_T1 = (bme280_read_reg(0x89) << 8) | bme280_read_reg(0x88);
	dig_T2 = (bme280_read_reg(0x8B) << 8) | bme280_read_reg(0x8A);
	dig_T3 = (bme280_read_reg(0x8D) << 8) | bme280_read_reg(0x8C);
	dig_P1 = (bme280_read_reg(0x8F) << 8) | bme280_read_reg(0x8E);
	dig_P2 = (bme280_read_reg(0x91) << 8) | bme280_read_reg(0x90);
	dig_P3 = (bme280_read_reg(0x93) << 8) | bme280_read_reg(0x92);
	dig_P4 = (bme280_read_reg(0x95) << 8) | bme280_read_reg(0x94);
	dig_P5 = (bme280_read_reg(0x97) << 8) | bme280_read_reg(0x96);
	dig_P6 = (bme280_read_reg(0x99) << 8) | bme280_read_reg(0x98);
	dig_P7 = (bme280_read_reg(0x9B) << 8) | bme280_read_reg(0x9A);
	dig_P8 = (bme280_read_reg(0x9D) << 8) | bme280_read_reg(0x9C);
	dig_P9 = (bme280_read_reg(0x9F) << 8) | bme280_read_reg(0x9E);
	dig_H1 = bme280_read_reg(0xA1);
	dig_H2 = (bme280_read_reg(0xE2) << 8) | bme280_read_reg(0xE1);
	dig_H3 = bme280_read_reg(0xE3);
	dig_H4 = ((int16_t)(int8_t)bme280_read_reg(0xE4) << 4) | (bme280_read_reg(0xE5) & 0x0F);
	dig_H5 = ((int16_t)(int8_t)bme280_read_reg(0xE6) << 4) | (bme280_read_reg(0xE5) >> 4);
	dig_H6 = (int8_t)bme280_read_reg(0xE7);
}

// sensoru baslatma
uint8_t bme280_init(void) {
	uint8_t id = bme280_read_reg(0xD0);
	if (id != 0x60) return 0;  // Sensor bulunamadi

	bme280_write_reg(0xF2, 0x01);  // humidity oversampling x1
	bme280_write_reg(0xF4, 0x27);  // temp+pressure oversampling x1, normal mode
	bme280_write_reg(0xF5, 0xA0);  // standby 1000ms

	bme280_read_calibration();
	_delay_ms(100); // kisa bekleme
	return 1;
}

// sicaklik, basinc ve nem okuma
BME280_Data bme280_read(void) {
	BME280_Data data = {0, 0, 0};

	// ham verileri okuma:
	// sicaklik
	int32_t adc_T = ((int32_t)bme280_read_reg(0xFA) << 12) |
	((int32_t)bme280_read_reg(0xFB) << 4)  |
	(bme280_read_reg(0xFC) >> 4);
	// basinc
	int32_t adc_P = ((int32_t)bme280_read_reg(0xF7) << 12) |
	((int32_t)bme280_read_reg(0xF8) << 4)  |
	(bme280_read_reg(0xF9) >> 4);
	// nem
	int32_t adc_H = ((int32_t)bme280_read_reg(0xFD) << 8) |
	bme280_read_reg(0xFE);

	// sicaklik hesabi (Bosch referans algoritmasi)
	int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * dig_T2) >> 11;
	int32_t var2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) * dig_T3) >> 14;
	t_fine = var1 + var2;
	data.temperature = (float)((t_fine * 5 + 128) >> 8) / 100.0f;

	// basinc hesabi (Bosch referans algoritmasi)
	int64_t p_var1 = (int64_t)t_fine - 128000;
	int64_t p_var2 = p_var1 * p_var1 * (int64_t)dig_P6;
	p_var2 = p_var2 + ((p_var1 * (int64_t)dig_P5) << 17);
	p_var2 = p_var2 + (((int64_t)dig_P4) << 35);
	p_var1 = ((p_var1 * p_var1 * (int64_t)dig_P3) >> 8) + ((p_var1 * (int64_t)dig_P2) << 12);
	p_var1 = (((int64_t)1 << 47) + p_var1) * (int64_t)dig_P1 >> 33;
	
	if (p_var1 != 0) {
		int64_t p = 1048576 - adc_P;
		p = (((p << 31) - p_var2) * 3125) / p_var1;
		p_var1 = ((int64_t)dig_P9 * (p >> 13) * (p >> 13)) >> 25;
		p_var2 = ((int64_t)dig_P8 * p) >> 19;
		p = ((p + p_var1 + p_var2) >> 8) + ((int64_t)dig_P7 << 4);
		data.pressure = (float)p / 25600.0f;
	}

	// nem hesabi (Bosch referans algoritmasi)
	int32_t h = t_fine - 76800;
	h = (((adc_H << 14) - ((int32_t)dig_H4 << 20) - ((int32_t)dig_H5 * h)) + 16384) >> 15;
	h = h * (((((((int32_t)dig_H6 * h) >> 10) * (((int32_t)dig_H3 * h) >> 11) + 32768)) >> 10) + 2097152);
	h = (h * (int32_t)dig_H2 + 8192) >> 14;
	h = h - (((((h >> 15) * (h >> 15)) >> 7) * (int32_t)dig_H1) >> 4);
	// nem degerini 0 ile 100% arasinda sinirlama
	h = h < 0 ? 0 : h;
	h = h > 419430400 ? 419430400 : h;
	
	//data.humidity = (float)(h >> 12) / 1024.0f;
	
	data.humidity = (float)(t_fine - 76800) / 1000.0f;

	return data;
}