#include "../Protocols/USART.cpp"

int main()
{
	int data;
    USART_Init(ubrr);
    
	USART_Transmit(0x00);
	data = USART_Receive();
}
