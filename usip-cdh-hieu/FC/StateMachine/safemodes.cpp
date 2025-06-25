#include "safemodes.h"

volatile static int currentTime;

// Timer 1 compare match interrupt handler
ISR(TIMER1_COMPA_vect)
{
	currentTime++; // Increment the time counter every second
}

//Safe Modes
int check(int input, int min, int max){
    int result;
    if(input >= max){
        result = 2;
    }
    else if(input <= min){
        result = 0;
    }
    else{
        result = 1;
    }
    return result;
}

int radioCheck(){
	uint8_t data[205];
	// Send UTC request
	for(int i=0; i<3; i++){
		data[i] = 0x50;
	}
	data[3] = 0x4A;
	for(int i=4; i<205; i++){
		data[i] = 0x00;
	}
	
	for(int i = 0; i < 205; i++){
		Radio_Transmit(data[i]);
	}
	
	// UTC request packet sent to radio
	int recvPacket;
	recvPacket = RadioCheck_Receive();
	return recvPacket;
}

void PrimarySafeMode(void){        
    // Initial hardware response check

    // Temperature sensor device check
    uint8_t* temp = TEMP();
    for(int i = 0; i < 6; i++){
        deviceCheck[i] = (I2C_ERROR_CODE != temp[i]);
    }

    // Pressure Transducers
    uint8_t* pressureArray = ADCS();
    deviceCheck[PRESS_SENS_0] = (pressureArray[0] << 8) | (pressureArray[1]);
    deviceCheck[PRESS_SENS_1] = (pressureArray[2] << 8) | (pressureArray[3]);


    //-------------------------------------------------------------------------------
    // Check values in the deviceCheck array, restart the appropriate line to
    // attempt to get the device working. If we've restarted a number of times (defined
    // by the MAX_RESTARTS global var in missionmodes.h), continue on with the mission
    
    int currentRestart = 0;

    // If any pressure sensors or temp sensors aren't working, restart 5V line (restarts flight computer)
    bool thermalSensorBroken = (!deviceCheck[0] || !deviceCheck[1] || !deviceCheck[2] || !deviceCheck[3] || !deviceCheck[4] || !deviceCheck[5] );
    while((!deviceCheck[PRESS_SENS_0] || !deviceCheck[PRESS_SENS_1] || thermalSensorBroken) && currentRestart < FC_MAX_RESTARTS){
        EPS_Transmit(NUM_RESTARTS);
        currentRestart = EPS_Receive();

        if(currentRestart < FC_MAX_RESTARTS){
            EPS_Transmit(RESTART_5V);
			_delay_ms(15000);
        }
    }

    //Turn on 12V
	EPS_Transmit(0x01);
	//PORTJ ^= (1<<PJ2);
    LineToggle12V = 1;

	_delay_ms(5000);
	
	//Turn on 3V
	EPS_Transmit(0x10);
	//PORTJ ^= (1<<PJ0);
	
	// Check PPU
	deviceCheck[PPU_CHECK] = (PPU_ACK == PPU_ping());

	currentRestart = 0;
	int PPU_MAX_RESTARTS = 3;

	// Check PPU, toggle 12V line if its not working
	while(!deviceCheck[PPU_CHECK] && currentRestart < PPU_MAX_RESTARTS){
		if(currentRestart < PPU_MAX_RESTARTS){
			EPS_Transmit(RESTART_12V);
			EPS_Transmit(0x11);
			//PORTJ &= ~(1<<PJ0);
			
			for(int i = 0; i < 15; i++){
				_delay_ms(1000);
			}
			
			EPS_Transmit(0x10);
			//PORTJ ^= (1<<PJ0);
			_delay_ms(1000);
			
			deviceCheck[PPU_CHECK] = (PPU_ACK == PPU_ping());
			currentRestart += 1;
		}
	}
		
	//Turn off 3V
	EPS_Transmit(0x11);
	//PORTJ &= ~(1<<PJ0);

	// 2 min wait for radio reboot
	for(int i=0; i<120; i++){	
		_delay_ms(1000);
	}
	
    // Check Radio
    int radcheck = radioCheck();

    currentRestart = 0;
    int RADIO_MAX_RESTARTS = 2;
    
    // Check radio, toggle 12V line if it's not working
    while(radcheck == 0 && currentRestart < RADIO_MAX_RESTARTS){
        if(currentRestart < RADIO_MAX_RESTARTS){            
            EPS_Transmit(RESTART_12V);

            // 2 min wait for radio reboot
            for(int i=0; i<120; i++){
                _delay_ms(1000);
            }
            
            radcheck = radioCheck();
            currentRestart += 1;
        }
    }
}

void ThermalSafeMode(int tMin, int tMax, int SensorCount){
    //PORTJ ^= (1<<PJ3);
    DDRC = (1<<DDC0);

    EPS_Transmit(0x02);
	//PORTJ &= ~(1<<PJ2);
    LineToggle12V = 0;

    EPS_Transmit(0x11);
	//PORTJ &= ~(1<<PJ0);

    int passes = 0;                             
    int HCElapsedTime = 0;                      
    int initHCtime = currentTime;

    uint16_t* Voltage = Battery();

    while(passes < 4 && HCElapsedTime <= 7200 && Voltage[4] >= CritBatPer)
    {
        uint8_t* temp = TEMP();
        for(int i=0; i<SensorCount; i++)                   
        {
			int TempVal;
			if(temp[i] > 0x7F){
				TempVal = temp[i] - 254;
			}
			else{
				TempVal = temp[i];
			}

            int tempcheck = check(TempVal,tMin,tMax);
            if(tempcheck == 0){
                passes = 0;
                PORTC = (1<<PC0);                   //heater on
        
            }
            else if(tempcheck== 2){
                passes = 0;
                PORTC = (0<<PC0);                   //heater off
            }
            else{
                passes++;
            }
        }

        uint16_t* Voltage = Battery();

        int finalHCtime = currentTime;
        HCElapsedTime = finalHCtime - initHCtime;
    }
    //PORTJ &= ~(1<<PJ3);
	PORTC = (0<<PC0);
}