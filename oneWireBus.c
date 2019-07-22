#include "oneWireBus.h"
#include "output.h"
#include "basisFunktionen.h"
#include "error.h"
#include <stdbool.h>

#define   FALSE 0
#define   TRUE 1
#define CRC9_Polynom 0B00101111 
/////////////////////////////// GLOBAL VARIABLES /////////////////////////////////////////////
unsigned char ROM_NO[8]; // ROM Bit
unsigned char FoundROM[5][8]; // table of found ROM codes
unsigned char crc;
int numROMs;
int lastDiscrepancy;
int lastFamilyDiscrepancy;
int lastDeviceFlag;
int search_direction;
int id_bit_number;
int last_zero;
int id_bit;
int cmp_id_bit;


/**
* @brief handles the crc check
* @param 
* @return the crc
*/
unsigned char DO_CRC(){

   uint8_t crc = 0;
    
    // Maxim 1-Wire 8-bit CRC (poly = x^8 + x^5 + x^4 + 1)
	for (int i = 0; i < 8; ++i) {
		uint8_t val = ROM_NO[i];

		for (int y = 0; y < 8; ++y) {
			if ((crc ^ val) & 0x01) {
				// 0x18 = 00011000 
				// 0x80 = 10000000
				crc = ((crc ^ 0x18u) >> 1) | 0x80u; // XOR mit Bit 0 danach shiften und if bit 8 ist 1 dann sowie bit 0 4 5
			} else {
				crc >>= 1;
			}
			val >>= 1;
		}
	}
    return crc;
}


/**
* @brief FIRST Function performed by setting LastDiscrepancy, 
*	LastFamilyDiscrepancy, and LastDeviceFlag to zero and then doing the search.
* @param 
* @return next()
*/
int first(void) {
	reset_search();
	return next();
}

/**
* @brief NEXT Function is performed by leaving the state unchangedfrom the previous search and performing another search
* @param 
* @return TRUE or FALSE, If there are no more devices on the 1-Wire then false is returned.
*/

int next(void) {
	int resetPulseFlag;
	char maske = 1;
	int nxt = FALSE;
	int n = 0; //ROM_NO index
	crc = 0; // reset crc
	resetPulseFlag = resetPulse(); // perform a 1 wire reset
	if(resetPulseFlag || lastDeviceFlag) { //resetPulseFlag = lastDeviceFlag = 1
		reset_search(); //2
		return FALSE;
	}
	id_bit_number = 1; last_zero = 0;
	writeByte(0xF0); // send search command
	do { // for all eight bytes
		id_bit = readBit();cmp_id_bit = readBit(); // Read bit and complement id bit
		if(id_bit == 1 && cmp_id_bit == 1) {
			reset_search(); //2

			return FALSE;
		}
        
		else if(id_bit == 0 && cmp_id_bit == 0) {
			if(id_bit_number == lastDiscrepancy) {
                printf("E1\n");
				search_direction = 1;
            }
			else if(id_bit_number > lastDiscrepancy) {
				search_direction = 0;
                printf("E2\n");
            }
			else {//id_bit_number < lastDiscrepancy 
				search_direction = (ROM_NO[n]&maske)  ; //set search_direction to id_bit_number
                printf("E3\n");
            }
			if(search_direction == 0) last_zero = id_bit_number;
		}
		else { //all devices coupled have 0 or 1, id_bit = 0 or cmp_id_bit == 1 and contrary
			search_direction = id_bit;
		}
		if(search_direction == 1) {
			ROM_NO[n] |= maske; // write 1 into ROM at pos maske
		}
		else //search_direction == 0
			ROM_NO[n] &= ~maske; // write 0 into ROM at pos k , last zero is the position in ROMNO to manipulate as 0
		writeBit(search_direction);//send to 1 Wire
		id_bit_number++;
		maske = maske << 1;
		if(maske == 0){ // 8 bits have done to work, go to new ROM, and do check crc

			n++;
			maske++; // make maske = 1 again
		}
        
	}while(n<8); // lesen 8 mal und speichern zu ROMNO[]
    crc = DO_CRC();
	if(id_bit_number < 65 || crc) {//crc = 1 --> unsuccesful
		reset_search();
	}
	else { // crc = 0, successful
         printf("LD\n");
		lastDiscrepancy = last_zero;
		if(lastDiscrepancy == 0)
			lastDeviceFlag = 1;
		nxt = TRUE;
	}

	printf("\nROM CODE =%02X%02X%02X%02X%02X%02X%02X%02X\n",
	ROM_NO[7],ROM_NO[6],ROM_NO[5],ROM_NO[4],
	ROM_NO[3],ROM_NO[2],ROM_NO[1],ROM_NO[0]);
	return nxt;
}
/**
* @brief reset function
* @param 
* @return 
*/

void reset_search(void) {
	lastDiscrepancy = 0;
	lastFamilyDiscrepancy = 0;
	lastDeviceFlag = 0;
}

/**
* @brief find device function
* @param 
* @return 
*/
void FindDevices(void){
	
	if(!resetPulse()) {
		if(first()) {
			numROMs = 0;
			do {
				numROMs++;
				for(int i = 0; i<8; i++) {
					FoundROM[numROMs][i] = ROM_NO[i];
				}
				printSensorID(numROMs,FoundROM[numROMs]);
////				printf("\nROM CODE =%02X%02X%02X%02X%02X%02X%02X%02X\n",
////				FoundROM[numROMs][7],FoundROM[numROMs][6],FoundROM[numROMs][5],FoundROM[numROMs][4],
////				FoundROM[numROMs][3],FoundROM[numROMs][2],FoundROM[numROMs][1],FoundROM[numROMs][0]);
			}while(next() && (numROMs < 5));
		}
	}
}

/**
* @brief for Schritte 1
* @param 
* @return 
*/
void searchROM(void) {
	FindDevices();
    
	
	if(numROMs == 0) {
		errorCode = NOSENSOR;
		printErrorMessage();
	}
}

int readTemp1Sensor(void) {
    uint8_t scratchpad [10];  // local memory for scratchpad
    uint8_t tempHigh = 0;
	uint8_t tempLow = 0;
    
    resetPulse();
    writeByte(0xCC);
    
    writeByte(0x44);
	setPushPullMode(0); //PG0
	delay(750000);	
	setOpenDrainMode(0);
	resetPulse();
	writeByte(0xCC);
    
    writeByte(0xBE); // read scratchpad
    
	for (int i=0;i<9;i++){
		scratchpad[i]=readByte();
	}
	
	tempHigh = scratchpad[1];
	tempLow = scratchpad[0];
	float temperature = calculateTemp(tempHigh,tempLow);

	printData(1, temperature);
	return 0;
    
}


/**
* @brief readTemp function, read temp from scratchpad
* @param 
* @return 
*/
int readTemp(int nr, unsigned char* readROM) {
	uint8_t scratchpad [10];  // local memory for scratchpad
	uint8_t tempHigh = 0;
	uint8_t tempLow = 0;
	
	resetPulse();
	writeByte(0x55);
	for(int i = 0; i < 8; i++){
		writeByte(readROM[i]);
	}
	writeByte(0x44);
	setPushPullMode(0); //PG0
	delay(750000);	
	setOpenDrainMode(0);
    while(1){
	resetPulse();
	writeByte(0x55);

	for(int i = 0; i < 8; i++){
		writeByte(readROM[i]);
       
	}
	writeByte(0xBE); // read scratchpad
    GPIOG-> BSRRL = 0x01U<<4; 
	for (int i=0;i<2;i++){
		scratchpad[i]=readByte();
	}
     GPIOG-> BSRRH = 0x01U<<4;  

	
	tempHigh = scratchpad[1];
	tempLow = scratchpad[0];
	float temperature = calculateTemp(tempHigh,tempLow);

	printData(nr, temperature);
}
	return 0;
    
}

float calculateTemp(uint8_t tempHigh, uint8_t tempLow) {
    
    uint16_t uint_temperature = 0;
    uint_temperature = (uint16_t)tempHigh;
    uint_temperature = uint_temperature << 8;
    uint_temperature |= tempLow;
    

    int16_t int_temperature = (int16_t) uint_temperature;
    float float_temperature = (float) int_temperature;
    float_temperature /= 16;
    
   
    
    return float_temperature;
}




