#include "I2C.h"

//Error function
void ERROR(void)
{
    i2c_stop();
    i2c_start();
    TWDR = 0x00;                                         //Loads address into data register
    TWCR = SEND;                                             //Transmits data
    while (!(TWCR & (1<<TWINT)));
    TWDR = 0x89;
}


/* The functions below are specific bit manipulation to specific registers to the atmega 2560. Everything here can be found in the datasheet for further details if not described here*/

//Initilization
void i2c_init()
{
	TWSR &= ~((1<<TWPS1) | (1<<TWPS0)); 			         // pre-scalar 1
	TWBR = ((F_CPU / F_I2C) - 16) / 2; 				         // baud rate factor 12
}

//Start condition
void i2c_start()                                         
{
    TWCR = CRSTART;                                          //Starts I2C
    while(!(TWCR & (1<<TWINT)));				             //Waits for start condition to be sent, sets the interrupt flag while busy. writing a one to this bit clears the flag and allows to move foward (pg 243 in datasheet)
    if ((TWSR & 0xF8) != START ||(TWSR & 0xF8) != RSTART )   //(TWSR & 0xF8) takes first 5 bits of TWSR to compare to status code
    {
                                             //If status does not match status code, error
    }                                                       
}

//Address transmission
void i2c_address(uint8_t address,char mode)                  //Input address of device and set mode, 'w' for write, 'r' for read
{
    uint8_t addressN;
    addressN = address;                                      //Setting to new variable just in case to avoid the actual address from getting overwritten
    switch (mode)                                            //Checks what mode we want
    {
        case 'r':
        addressN <<= 1;                                      //Left shifts address to add a 0 at the end
        addressN |= 0b1;                                     //Changes last bit to a 1 for reading
        break;

        case 'w':
        addressN <<= 1;                                      //Left shifts address to add a 0 at the end for writing
        break;
    }
    
    TWDR = addressN;                                         //Loads address into data register
    TWCR = SEND;                                             //Transmits data
    while (!(TWCR & (1<<TWINT)));				             //Waits for interrupt flag to clear
    if ((TWSR & 0xF8) != SLAW_ACK && mode == 'w')            //Checks for not matching error codes for write case   
    {
		ERROR();

    }
    else if((TWSR & 0xF8) != SLAR_ACK && mode == 'r')        //Checks for not matching status codes for read case 
    {
		ERROR();
    }
}

//Send data byte
void i2c_dataTransmit(int data)
{
    TWDR = data;                                              //Loads data into data register     
    TWCR = SEND;                                              //Transmits data
    while (!(TWCR & (1<<TWINT)));				              //Waits for interrupt flag to clear
    if ((TWSR & 0xF8) != DATAW)                               //Checks for not matching status codes
    {

    } 
}

//Receive data byte nack
uint8_t i2c_dataReceive_nack()
{
    uint8_t data;
    TWCR = RECEIVE_NACK;                                      //Sets bits in control register to recieve nack mode
    while (!(TWCR & (1<<TWINT)));                             //Waits for interrupt flag to clear
    data = TWDR;                                              //Loads data received into data register
    if ((TWSR & 0xF8) != DATAR_NACK)                          //Checks for not matching status code
    {
        data = 0x89;                             
    } 
    return data;                                              //returns data to be used outside function
}   

//Receive data byte ack
uint8_t i2c_dataReceive_ack()
{
    uint8_t data;
    TWCR = RECEIVE_ACK;                                       //Sets bits in control register to recieve ack mode
    while (!(TWCR & (1<<TWINT)));
    data = TWDR;                                              //Loads data recieved into data register
    if ((TWSR & 0xF8) != DATAR_ACK)                
    {
        data = 0x89;                                             
    } 
    return data;
}

//Stop condition
void i2c_stop()
{
    TWCR = CRSTOP;                                            //Sets bits in control register to stop I2C transmission for the packet
    while (!(TWCR & (1 << TWSTO)));                        
}

/* Functions below take everything above and inplement them to consolidate and simplify using I2C. They automate sending and recieving
multiple packets of data and is done with one line. transmitting requires an input of an array with all the data you want to send and the amount of packets you want to send.
receiving is simply an input of how many packets you want to receive. addresses are specified before hand by setting it equal to a variable before using the function.
If a register is required to write to before hand, the register address can be set to a variable

to use for any I2C communication:
for this example, using a variable called example using class type i2c (i2c example;)

1) for transmitting:                    2) for receiving:                   3) for receiving from a specific register:
example.address = 0x04                  example.address = 0x04              example.address = 0x04
int data[3] = {1,2,3};                  example.receive(3)                  example.register_address = 0x65
example.transmit(data,3)                                                    example.receive(3) */

void i2c::transmit(uint8_t transmit_data[],int transmit_size)
{
    i2c_start();
    i2c_address(address,'w');
    for(int i = 0; i < transmit_size; i++)                    //Loops through transmitting 8 bit data packets for transmit_size amount of times
    {
        i2c_dataTransmit(transmit_data[i]);                   //Takes every value in input data array and transmits it one at a time
    }
    i2c_stop();
}

uint8_t* i2c::receive(int receive_size)
{
    static uint8_t received_data[0]{};                        //Initializes an empty array with static type to be able to use data outside of function
    i2c_start();
    if(register_address == 0xFF)                              //Default value if specific register address is not assigned. this will run a sequence without writing to register first
    {
        i2c_address(address,'r');
        for(int i = 0; i < receive_size; i++)                 //Loops through sending data with amount of data wanted to receive specified in input
        {
            if(receive_size == 1 || i == receive_size-1)      //Checks if the the amount of data is 1 or if it has reached the last value of the array
            {                                                 //if true, it will return a nack in order to proceed with the program. If an ack is returned at the end of the data received instead of nack, it will infinitely wait for data
                received_data[i] = i2c_dataReceive_nack();
            }
            else
            {
                received_data[i] = i2c_dataReceive_ack();
            }
        }
    }
    else
    {
        i2c_address(address,'w');                             //If a register address is specified, it will write to the register address first before receiving data
        i2c_dataTransmit(register_address);
        i2c_start();
        i2c_address(address,'r');
        for(int i = 0; i < receive_size; i++)
        {
            if(receive_size == 1 || i == receive_size-1)
            {
                received_data[i] = i2c_dataReceive_nack();
            }
            else
            {
                received_data[i] = i2c_dataReceive_ack();
            }
        }
    }
    i2c_stop();
    return received_data;
}
