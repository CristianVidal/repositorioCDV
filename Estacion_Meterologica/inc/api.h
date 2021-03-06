/*
 * api.h
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#ifndef API_H_
#define API_H_

#include "sapi.h"
#include "apiSD.h"

uint8_t apiReadTemperatureHumdity		(uint16_t * dataTemp, uint16_t * dataHum);
uint8_t apiReadWind						(uint16_t * dataWind);
uint8_t apiReadSensor					(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind);
uint8_t apiProcessInformation			(uint16_t dataTemp, uint16_t dataHum, uint16_t dataWind, uint8_t * destiny);
uint8_t apiWriteSD						(uint8_t * filename, uint8_t * stringData);

#endif /* API_H_ */
