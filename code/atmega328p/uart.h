/*
 * uart.h
 *
 * Created: 23.03.2026 21:18:49
 *  Author: MSI
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdint.h>

uint8_t uart_uint16_to_str(uint16_t val, char *buf);
uint8_t uart_receive_char(void);

void uart_init(uint16_t ubrr);
void uart_send_char(char c);
void uart_send_string(const char *str);
void uart_send_uint16(uint16_t val);

uint8_t uart_read_response(char *buf, uint8_t max_len, uint16_t wait_ms);
uint8_t uart_contains(const char *buf, const char *key);

#endif /* UART_H_ */