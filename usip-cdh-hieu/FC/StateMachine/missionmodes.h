#include "safemodes.h"

#ifndef MISSIONMODES_H
#define MISSIONMODES_H

volatile const static int tempMin = -20;             //flight: -15
volatile const static int tempMax = 45;              //flight: 45
volatile const static int tempSensorCount = 6;

volatile const static int proptempMin = -20;         //flight: -20
volatile const static int proptempMax = 140;         //flight: 140

volatile const static int pressMin = 15;			//flight 15?
volatile const static int pressMax = 50;			//flight 50
volatile const static int pressureSensorCount = 2;

void initilization(void);
void downlink(uint8_t data[], int datasize);
void healthcheck(void);
void eburn(void);

#endif