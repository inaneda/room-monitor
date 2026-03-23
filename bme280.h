/*
 * bme280.h
 *
 * Created: 23.03.2026 23:09:09
 *  Author: MSI
 */ 


#ifndef BME280_H_
#define BME280_H_

#include <avr/io.h>

#define BME280_ADDR 0x76  // SDO GND'ye bagli -> 0x76, VCC'ye bagli olsaydi -> 0x77

typedef struct {
	float temperature;
	float humidity;
	float pressure;
} BME280_Data;

uint8_t bme280_init(void);
BME280_Data bme280_read(void);

#endif /* BME280_H_ */