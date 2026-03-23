/*
 * gpio.h
 *
 * Created: 22.03.2026 21:54:13
 *  Author: MSI
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>

// LED pin tanimlari (PORTD)
#define LED_RED_PIN    PD5
#define LED_YELLOW_PIN PD6
#define LED_GREEN_PIN  PD7

// Buzzer pin tanimi (PORTB)
#define BUZZER_PIN     PB1

// Fonksiyon prototipleri
void gpio_init(void);
void led_set(uint8_t red, uint8_t yellow, uint8_t green);
void buzzer_on(void);
void buzzer_off(void);

#endif /* GPIO_H_ */