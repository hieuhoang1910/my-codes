#include "missionmodes.h"

int LineToggle12V;
volatile static int currentTime;

// Timer 3 compare match interrupt handler
ISR(TIMER3_COMPA_vect)
{
	currentTime++; // Increment the time counter every second
}

void initilization(void)
{
	
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


  // Set Timer 3 mode to CTC (Clear Timer on Compare Match)
    TCCR3A = 0;
    TCCR3B = (1 << WGM32);

    // Set the prescaler to 1024
    TCCR3B |= (1 << CS32) | (1 << CS30);

    // Calculate the compare value for a 1-second interval at 8MHz
    OCR3A = 7812;

    // Enable Timer 3 compare match interrupt
    TIMSK3 = (1 << OCIE3A);
	
    sei(); // Enable global interrupts

    i2c_init();
    USART_Init(ubrr);

	PrimarySafeMode();
}

void downlink(uint8_t data[], int datasize)
{
	uint8_t packet[205];
	uint8_t pingback[5];
	
    bool sent = 0;
    int tries = 0;
	
    while(sent == 0 && tries < 3){ 
        for(int i=0; i<3; i++){
            packet[i] = 0x50;       //3 byte header
        }

        packet[3] = 0xF5;           //Command byte
        packet[4] = datasize;       //size of data
        
        for(int i = 0; i < datasize; i++){
            packet[i+5] = data[i]; //put storage data to packet
        }
        
        for(int i = datasize+5; i < 205; i++){
            packet[i] = 0xEE;
        }

        for(int i = 0; i < 205; i++){
            Radio_Transmit(packet[i]); 
        }

        for(int i = 0; i < 3; i++){
            pingback[i] = Radio_Receive();  //gets either ack or nack
        }

        if(pingback[2] == 0x00 && pingback[1] == 0x05 && pingback[0] == 0xAA){       //if statement is true if we get an ack

            for(int i = 3; i < 5; i++){
                pingback[i] = Radio_Receive();  //gets pass or fail message
            }

            if(pingback[4] == 0xFF){         //if statement is true if get a fail
                tries++;
                _delay_ms(5000);
            }   
            else{               //succesfully sent message to Iridium
                sent = 1;
            }
        }
        else{                   //go here if we don't get an ack, doesn't check for pass or fail
            tries++;
            _delay_ms(5000);
        }
    }
}

void healthcheck(void)
{
	//Turn on 3V
	EPS_Transmit(0x10);
	//PORTJ ^= (1<<PJ0);
	
    _delay_ms(5000);
    //include pinging of each device and implement PrimarySafeMode if something fails

    bool toggleThermalSM;
    bool togglePressureSM;
    bool toggleBatterySM;
   
    //Battery Percentage
    uint16_t* Voltage = Battery();
    
	//Reading values of the two batteries outputting correct voltage and picking smaller value to use for checks
	uint16_t mainBat = Voltage[4];
	
	//Battery safe mode check
    if(mainBat <= CritBatPer){              // ~30% from max before voltage drops below operational levels
        toggleBatterySM = 1;
    }      
	
	uint8_t BatVolt[8];
	
	int j = 0;
	for(int i=0; i<4; i++){
		BatVolt[j]= (uint8_t)(Voltage[i] >> 8);
		BatVolt[j+1] = (uint8_t)(Voltage[i] & 0xFF);
		j+=2;
	}


    //Pressure Sensors
    uint8_t* Vpressure = ADCS();
    double p1 = static_cast<double>((Vpressure[0]<<8)|(Vpressure[1]));   //Combines high and low byte to extract voltage value for transducer 1
    double p2 = static_cast<double>((Vpressure[2]<<8)|(Vpressure[3]));   //Combines high and low byte to extract voltage value for transducer 2
	
	double v1 = p1*0.0000625;
	double v2 = p2*0.0000625;
	
    p1 = (v1*123.78) - 74.116;
    p2 = (v2*123.78) - 74.116;

    double pressure[2] = {p1,p2};                  //Puts hex voltages into one array

    for(int i=0; i<pressureSensorCount; i++){
        int presscheck = check(pressure[i],pressMin,pressMax);

        if(presscheck != 1){
            togglePressureSM = 1;
        }
    }

    //Temperature Sensors
    uint8_t* temp = TEMP();
    for(int i=0; i<tempSensorCount; i++){
		if(temp[i] == 0x89){
			continue;
		}
		
		int TempVal;
		if(temp[i] > 0x7F){
			TempVal = temp[i] - 254;
		}
		else{
			TempVal = temp[i];
		}
		
        int tempcheck = check(TempVal,tempMin,tempMax);
        if(tempcheck != 1){
            toggleThermalSM = 1;
        }
    }

    //Storing all data in a single array for radio
    uint8_t HCdata1[18];
    for(int i=0; i<4; i++){
        HCdata1[i] = Vpressure[i];
    }
    for(int i=4; i<10; i++){
        HCdata1[i] = temp[i-4];
    }
    for(int i=10; i<18; i++){
		HCdata1[i] = BatVolt[i-10];
	}
	
    downlink(HCdata1,18);

    if(toggleThermalSM == 1 && togglePressureSM != 1 && toggleBatterySM != 1){
        ThermalSafeMode(tempMin,tempMax,tempSensorCount);
    }
}

void eburn(){
	
	uint16_t* Voltage = Battery();
	int passes;
	int ElapsedTime = 0;                      
	int initTime = currentTime;
	
	while(passes < 1 && ElapsedTime < 900 && Voltage[4] >= CritBatPer){
        DDRC = (1<<DDC0);

		uint8_t* temp = TEMP();
		int PropTemp;
		if(temp[4] > 0x7F){
			PropTemp = temp[4] - 254;
		}
		else{
			PropTemp = temp[4];
		}

		int Proptempcheck = check(PropTemp,proptempMin,proptempMax);
		if(Proptempcheck == 0){
			//Turn heater on
			PORTC = (1<<PC0);
			//Turn off 12V during duration of error handling
			EPS_Transmit(0x02);
			//PORTJ &= ~(1<<PJ2);
            LineToggle12V = 0;
		}
		else if(Proptempcheck == 2){
			//Turn heater off
			PORTC = (0<<PC0);
			//Turn off 12V during duration of error handling
			EPS_Transmit(0x02);
			//PORTJ &= ~(1<<PJ2);
            LineToggle12V = 0;
		}
		else{
			passes = 1;
		}
		
        uint16_t* Voltage = Battery();

		int finaltime = currentTime;
		ElapsedTime = finaltime - initTime;
	}
	
	PORTC = (0<<PC0);
	
    //Turn on 12V
    EPS_Transmit(0x01);
    //PORTJ ^= (1<<PJ2);
    LineToggle12V = 1;

    // 2 min wait for radio reboot
    for(int i=0; i<120; i++){		
        _delay_ms(1000);
    }

	int burncounter = 0;
    while(burncounter < 5){
		uint8_t* Vpressure1 = ADCS();
		uint8_t* temp1 = TEMP();
		uint16_t* Voltage1 = Battery();
		uint8_t BatVolt1[8];
		int j = 0;
		for(int i=0; i<4; i++){
			BatVolt1[j]= (uint8_t)(Voltage1[i] >> 8);
			BatVolt1[j+1] = (uint8_t)(Voltage1[i] & 0xFF);
			j+=2;
		}
		
        //Storing data in 1 array
        uint8_t EBdata[39];
        for(int i=0; i<4; i++){
            EBdata[i] = Vpressure1[i];
        }
        for(int i=4; i<10; i++){
            EBdata[i] = temp1[i-4];
        }
        for(int i=10; i<18; i++){
            EBdata[i] = BatVolt1[i-10];
        }
        
        //spacer data to differentiate before and after data easier
        EBdata[18] = 0xEE;
        EBdata[19] = 0xEE;
        EBdata[20] = 0xEE;

        // BURN PHASE
        PPU_on();
        PPU_GD_on();

        for(int i=1; i<5; i++){
            openSolValve(i);
        }

        // start 30 second burn timer
        for(int i=0; i<30; i++){		
	        _delay_ms(1000);
        }
        
        PPU_GD_off();
        PPU_off();

        //After burn data
        uint8_t* Vpressure2 = ADCS();
        uint8_t* temp2 = TEMP();
        uint16_t* Voltage2 = Battery();

        uint8_t BatVolt2[8];
        j = 0;
        for(int i=0; i<4; i++){
            BatVolt2[j]= (uint8_t)(Voltage2[i] >> 8);
            BatVolt2[j+1] = (uint8_t)(Voltage2[i] & 0xFF);
            j+=2;
        }

        //Closes solenoids after data collection to gain pressure data
        for(int i=1; i<5; i++){
            closeSolValve(i);
        }

        //Storing data in 1 array
        for(int i=21; i<25; i++){
            EBdata[i] = Vpressure2[i];
        }
        for(int i=25; i<31; i++){
            EBdata[i] = temp2[i-25];
        }
        for(int i=31; i<39; i++){
            EBdata[i] = BatVolt2[i-31];
        }

        downlink(EBdata,39);
        burncounter++;
    }
}

