#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "timer.h"
#include "TI_Lib.h"
#include "math.h"
#include <stdint.h>
#include <stdio.h>
#include <tft.h>
//#include "stm32f10x.h" //Simulation
#include "stm32f4xx.h" //TI-Board
#define TEMPMAX	85.0000

/**
  * @brief  Init für das Display
  */
void initTft(void);

/**
  * @brief  berechnet und druckt die Werte auf das Dislay
  * @param float: Winkel des Drehgebers
	* @param float: Drehgeschwindigkeit
  */
void printData(int, float);

void printSensorID(int, unsigned char*);

#endif

