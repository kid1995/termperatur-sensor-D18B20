
#include "output.h"

#define FONT 		    2  //Paramenter für die Wertefenster
#define	INPUTXV		 	13
#define	INPUTYV		 	2
#define	WIDTHV 		 	17
#define	ROWSV			12

#define	INPUTXI	 	 	2 //Parameter für die Display Initzialisierung
#define	INPUTYI		 	2
#define	WIDTHI 		 	40
#define	ROWSI			12


/**
  * @brief  Init für das Display
  */
void initTft()
{
	TFT_Init();
	TFT_set_window(FONT, INPUTXI, INPUTYI, WIDTHI, ROWSI);
	TFT_cls();
	TFT_puts("SensorID:\n\r");
	TFT_puts("Temperatur:          C\n\r\n\r");
	TFT_puts("SensorID:\n\r");
	TFT_puts("Temperatur:          C\n\r\n\r");
	TFT_puts("SensorID:\n\r");
	TFT_puts("Temperatur:          C\n\r\n\r");
	TFT_puts("SensorID:\n\r");
	TFT_puts("Temperatur:          C");
	
	
	TFT_set_window( FONT, INPUTXV, INPUTYV, WIDTHV, ROWSV);
	TFT_set_font_color(3);
}

void printData(int pos, float temp)
{
	if (temp == TEMPMAX) return;
	pos--;
	char tempString[9];
	TFT_gotoxy(1,(pos%4)*3+2);
	snprintf(tempString, 9, "%f", temp);
	TFT_puts(tempString);
	TFT_cursor_off();
}

void printSensorID(int pos, unsigned char* ROM)
{
	pos--;
	TFT_gotoxy(1,(pos%4)*3+1);
	char rom[17];
	snprintf(rom, 17, "%02X%02X%02X%02X%02X%02X%02X%02X",
	ROM[7],ROM[6],ROM[5],ROM[4],
	ROM[3],ROM[2],ROM[1],ROM[0]);
	TFT_puts(rom);
	TFT_cursor_off();
}
