/*
M-SAT M3 mission, the most cursed object to ever grace the face of the Earth
To those that made it possible:

Dr. Pernicka

Ian Williams
Drake Beaman
Emily Dodemeade

Rayan Barghchoun
Aneliese Hebensteit
Luke Serrano
Kai Shafe
Mason Philips
Dheemanth Mannem
Bart Smith
Hannah Rodecap
Everett Judy
Yumeka Nagano
Hieu Hoang
Caden Dennis

and many, many more...
*/

#include "StateMachine/missionmodes.h"

int MissionMode;

int main()
{
    //Setting all LED pins as output
    //DDRG = (1<<DDG2);
    //DDRJ = (1<<DDJ6)|(1<<DDJ5)|(1<<DDJ4)|(1<<DDJ3)|(1<<DDJ2)|(1<<DDJ1)|(1<<DDJ0);

    MissionMode = 1;
	
	//PORTG &= ~(1<<PG2);
	//PORTJ ^= (1<<PJ1);
	
	for(int i = 0; i < 15; i++){
		_delay_ms(1000);
	}
	
    while(1)
    {
        switch(MissionMode)
        {
            case 1:
				//Initialization
                //PORTG ^= (1<<PG2);
                initilization();
                MissionMode = 2;
				break; 

            case 2:
                //Healthcheck
                //PORTG &= ~(1<<PG2);
                //PORTJ ^= (1<<PJ6);
                healthcheck();
                MissionMode = 3;
                break;

            case 3:
                //Electric Burn
				//PORTJ &= ~(1<<PJ6);
				//PORTJ ^= (1<<PJ5);
                eburn();
                MissionMode = 4;
                break;
                
            case 4:
                //End of Life
                //PORTJ &= ~(1<<PJ5);
				//PORTJ ^= (1<<PJ4);
                EPS_Transmit(0x01);
                //PORTJ ^= (1<<PJ2);
                EPS_Transmit(0x10);
                //PORTJ ^= (1<<PJ0);
                break;
        }
    }
}
