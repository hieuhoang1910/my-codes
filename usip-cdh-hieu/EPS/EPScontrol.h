#include "USART.h"

#ifndef EPScontrol_H
#define EPScontrol_H

#define PING 0x00

#define ON_12V 0x01
#define OFF_12V 0x02

#define ON_3V 0x10
#define OFF_3V 0x11

#define RESTART12V 0X04
#define RESTART5V 0X05

#define BATTERY_CHECK 0x03
#define SHUTDOWN 0x06
#define HEALTH_STATUS 0X07 
#define RESET_SOC 0x08
#define READING 0x09

#define ACK 0x30
#define NACK 0x31

#define NUM_FC_RESTARTS 0x12

volatile static uint8_t flightComputerRestarts = 0;

void VoltControl_Init(void);
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
void set3VControl(bool);
void set5VControl(bool);
void set12VControl(bool);
void BatteryCheck(void);

#endif