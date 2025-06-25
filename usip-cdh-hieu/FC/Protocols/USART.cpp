#include "USART.h"

//Library
void USART_Init(unsigned int UBRR)
{
/* Set baud rate */
UBRR1H = (unsigned char)(UBRR>>8);
UBRR1L = (unsigned char)UBRR;

UBRR2H = (unsigned char)(UBRR>>8);
UBRR2L = (unsigned char)UBRR;

/* Enable receiver and transmitter */
UCSR1B = (1<<RXEN1)|(1<<TXEN1);
/* Set frame format: 8data, 1 stop bit */
UCSR1C = (0<<USBS1)|(3<<UCSZ10);

/* Enable receiver and transmitter */
UCSR2B = (1<<RXEN2)|(1<<TXEN2);
/* Set frame format: 8data, 1 stop bit */
UCSR2C = (0<<USBS2)|(3<<UCSZ20);
} 


//Functions for USART1 lines
void Radio_Transmit(uint8_t data)
{
/* Wait for empty transmit buffer */
while (!( UCSR1A & (1<<UDRE1)));
/* Put data into buffer, sends the data */
UDR1 = data;
}


uint8_t RadioCheck_Receive(void)
{
/* Wait for data to be received */
long ElapsedTime = 0;
while (!(UCSR1A & (1<<RXC1)) && ElapsedTime < 10000000){
	_delay_us(1);
	ElapsedTime++;
}
/* Get and return received data from buffer */
return UDR1;
}

uint8_t Radio_Receive(void)
{
/* Wait for data to be received */
long ElapsedTime = 0;
while (!(UCSR1A & (1<<RXC1)) && ElapsedTime < 75000000){
	_delay_us(1);
	ElapsedTime++;
}
/* Get and return received data from buffer */
return UDR1;
}


//Functions for USART2 lines
void EPS_Transmit(uint8_t data)
{
/* Wait for empty transmit buffer */
while (!( UCSR2A & (1<<UDRE2)));
/* Put data into buffer, sends the data */
UDR2 = data;
}

uint8_t EPS_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR2A & (1<<RXC2)));
/* Get and return received data from buffer */
return UDR2;
}
