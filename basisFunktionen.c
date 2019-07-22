
#include "basisFunktionen.h"


void setLow(int pin)
{
	GPIOG->BSRRH = (0x01U << pin);
}

void setHigh(int pin){
	
	GPIOG->BSRRL = (0x01U << pin);
}

int readBus(){
	return (GPIOG->IDR & (0x01U << PG_0));
}

//active waiting for a given time in micro seconds
void delay(int uSec){

	TIM2->CR1 = 0;            /* Enable Timer                  */
    uint32_t stamp = uSec * 84 ;
    TIM2->ARR = stamp;  // ARR zu stamp eingesetzt
    TIM2->CNT =0;
    TIM2->SR = ~TIM_SR_UIF;     //Status Register zu 0 eingesetzt
    TIM2->CR1 = TIM_CR1_CEN;    /* Enable Timer                  */	    
	while((TIM2->SR & 0x01) == 0){}//aktuelle warten 

}

int readBit(){ //k6-18
	setLow(PG_0); // bus on low
	delay(6);
	setHigh(PG_0); // bus on high
	delay(9);
	int result = readBus(); // read from bus
	delay(55);
	return result;
}

int readByte(){
	int result = 0;
	for( int i = 0; i < 8; i++){
		result = result | readBit()<<(i);
	}
	return result;
}

int writeBit(char bitVal){
	if(bitVal == 1){
			setLow(PG_0);
			delay(6);
			setHigh(PG_0);
			delay(64);
		} 
	else if(bitVal == 0){
			setLow(PG_0);
			delay(60);
			setHigh(PG_0);
			delay(10);
		}
	else{
			return -1; // TODO: fehlercode
		}
		return 0;
}

int writeByte(char byteVal){
	int bit = 0;
	
	for (int i = 0; i < 8; i++)	{
		bit = byteVal >> i;
		bit &= 0x01;
		writeBit(bit);
	}
	return 0;
}

int resetPulse(){
	int busZustand;
	setLow(PG_0);
	delay(480);
	setHigh(PG_0);
	delay(70);
	busZustand = readBus();
	delay(410);
	return busZustand;
}

#define VCC_PIN			1

void powerWithVCC()
{
	GPIOG->OTYPER &= ~(0x1U << VCC_PIN);
	GPIOG->BSRRL = (0x1U << VCC_PIN);
}

void setOpenDrainMode(int pin)
{
	GPIOG->OTYPER |= (0x1U<<pin);
	
	setHigh(PG_0); // Set bus pin to high
	powerWithVCC();
}

void setPushPullMode(int pin)
{
	GPIOG->OTYPER &= ~(0x1U<<pin);

	setHigh(PG_0);
	powerWithVCC();
}

void bus_Init(void){
	setOpenDrainMode(PG_0);
	setHigh(PG_0);
	powerWithVCC();
}
