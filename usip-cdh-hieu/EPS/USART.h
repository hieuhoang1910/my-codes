#define F_CPU 8000000UL // Throws warnings if placed elsewhere

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef USART_H
#define USART_H

#define FOSC 8000000// Clock Speed
#define BAUD 38400
#define ubrr FOSC/16/BAUD-1

void USART_Init(unsigned int);
void USART_Transmit(unsigned char);
unsigned char USART_Receive(void);

#endif