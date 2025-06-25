#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

// Timer 1 compare match interrupt handler
ISR(TIMER1_COMPA_vect) {
    currentTime++; // Increment the time counter every second
}

int main() {
    // Initialize Timer 1
    initTimer1();
    PORTG = (0<<PG2);

    // Main loop
    while (1) 
    {
        if(currentTime >= 10)
        {
            PORTG = (1<<PG2);
        }
    }
}

/*
#include <avr/io.h>

int main(void)
{
    int ElapsedSeconds = 0;
    TCCR1B = (1<<CS12);            //sets prescalar to 256 from equation Target Timer Count = (input frequency/(prescale * target frequency)) - 1
                                   // 256 was chosen because (8000000 (operating Hz)/(256 * 1 (target is 1 Hz to get 1 second per cycle))) - 1 = 31249 which fits in 16 bits
    PORTG = (0<<PG2);
    
    
    while(1)
    {
        if(ElapsedSeconds == 60)
        {   
            ElapsedSeconds = 0;
            PORTG  = (1<<PG2);
        }
        
        if(TCNT1 >= 31249)             //16 timer counting register
        {   
            TCNT1 = 0;
            ElapsedSeconds++;
        }
    }   
}
*/