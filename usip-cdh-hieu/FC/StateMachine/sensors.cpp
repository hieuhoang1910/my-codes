#include "sensors.h"

void channel_sel(uint8_t channel)               //Used to select what channel on the mux we want on
{
    i2c MUX;
    uint8_t sel[1] = {channel};
    MUX.address = addressMUX;
    MUX.transmit(sel,1);
}

void imuinit(void)                              //Initilizes and configures IMU before collecting data
{
    i2c initIMU;

    initIMU.address = addressIMU;
    channel_sel(ch2);

    uint8_t imuinit1[2] = {REG_BANK_SEL,0x00};
    uint8_t imuinit2[2] = {PWR_MGT_1,0x01};
    initIMU.transmit(imuinit1,2);
    initIMU.transmit(imuinit2,2);
}

void ppuinit(void)                              //Sets override frequency and duty values
{
    i2c initPPU;

    initPPU.address = addressPPU;
    channel_sel(ch1);

    uint8_t freqOR[4] = {0x06,0x00,0x06,0xC7};
    uint8_t dutyOR[4] = {0x05,0x00,0x05,0x13};
    initPPU.transmit(freqOR,4);
    initPPU.transmit(dutyOR,4);
}

uint8_t* TEMP(void)                             //Temperature collection from all sensors available
{
    i2c TEMP;
    static uint8_t temp[6];

    channel_sel(ch2);
    TEMP.address = addressTS0;
    temp[0] = *(TEMP.receive(1));  //battery A temp
    TEMP.address = addressTS5;
    temp[1] = *(TEMP.receive(1));  //battery B temp

    channel_sel(ch3);
    TEMP.address = addressTS4;
    temp[2] = *(TEMP.receive(1));  //battery C temp
    TEMP.address = addressTS7;		
    temp[3] = *(TEMP.receive(1));  //battery D temp
    TEMP.address = addressTS6;
    temp[4] = *(TEMP.receive(1));  //prop tank temp
    TEMP.address = addressTS2;
    temp[5] = *(TEMP.receive(1));  //thruster temp
    return temp;
}

uint8_t* IMU(void)                              //Collects rotation data
{
    i2c IMU;
    
    IMU.address = addressIMU;
    IMU.register_address = IMUXH;
    channel_sel(ch2);

    uint8_t* rotation = IMU.receive(4);
    return rotation;
}

uint8_t* ADCS(void)                             //Collects pressure data
{
    i2c ADCS;
    
    ADCS.address = addressADC;
    ADCS.register_address = conversionRegADC;
    channel_sel(ch1);
    static uint8_t pressure[4];

    uint8_t pin1config[3] = {configRegADC, MSBConfigADC_1, LSBConfigADC};
    uint8_t pin2config[3] = {configRegADC, MSBConfigADC_2, LSBConfigADC};

    ADCS.transmit(pin1config,3);
    uint8_t* p1 = ADCS.receive(2);
    ADCS.transmit(pin2config,3);
    uint8_t* p2 = ADCS.receive(2);

    for(int i = 0; i < 2; i++){
        pressure[i] = p1[i]; 
        pressure[i+2] = p2[i];
    }
    return pressure;
}

//Battery Voltages
uint16_t* Battery(void){
    uint8_t pBatVolt[8];
    EPS_Transmit(0x03);

    for(int i=0; i<8; i++){
        pBatVolt[i] = EPS_Receive();
    }

    static uint16_t BatVolt[6];
    int j = 0;
    for(int i=0; i<5; i++){
        BatVolt[i] = (pBatVolt[j]<<8)|(pBatVolt[j+1]);
		j+=2;
    }

	
    uint16_t mainBat = BatVolt[3];
	int mainBatnum = 3;
    if(BatVolt[3] < 0x028F){
        mainBat = BatVolt[2];
		mainBatnum = 2;
		if(BatVolt[2] < 0x028F){
			mainBat = BatVolt[1];
			mainBatnum = 1;
			if(BatVolt[1] < 0x028F){
				mainBat = BatVolt[0];
				mainBatnum = 0;
			}
		}
	}
	
    BatVolt[4] = mainBat;  //to have ability to just call voltage of current main battery for checks, rest is to just downlink
	BatVolt[5] = mainBatnum;
	
	if(BatVolt[mainBatnum - 1] < mainBat){
		BatVolt[4] = BatVolt[mainBatnum - 1];
		BatVolt[5] = mainBatnum - 1;
	}
	return BatVolt;
}
   

//PPU Stuff
uint8_t PPU_ping(void)
{
    i2c PPU;
    PPU.address = addressPPU;
    PPU.register_address = 0x00;

    uint8_t* pingbackA = PPU.receive(1);
    uint8_t pingback = pingbackA[0];
    return pingback;
}

uint8_t PPU_HC(void)
{
    i2c PPU;
    PPU.address = addressPPU;
    PPU.register_address = 0x01;

    uint8_t* AppuHC = PPU.receive(1);
    uint8_t ppuHC = AppuHC[0];
    return ppuHC;
}

void PPU_on(void)
{   
    i2c PPU;
    PPU.address = addressPPU;

    uint8_t data[2] = {0x02,0x01};
    PPU.transmit(data,2);
    PPU.receive(1);
}

void PPU_off(void)
{   
    i2c PPU;
    PPU.address = addressPPU;

    uint8_t data[2] = {0x02,0x00};
    PPU.transmit(data,2);
    PPU.receive(1);
}

uint8_t* PPU_ADC_echo(void)
{
    i2c PPU;
    PPU.address = addressPPU;
    PPU.register_address = 0x03;
    
    uint8_t* ADCecho = PPU.receive(8);
    return ADCecho;
}

uint8_t* PPU_duty_echo(void)
{
    i2c PPU;
    PPU.address = addressPPU;
    PPU.register_address = 0x04;
    
    uint8_t* dutyecho = PPU.receive(1);
    return dutyecho;
}

void PPU_RAM_echo(void)
{
    i2c PPU;
    PPU.address = addressPPU;

    uint8_t data[1] = {0x07};
    PPU.transmit(data,1);
}

void PPU_GD_on(void)
{
    i2c PPU;
    PPU.address = addressPPU;

    uint8_t data[2] = {0x07,0x01};
    PPU.transmit(data,2);
    PPU.receive(1);
}

void PPU_GD_off(void)
{
    i2c PPU;
    PPU.address = addressPPU;

    uint8_t data[2] = {0x07,0x00};
    PPU.transmit(data,2);
    PPU.receive(1);
}

void openSolValve(const uint8_t solValvNum)
{
    switch (solValvNum) {
    case 1:
        PORTA |= (1 << PINA1); // Set to 12 V
        _delay_ms(1);
        PORTA |= (1 << PINA0); // Set to 3V3
        break;
    case 2:
        PORTA |= (1 << PINA3); // Set to 12 V
        _delay_ms(1);
        PORTA |= (1 << PINA2); // Set to 3V3
        break;
    case 3:
        PORTA |= (1 << PINA5); // Set to 12 V
        _delay_ms(1);
        PORTA |= (1 << PINA4); // Set to 3V3
        break;
    case 4:
        PORTA |= (1 << PINA7); // Set to 12 V
        _delay_ms(1);
        PORTA |= (1 << PINA6); // Set to 3V3
        break;
    }
    return;
}

void closeSolValve(const uint8_t solValvNum)
{
    switch (solValvNum)
    {
    case 1:
        PORTA &= 0b11111101;
        PORTA &= 0b11111110;
        break;
    case 2:
        PORTA &= 0b11110111;
        PORTA &= 0b11111011;
        break;
    case 3:
        PORTA &= 0b11011111;
        PORTA &= 0b11101111;
        break;
    case 4:
        PORTA &= 0b01111111;
        PORTA &= 0b10111111;
        break;
    }
    return;
}
