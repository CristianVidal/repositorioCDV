/*
 * api.c
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#include "sapi.h"
#include "api.h"
#include "apiSD.h"

static void FormatInformationArray(uint16_t valor, uint8_t * destiny, uint8_t pos);

uint8_t apiReadTemperatureHumdity(uint16_t * dataTemp, uint16_t * dataHum) {
	uint16_t adcValue1, adcValue2;

	adcValue1 = adcRead(CH1); // temp
	(*dataTemp) = adcValue1;

	adcValue2 = adcRead(CH2); // hum
	(*dataHum) = adcValue1;

	return _API_STATE_OK;
}

uint8_t apiReadWind(uint16_t * dataWind) {
	uint16_t adcValue3 = 0;

	adcValue3 = adcRead(CH3); // viento
	(*dataWind) = adcValue3;

	return _API_STATE_OK;
}

uint8_t apiReadSensor(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind) {

	apiReadTemperatureHumdity(dataTemp, dataHum);

	apiReadWind(dataWind);

	return _API_STATE_OK;
}

static void FormatInformationArray(uint16_t valor, uint8_t * destiny, uint8_t pos){
	destiny[pos] = valor/1000 		+ '0';// unidad de mil
	pos++;
	destiny[pos] = (valor%1000)/100 	+ '0';//centena
	pos++;
	destiny[pos] = (valor%100)/10 	+ '0';//decena
	pos++;
	destiny[pos] = (valor%10) 		+ '0';//unidad
	pos++;
}

uint8_t apiProcessInformation(uint16_t dataTemp, uint16_t dataHum, uint16_t dataWind, uint8_t * destiny) {

	/*
	 * posiciones	= 0123 4 5678 9 0123 4 5  6  7
	 * string 		= XXXX ; YYYY ; ZZZZ ; \r \n \0
	 */

	FormatInformationArray(0, destiny, 0);
	destiny[4] = ';';
	FormatInformationArray(dataHum, destiny, 5);
	destiny[9] = ';';
	FormatInformationArray(dataWind, destiny, 10);
	destiny[14] = ';';
	destiny[15] = '\r';
	destiny[16] = '\n';
	destiny[17] = '\0';

	return _API_STATE_OK;
}

uint8_t apiWriteSD(uint8_t * filename, uint8_t * stringData) {

	if(apiSD_Init() == _API_STATE_ERROR) {
		// error
	} else {
		if(apiSD_Write((char*)filename, (char*)stringData) == _API_STATE_ERROR) {
			// error
			gpioWrite( LEDR, ON );
		} else {
			// ok
			gpioWrite( LEDG, ON );
		}
	}

	return _API_STATE_OK;
}
