/*
 * ds1307.h
 *
 * Created: 23.03.2026 23:50:25
 *  Author: MSI
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>

#define DS1307_ADDR 0x68

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} DS1307_Time;

void ds1307_set_time(DS1307_Time t);
DS1307_Time ds1307_get_time(void);

#endif /* DS1307_H_ */