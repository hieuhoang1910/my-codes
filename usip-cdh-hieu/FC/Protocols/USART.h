#define F_CPU 8000000UL  //This is 8MHz clockspeed, used for delay.h for delay timings

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef USART_H
#define USART_H

#define FOSC 8000000// Clock Speed
#define BAUD 38400
#define ubrr FOSC/16/BAUD-1

void USART_Init(unsigned int);

void Radio_Transmit(uint8_t);
uint8_t RadioCheck_Receive(void);
uint8_t Radio_Receive(void);

void EPS_Transmit(uint8_t);
uint8_t EPS_Receive(void);

#endif