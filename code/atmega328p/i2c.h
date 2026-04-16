/*
 * i2c.h
 *
 * Created: 23.03.2026 22:23:48
 *  Author: MSI
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

#define F_CPU 16000000UL

void i2c_init(void);
uint8_t i2c_start(uint8_t addr_rw);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);

#endif