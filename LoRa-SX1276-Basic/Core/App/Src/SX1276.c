/*
 * SX1276.c
 *
 *  Created on: Jan 19, 2024
 *      Author: Eng. André A. M. Araújo
 */

#include "SX1276.h"

void 	SX1267_Init(void)
{
	uint8_t SPI_buffer[8];
	uint8_t version = 0;

	HAL_GPIO_WritePin(LORA_NRST_GPIO_Port, LORA_NRST_Pin, 0);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 1);

	HAL_SPI_Transmit(&hspi1, SPI_buffer, 1, SPI_TIMEOUT);

	HAL_GPIO_WritePin(LORA_NRST_GPIO_Port, LORA_NRST_Pin, 1);
	HAL_Delay(100);

	version = SX1276_GetVersion();

	if (version == 0x12)
	{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
	}
}

uint8_t SX1276_GetVersion(void)
{
	uint8_t SPI_buffer[8];

	SPI_buffer[0] = 0x42;

	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 0);
	HAL_SPI_Transmit(&hspi1, SPI_buffer, 1, SPI_TIMEOUT);
	HAL_SPI_Receive(&hspi1, SPI_buffer, 1, SPI_TIMEOUT);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 1);

	return SPI_buffer[0];
}
