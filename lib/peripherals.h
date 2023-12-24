#ifndef PERIPHERALS_H
#include PERIPHERALS_H

typedef struct 
{
    int mode;
}Peripherals;

//
//Initialitaion peripherals
//
void peripheralsInit(void);

//
//Peripherals shutdown
//
void peripheralsShutdown(void);

//
//Peripherals when alarm is set
//
void peripheralsENABLE(void);

//
//Peripherals when alarm is unset
//
void peripheralsDISABLE(void);
#endif PERIPHELARS_H