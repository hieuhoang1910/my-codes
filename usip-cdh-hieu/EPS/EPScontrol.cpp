#include "EPScontrol.h"

volatile int currentTime = 0;

// Timer 1 initialization for 8MHz clock
void initTimer1() {
    cli(); // Disable global interrupts while setting up

    // Set Timer 1 mode to CTC (Clear Timer on Compare Match)
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);

    // Set the prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Calculate the compare value for a 1-second interval at 8MHz
    OCR1A = 7812;

    // Enable Timer 1 compare match interrupt
    TIMSK1 = (1 << OCIE1A);

    sei(); // Enable global interrupts
}

void delay(int DelayTime){
	int ElapsedTime = 0;                      
	int inittime = currentTime;
	while(ElapsedTime <= DelayTime)
	{
		int finaltime = currentTime;
		ElapsedTime = finaltime - inittime;
	}
}

void VoltControl_Init() 
{
	// Setting voltage control pins to 0 before setting them as outputs
	// This ensures none of them will turn their voltage on before they should
	// Setting each of the voltage control pins as outputs
	DDRB = (1<<DDB4); // 3V3 control
	DDRC = (1<<DDB7); // 5V control
	DDRD = (1<<DDD1); // 12V control

	PORTB = (0<<PORTB4); // 3V3 control
	PORTC = (0<<PORTC7); // 5V control
	PORTD = (0<<PORTD1); // 12V control
}

// Function to initialize the ADC
void ADC_Init() {
    // Enable the ADC and set the prescaler to 128 for maximum resolution
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_Read(uint8_t channel) {
    // Select the ADC channel
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    
    // Start the conversion
    ADCSRA |= (1 << ADSC);
	
    // Wait for the conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Return the 10-bit ADC value
    return ADC;
}

void set3VControl(bool value) 
{
	if(value == 1){
		PORTB = (1<<PORTB4);
	}
	else{
		PORTB = (0<<PORTB4);
	}
	
}

void set5VControl(bool value) 
{
	if(value == 1){
		PORTC = (1<<PORTC7);
	}
	else{
		PORTC = (0<<PORTC7);
	}
}

void set12VControl(bool value) 
{
	if(value == 1){
		PORTD = (1<<PORTD1);
	}
	else{
		PORTD = (0<<PORTD1);
	}
}

void BatteryCheck(void)
{
	uint8_t BatVolt[8];
	int ADC_ch[4] = {3,5,6,7};
	int j = 0;
		
	for(int i=0;i<8;i+=2){
		uint16_t adcValue = ADC_Read(ADC_ch[j]);
		
		BatVolt[i] = (uint8_t)(adcValue >> 8);
		BatVolt[i+1] = (uint8_t)(adcValue & 0xFF);
		
		j++;
	}

	for(int i=0;i<8;i++){
		USART_Transmit(BatVolt[i]);
		}
}