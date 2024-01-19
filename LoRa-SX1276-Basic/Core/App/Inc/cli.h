/*
 * cli.h
 *
 *  Created on: Jan 18, 2024
 *      Author: Eng. André A. M. Araújo
 */

#ifndef APP_INC_CLI_H_
#define APP_INC_CLI_H_

#include "main.h"

#include <string.h>

extern UART_HandleTypeDef huart1;

void CLI_Write(char * buffer);

#endif /* APP_INC_CLI_H_ */
