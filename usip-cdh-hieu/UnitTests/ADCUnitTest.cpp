#include "../EPS/EPScontrol.cpp"

// Function to initialize the ADC
void ADC_Init() {
    // Enable the ADC and set the prescaler to 128 for maximum resolution
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// Function to read the ADC value
uint16_t ADC_Read(uint8_t channel) {
    // Select the ADC channel
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07);
    
    // Start the conversion
    ADCSRA |= (1 << ADSC);
	
    // Wait for the conversion to complete
    while (ADCSRA & (1 << ADSC));
    
    return ADC;
}

int main() {
    USART_Init(ubrr);
    ADC_Init();

    while (1) {
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
        // Delay for a while before taking the next reading
        _delay_ms(1000);
    }

    return 0;
}




