/*
 * lcd.h
 *
 * Created: 23.03.2026 22:23:00
 *  Author: MSI
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>

#define LCD_ADDR 0x27  // I2C adresi (0x3F de olabilir)

void lcd_send_char(char c);

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(const char *str);
void lcd_print_uint16(uint16_t val);

#endif /* LCD_H_ */