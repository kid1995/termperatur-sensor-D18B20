
/* Includes ------------------------------------------------------------------*/
#include "error.h"
#include "tft.h"
#include "string.h"

// Parameter für die Displayausgabe fuer den Error Code
#define FONTE		 2
#define	ERRORX		 6
#define	ERRORY		14
#define	WIDTHE 		35
#define	ROWSE		 1

int errorCode = 0;
static char errorMessage[35];

/**
	* @Brief Setzt die Fehlermeldung nach dem Fehlercode
	*/
void setErrorMessage()
{
	switch(errorCode)
	{
		case(0): strcpy(errorMessage, " "); break;
		case(NOSENSOR): strcpy(errorMessage, "[#101] no sensor found"); break;
	}
}

/**
	* @Brief Druckt die fehlermeldung auf das TFT Display
	*/
void printErrorMessage()
{
	setErrorMessage();	
	int index = 0;	
	
	TFT_set_window( FONTE, ERRORX, ERRORY, WIDTHE, ROWSE); 
	TFT_cls();
	
	TFT_set_font_color(3);
	do{
		TFT_putc(errorMessage[index]);
		index++;
	}while(errorMessage[index] != 0);
	TFT_set_font_color(7);
}

/**
	* @Brief leert das fehler feld
	*/
void clearErrorField()
{
	
	TFT_set_window( FONTE, ERRORX, ERRORY, WIDTHE, ROWSE); 
	TFT_cls();
}

