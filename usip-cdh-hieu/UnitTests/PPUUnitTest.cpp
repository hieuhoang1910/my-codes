#include "../StateMachine/sensors.cpp"

int main()
{
        ppuinit();
        
        PPU_ping(); 
        _delay_ms(1000);

        PPU_on(); 
        _delay_ms(1000);
        
        PPU_duty_echo(); 
        _delay_ms(1000);
        
        PPU_off(); 
        _delay_ms(5000);
}
