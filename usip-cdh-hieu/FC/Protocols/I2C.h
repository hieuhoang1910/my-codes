#define F_CPU 8000000UL  //This is 8MHz clockspeed, used for delay.h for delay timings
#define F_I2C 100000LL    //Sets I2C speed to 100 kHz, a common speed for I2C

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* These define functions are used to verify transmission errors and successes with status codes.
The defined values are used to compare with the first 5 bits in the TWSR register after an action.
The last 3 bits are not considered becuase they relate to clock frequency and considering them may cause false positives.
 
EX: if the first 5 values of TWSR = 0x08, the register = 0000 1000
status codes can be found in the ATMEGA2560 datasheet pg.249 */

#define START 0x08                              //Start condition
#define RSTART 0x10                             //Repeated start condition
#define SLAW_ACK 0x18                           //SLA+W has been transmitted, ACK has been received:
#define DATAW 0x28                              //Data byte has been transmitted, ACK has been received
#define SLAR_ACK 0x40                           //SLA+R has been transmitted, ACK recieved
#define DATAR_ACK 0x50                          //Data byte recieved, sent ACK
#define DATAR_NACK 0x58                         //Data byte recieved, sent NACK

/* These next define functions are used to set values in the TWCR register (Two Wire Control Register).
This controls the interupt flag and data transmission */

#define CRSTART 0xA4                            //TWCR = 10100100 (enabling TWINT, TWSTA, and TWEN), 
#define SEND 0x84                               //TWCR = 10000100 (enabling TWINT, TWEN)
#define RECEIVE_NACK 0x84                       // |
#define RECEIVE_ACK 0xC4                        // |
#define CRSTOP 0x94                             // |      

void ERROR(void);                                //Error function. Need to flesh out
void i2c_init(void);                            //Initilizes baud rate
void i2c_start(void);                           //Starts i2c and sets read or write
void i2c_address(int address,char mode);        //Transmits address with R/W bit
void i2c_dataTransmit(int data);                //Transmits data
uint8_t i2c_dataReceive_nack(void);  
uint8_t i2c_dataReceive_ack(void);              //Recieves data
void i2c_stop(void);                            //Stops i2c


class i2c                                       //Class used for transmit/receive functions
{
    public:
    uint8_t address, register_address = 0xFF;   //Default value to specify not to write to an address

    void transmit(uint8_t[],int);
    uint8_t* receive(int);
};

#endif