/**
  ******************************************************************************
  * @file    	timer.c 
  * @author  	Schoenher, Nils
  * @version  V1.0
  * @date     10.10.2016
  * @brief    Timer related functions
  ******************************************************************************
  */

#include "TI_memory_map.h"
#include "timer.h"
#include <stdio.h>

void timerinit(void){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* Takt fuer Timer 2 einschalten */
    TIM2->CR1 = 0;                      /* Timer disabled                */
    TIM2->CR2 = 0;                      /*                               */
    TIM2->PSC = 1-1;                      /* Prescaler   (84MHz)           */
#ifndef SIMULATION
    TIM2->ARR = 84000000;             /* Auto reload register          */
#else
    TIM2->ARR = 0xffff;             		/* Auto reload register          */
#endif
    TIM2->DIER  = 0;                    /* Interrupt ausschalten         */
    TIM2->CR1 = TIM_CR1_CEN;            /* Enable Timer                  */
}

uint32_t getTimeStamp(){
	return (TIM2->CNT);
}

uint8_t checkSR(){
    uint8_t status = 0 ;
    if((TIM2-> SR & TIM_SR_UIF) == TIM_SR_UIF){
    status = 1;
    TIM2->SR = ~TIM_SR_UIF;
    }
    return status;
}





