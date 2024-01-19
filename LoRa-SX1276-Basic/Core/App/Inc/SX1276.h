/*
 * SX1276.h
 *
 *  Created on: Jan 19, 2024
 *      Author: Eng. André A. M. Araújo
 */

#ifndef APP_INC_SX1276_H_
#define APP_INC_SX1276_H_

#include "main.h"

#define SPI_TIMEOUT 100

extern SPI_HandleTypeDef hspi1;

void 	SX1267_Init(void);
uint8_t SX1276_GetVersion(void);

#endif /* APP_INC_SX1276_H_ */
