/*
 * cli.c
 *
 *  Created on: Jan 18, 2024
 *      Author: Eng. André A. M. Araújo
 */

#include "cli.h"

void CLI_Write(char * buffer)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}
