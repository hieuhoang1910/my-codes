#include "../Protocols/I2C.h"
#include "../Protocols/USART.h"

#ifndef SENSORS_H
#define SENSORS_H

/* For description of how functions inside data collection functions work, refer to I2C.cpp */

//MUX
#define ch1 0x01
#define ch2 0x02
#define ch3 0x04
#define addressMUX 0x70

//IMU registers
#define PWR_MGT_1 0x06
#define REG_BANK_SEL 0x7F
#define GYRO_CONFIG_1 0x01
#define GYRO_SMPLRT_DIV 0x00
#define IMUXH 0x33 //previously 0x35 check data sheet (we want x and y)

//ADC registers
#define conversionRegADC 0x00
#define configRegADC 0x01
#define LSBConfigADC 0x83
#define MSBConfigADC_1 0x84  
#define MSBConfigADC_2 0xB4                     //(PT2) we will have to change this to indicate which pressure transducer we are talking to (PT1 = 10000100)

//PPU
#define frequencyHIGH 0x00
#define frequencyLOW 0xC7
#define dutyHIGH 0x00
#define dutyLOW 0x13

//addresses
#define addressTS0 0x48 //battery A on channel 2
#define addressTS1 0x49
#define addressTS2 0x4A //thruster on channel 3
#define addressTS3 0x4B //nitrogen tank on channel 3
#define addressTS4 0x4C //battery C on channel 3
#define addressTS5 0x4D //battery B on channel 2
#define addressTS6 0x4E //prop tank on channel 3
#define addressTS7 0x4F //battery D on channel 3

#define addressIMU 0x69
#define addressADC 0x48
#define addressPPU 0x22

void channel_sel(uint8_t);

void imuinit(void);
void ppuinit(void);

uint8_t* TEMP(void);
uint8_t* IMU(void);
uint8_t* ADCS(void);

uint16_t* Battery(void);

uint8_t PPU_ping(void);
uint8_t PPU_HC(void);
uint8_t* PPU_ADC_echo(void);
uint8_t* PPU_duty_echo(void);
void PPU_on(void);
void PPU_off(void);
void PPU_RAM_echo(void);
void PPU_GD_on(void);
void PPU_GD_off(void);

void openSolValve(const uint8_t solValvNum);
void closeSolValve(const uint8_t solValvNum);

#endif