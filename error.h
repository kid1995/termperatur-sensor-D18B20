#ifndef _ERROR_H
#define _ERROR_H
#define NOSENSOR   			1
#define FAIL 						 -1
extern int errorCode;
#endif 

/**
	* @Brief Druckt die fehlermeldung auf das TFT Display
	*/
void printErrorMessage(void);

/**
	* @Brief leert das fehler feld
	*/
void clearErrorField(void);
