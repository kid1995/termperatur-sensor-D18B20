#ifndef _BASISFUNK_H
#define _BASISFUNK_H

//#include "stm32f10x.h" //Simulation
#include "stm32f4xx.h" //TI-Board
#include "TI_memory_map.h"
#include "timer.h"

#define PG_0   0
void setLow(int pin);
void setHigh(int pin);
int readBus(void);
void delay(int uSec);
int readBit(void);
int readByte(void);
int writeBit(char bitVal);
int writeByte(char byteVal);
int resetPulse(void);
void powerWithVCC(void);
void setOpenDrainMode(int pin);
void setPushPullMode(int pin);
void bus_Init(void);
#endif
