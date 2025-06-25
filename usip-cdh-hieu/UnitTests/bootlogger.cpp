#include <avr/io.h>
#include <avr/eeprom.h>

//For testing purposes:
uint8_t ClearCode = 0x00;
uint8_t FlashBoot = eeprom_read_byte((const uint8_t *)0);

int main(void){
	DDRC |= (1<<PORTC7);
	DDRB |= (1<<PORTB4);
	
	PORTB &= ~(1<<PORTB3);
	PORTB &= ~(1<<PORTB4);
	
	if( FlashBoot == 0xFF){
		eeprom_write_byte((uint8_t *) 0, 0x00);
		eeprom_write_byte((uint8_t *) 1, 0x00);
	}
	else{
		uint8_t BootCounter = eeprom_read_byte((const uint8_t *)1);
		BootCounter++;
		eeprom_write_byte((uint8_t *)1, BootCounter);
	}
	
	if(ClearCode == 0x11){
		for(int i=0; i<512; i++){
			eeprom_write_byte((uint8_t *) i, 0xFF);
		}
	}
}

//IF 2ND BIT == 0X0F, VIBRATION TEST PASS
//ANYTHING MORE MEANS FAIL
//LESS THAN 0X0F DOES NOT MEAN FAIL