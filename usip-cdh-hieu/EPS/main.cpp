#include "EPScontrol.h"

// 7 day timer countdown
uint32_t countDownTimer = 70298; //70298 for 7 days
bool countingDown = true;

ISR(TIMER1_OVF_vect){
	// Reset 16b timer counter registers
	TCNT1 = 0;
	countDownTimer--;
	if(countDownTimer){
		TIMSK1 = (1 << TOIE1);
	}
	else{
		countingDown = false;
	}
}


ISR(USART_RX_vect){
	int data;
	data = USART_Receive();

	switch(data)
	{
		case PING:
			USART_Transmit(ACK);
			break;
		
		case ON_12V:
			set12VControl(1);
			break;

		case OFF_12V:
			set12VControl(0);
			break;

		case ON_3V:
			set3VControl(1);
			break;

		case OFF_3V:
			set3VControl(0);
			break;

		case BATTERY_CHECK:
			BatteryCheck();
			break;

		case RESTART12V:
			//for unresponsive PPU or Radio
			set12VControl(0);
			for(int i = 0; i < 15; i++){
				_delay_ms(1000);
			}
			set12VControl(1);
			_delay_ms(1000);
			break;

		case RESTART5V:
			//for unresponsive sensors
			set5VControl(0);
			for(int i = 0; i < 15; i++){
				_delay_ms(1000);
			}
			set5VControl(1);
			_delay_ms(1000);
			break;

		case NUM_FC_RESTARTS:
			// Keeps track of flight computer restarts for logic in FC code
			USART_Transmit(flightComputerRestarts);
			flightComputerRestarts += 1;
			break;
	}
}

int main()
{
	VoltControl_Init();
	
	// Power reduction through Power Reduction Register
	// Shut down everything except 16-bit timer
	// TODO: check for conflicts with operations
	PRR = ~(1 << PRTIM1);

	// 7 day timer
	// Applies maximum prescler in overflow interrupt mode
	// TODO: write ISR for timer 1 overflow interrupt

	// Timer 1 with 1024x prescaler
	TCCR1B = (1 << CS12) | (1 << CS10);

	// Interrupt overflow mode
	TIMSK1 = (1 << TOIE1);

	sei();

	while (countingDown){}

	PRR = 0;

	USART_Init(ubrr);
	ADC_Init();
	
	set5VControl(1);
	while(1){
	}
}
