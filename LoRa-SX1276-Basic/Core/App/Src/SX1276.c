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

	// Sleep
	SX1276_Sleep();

	// Set frequency
	SX1276_SetFrequency(915E6);

	// Set base address

	// Set LNA Boost

	// Set output power

	// Standby
}

uint8_t	SX1276_ReadRegister(uint8_t address)
{
	uint8_t SPI_buffer[1];

	SPI_buffer[0] = address;

	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 0);
	HAL_SPI_Transmit (&hspi1, SPI_buffer,  1, SPI_TIMEOUT);
	HAL_SPI_Receive  (&hspi1, SPI_buffer,  1, SPI_TIMEOUT);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 1);

	return SPI_buffer[0];
}


void 	SX1276_WriteRegister(uint8_t address, uint8_t data)
{
	uint8_t SPI_buffer[2];

	SPI_buffer[0] = address;
	SPI_buffer[1] = data;

	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 0);
	HAL_SPI_Transmit (&hspi1, SPI_buffer,  2, SPI_TIMEOUT);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, 1);
}

uint8_t SX1276_GetVersion(void)
{
	return SX1276_ReadRegister(0x42);
}

void 	SX1276_Sleep(void)
{
	SX1276_WriteRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void 	SX1276_Standby(void)
{
	SX1276_WriteRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}
void	SX1276_SetFrequency(uint32_t frequency)
{
	uint64_t frf = ( (uint64_t)frequency << 19) / 32000000;

	SX1276_WriteRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
	SX1276_WriteRegister(REG_FRF_MID, (uint8_t)(frf >>  8));
	SX1276_WriteRegister(REG_FRF_LSB, (uint8_t)(frf      ));
}
