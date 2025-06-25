#include "../StateMachine/sensors.cpp"
#include <util/delay.h>]
#define F_CPU 1000000UL

int main()
{
    i2c test;
    
    imuinit();
    _delay_ms(5000);
    
    uint8_t* temperature = TEMP();
    _delay_ms(5000);
    
    uint8_t* pressure = ADCS();
    _delay_ms(5000);
    
    uint8_t* rotation = IMU();
    _delay_ms(5000);
}  
