/*
 * app.c
 *
 *  Created on: Jan 18, 2024
 *      Author: Eng. André A. M. Araújo
 */

#include "app.h"

#include "cli.h"

#include "SEMTECH_main.h"

void APP_Init(void)
{
	uint8_t blink_counter = 0;

	CLI_Write("LoRa G-NiceRF Module test with STM32C011 \r\n");

	for (blink_counter = 0; blink_counter < 6; blink_counter++)
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_Delay(250);
	}



	APP_Run();
}

void APP_Run(void)
{
	while (1)
	{
		SEMTECH_main();
	}
}
