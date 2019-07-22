/**
  ******************************************************************************
  * @file    	main.c 
  * @author  	Alfred Lohmann
  *        	  HAW-Hamburg
  *          	Labor für technische Informatik
  *          	Berliner Tor  7
  *          	D-20099 Hamburg
  * @version V1.0
  * @date    23.05.2013
  * @brief   Main program body
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "output.h"
#include "basisFunktionen.h"
#include "oneWireBus.h"
#include "error.h"
//--- For GPIOs -----------------------------
//Include instead of "stm32f4xx.h" for
//compatibility between Simulation and Board
//#include "TI_memory_map.h"

//--- For Touch Keypad ----------------------
//#include "keypad.h"

//--- For Timer -----------------------------
//#include "timer.h"
#include "basisFunktionen.h"
/**
  * @brief  Main program
  * @param  None
  */
int main(void)
{
	
  Init_TI_Board();
  initTft();
  bus_Init();
   
  timerinit();
    //while(1){resetPulse();}
    //while(1){writeBit(1);}
    //while(1){writeBit(0);}
    //while(1){readBit();}
    
    
  searchROM();
    
//    while(1) {
////        GPIOE->BSRRH = (0x01<<7);
//        readTemp1Sensor();
//        GPIOE->BSRRL = (0x01U<<18);
//    }
    
	while(1) {
		for(int i = 1; i<=numROMs; i++) {
			readTemp(i,FoundROM[i]);
		}
	}
    



}
// EOF
