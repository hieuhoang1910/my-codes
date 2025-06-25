#include "../Protocols/USART.cpp"

int main()
{
    uint8_t packet[205]{};
    for(int i = 0; i < 3; i++)
    {
        packet[i] = 0x50;
    }
    
    for(int i = 3; i < 38; i++)
    {
        packet[i] = 0x01;
    }

    _delay_ms(100);
    USART_Init(ubrr);
    
    while(1)
    {
        for(int i = 0; i < 38; i++)
        {
            USART_Transmit(packet[i]);
        }
        _delay_ms(5000);
    }
}

