/*
 * uart.h
 *
 * Created: 23.03.2026 21:18:49
 *  Author: MSI
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void uart_init(uint16_t ubrr);
void uart_send_char(char c);
void uart_send_string(const char *str);
void uart_send_uint16(uint16_t val);

#endif /* UART_H_ */