#include "sensors.h"

#ifndef SAFEMODES_H
#define SAFEMODES_H

extern int LineToggle12V;

// Variable declaration for readability
const uint8_t I2C_ERROR_CODE = 0x89;
const uint8_t PPU_ACK = 0xAA;
const uint8_t EPS_ACK = 0x30;
const uint8_t PRESS_SENS_0 = 6;
const uint8_t PRESS_SENS_1 = 7;
const uint8_t PPU_CHECK = 8;
const uint8_t RADIO_CHECK = 9;
/* ----------------------------------------------
Device check array is indexed as followed:
0-5:    Thermal sensors
6-7:    Pressure transducers
8:      PPU
9:     Radio
-------------------------------------------------
All indices are set to false until initialization()
------------------------------------------------*/
volatile static uint8_t deviceCheck[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
volatile static uint16_t CritBatPer = 0x02FD;
	
volatile const static uint8_t RESTART_12V = 0x04;
volatile const static uint8_t RESTART_5V = 0x05;
volatile const static uint8_t NUM_RESTARTS = 0x12;
volatile const static uint8_t FC_MAX_RESTARTS = 2;

int check(int input, int min, int max);
int radioCheck(void);
void PrimarySafeMode(void);
void ThermalSafeMode(int tMin, int tMax, int SensorCount);

#endif