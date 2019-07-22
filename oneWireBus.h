#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include "basisFunktionen.h"
#include "tft.h"
#include <stdbool.h>
extern int numROMs;
extern unsigned char FoundROM[5][8];

void xor(void);
unsigned char DO_CRC(void);
int first(void);
int next(void);
void reset_search(void);
void FindDevices(void);
void searchROM(void);

int readTemp(int nr, unsigned char* readROM);
float calculateTemp(uint8_t tempHigh, uint8_t tempLow);
int readTemp1Sensor(void);
#endif
