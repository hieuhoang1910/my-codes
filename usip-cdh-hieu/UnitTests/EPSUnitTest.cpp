#include "EPScontrol.h"

int main()
{
	USART_Init(ubrr);
	VoltControl_Init();
	//sei();
	
	while(1)
	{
		int data;
		data = USART_Receive();
		
		switch(data)
		{
			case 0x02:
				set3VControl(1);
				set12VControl(0);
				set5VControl(0);
				USART_Transmit(0x22);
				break;
			
			case 0x03:
				set3VControl(0);
				set12VControl(0);
				set5VControl(1);
				USART_Transmit(0x33);
				break;
			
			case 0x04:
				set3VControl(0);
				set12VControl(1);
				set5VControl(0);
				USART_Transmit(0x44);
				break;
		}
	}
}