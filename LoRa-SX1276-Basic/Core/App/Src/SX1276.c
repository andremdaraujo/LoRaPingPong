/*
 * SX1276.c
 *
 *  Created on: Jan 19, 2024
 *      Author: Eng. André A. M. Araújo
 */

#include "SX1276.h"

static uint8_t _implicitHeader;

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

	SX1276_SetFrequency(915E6);			// 915 MHz
	SX1276_SetSignalBandwidth(500E3);	// 500 kHz
	SX1276_SetSpreadingFactor(11);		// SF11
	SX1276_SetCodingRate4(5);			// 4/5
	SX1276_SetPreambleLength(8);

	// Set base address
	SX1276_WriteRegister(REG_FIFO_TX_BASE_ADDR, 0);
	SX1276_WriteRegister(REG_FIFO_RX_BASE_ADDR, 0);

	// Set LNA Boost
	SX1276_WriteRegister(REG_LNA, SX1276_ReadRegister(REG_LNA) | 0x03);

	// set auto AGC
	SX1276_WriteRegister(REG_MODEM_CONFIG_3, 0x04);

	// Set output power
	SX1276_SetTxPower(17);

	// Standby
	SX1276_Standby();
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

void	SX1276_SetTxPower(uint8_t power)
{
    // PA BOOST
    if (power > 17)
    {
    	if (power > 20)
    	{
    		power = 20;
    	}

		// subtract 3 from level, so 18 - 20 maps to 15 - 17
    	power -= 3;

		// High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
		SX1276_WriteRegister(REG_PA_DAC, 0x87);
		SX1276_SetOCP(140);

    }
    else
    {
    	if (power < 2)
    	{
    		power = 2;
    	}
    	//Default value PA_HF/LF or +17dBm
    	SX1276_WriteRegister(REG_PA_DAC, 0x84);
    	SX1276_SetOCP(100);
    }

    SX1276_WriteRegister(REG_PA_CONFIG, PA_BOOST | (power - 2));
}

void	SX1276_SetOCP(uint8_t current)
{
	uint8_t ocpTrim = 27;

	if (current <= 120)
	{
		ocpTrim = (current - 45) / 5;
	}
	else if (current <=240)
	{
		ocpTrim = (current + 30) / 10;
	}

	SX1276_WriteRegister(REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void	SX1276_SetSpreadingFactor(uint8_t sf)
{
	if (sf < 6)
	{
		sf = 6;
	}
	else if (sf > 12)
	{
		sf = 12;
	}

	if (sf == 6)
	{
		SX1276_WriteRegister(REG_DETECTION_OPTIMIZE, 0xc5);
		SX1276_WriteRegister(REG_DETECTION_THRESHOLD, 0x0c);
	}
	else
	{
		SX1276_WriteRegister(REG_DETECTION_OPTIMIZE, 0xc3);
		SX1276_WriteRegister(REG_DETECTION_THRESHOLD, 0x0a);
	}

	SX1276_WriteRegister(REG_MODEM_CONFIG_2, (SX1276_ReadRegister(REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
	SX1276_SetLdoFlag();
}

void 	SX1276_SetSignalBandwidth(uint32_t sbw)
{
  uint8_t bw;

	if (sbw <= 7.8E3)
	{
		bw = 0;
	}
	else if (sbw <= 10.4E3)
	{
		bw = 1;
	}
	else if (sbw <= 15.6E3)
	{
		bw = 2;
	}
	else if (sbw <= 20.8E3)
	{
		bw = 3;
	}
	else if (sbw <= 31.25E3)
	{
		bw = 4;
	}
	else if (sbw <= 41.7E3)
	{
		bw = 5;
	}
	else if (sbw <= 62.5E3)
	{
		bw = 6;
	}
	else if (sbw <= 125E3)
	{
		bw = 7;
	}
	else if (sbw <= 250E3)
	{
		bw = 8;
	}
	else /*if (sbw <= 250E3)*/
	{
		bw = 9;
	}

	SX1276_WriteRegister(REG_MODEM_CONFIG_1, (SX1276_ReadRegister(REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
	SX1276_SetLdoFlag();
}

void 	SX1276_SetCodingRate4(uint8_t denominator)
{
	if (denominator < 5)
	{
		denominator = 5;
	}
	else if (denominator > 8)
	{
		denominator = 8;
	}

	uint8_t cr = denominator - 4;

	SX1276_WriteRegister(REG_MODEM_CONFIG_1, (SX1276_ReadRegister(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void 	SX1276_SetPreambleLength(uint32_t length)
{
	SX1276_WriteRegister(REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
	SX1276_WriteRegister(REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void 	SX1276_SetLdoFlag(void)
{
	// Section 4.1.1.5
	uint32_t symbolDuration = 1000 / ( SX1276_GetSignalBandwidth() / (1L << SX1276_GetSpreadingFactor()) ) ;

	// Section 4.1.1.6
	uint8_t ldoOn = (symbolDuration > 16) & 0x01;

	uint8_t config3 = SX1276_ReadRegister(REG_MODEM_CONFIG_3);
	bitWrite(config3, 3, ldoOn);
	SX1276_WriteRegister(REG_MODEM_CONFIG_3, config3);
}

uint32_t	SX1276_GetSignalBandwidth(void)
{
	uint8_t bw = (SX1276_ReadRegister(REG_MODEM_CONFIG_1) >> 4);

	switch (bw)
	{
		case 0: return 7.8E3;
		case 1: return 10.4E3;
		case 2: return 15.6E3;
		case 3: return 20.8E3;
		case 4: return 31.25E3;
		case 5: return 41.7E3;
		case 6: return 62.5E3;
		case 7: return 125E3;
		case 8: return 250E3;
		case 9: return 500E3;
	}

	return -1;
}

uint8_t	SX1276_GetSpreadingFactor(void)
{
	return SX1276_ReadRegister(REG_MODEM_CONFIG_2) >> 4;
}

uint8_t	SX1276_StartPacket(uint8_t implicitHeader)
{
	if (SX1276_IsTransmitting())
	{
		return 0;
	}

	// put in standby mode
	SX1276_Standby();

	if (implicitHeader)
	{
		SX1276_SetImplicitHeader();
	}
	else
	{
		SX1176_SetExplicitHeader();
	}

	// reset FIFO address and paload length
	SX1276_WriteRegister(REG_FIFO_ADDR_PTR, 0);
	SX1276_WriteRegister(REG_PAYLOAD_LENGTH, 0);

	return 1;
}

uint8_t SX1276_FinishPacket(void)
{

//	if ((async) && (_onTxDone))
//	{
//		writeRegister(REG_DIO_MAPPING_1, 0x40); // DIO0 => TXDONE
//	}


	// put in TX mode
	SX1276_WriteRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

//	if (!async)
//	{
		// wait for TX done
//		while ((SX1276_ReadRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
//		{
			HAL_Delay(100); 	//yield();
//		}
		// clear IRQ's
		SX1276_WriteRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
//	}

	return 1;
}

uint8_t	SX1276_IsTransmitting(void)
{
	if ((SX1276_ReadRegister(REG_OP_MODE) & MODE_TX) == MODE_TX)
	{
		return 1;
	}

	if (SX1276_ReadRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
	{
		// clear IRQ's
		SX1276_WriteRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
	}

	return 0;
}

void SX1176_SetExplicitHeader (void)
{
	_implicitHeader = 0;
	SX1276_WriteRegister(REG_MODEM_CONFIG_1, SX1276_ReadRegister(REG_MODEM_CONFIG_1) & 0xFE);
}

void SX1276_SetImplicitHeader(void)
{
	_implicitHeader = 1;
	SX1276_WriteRegister(REG_MODEM_CONFIG_1, SX1276_ReadRegister(REG_MODEM_CONFIG_1) | 0x01);
}

void SX1276_Print(uint8_t * buffer, uint8_t lenght)
{
	  uint8_t currentLength = SX1276_ReadRegister(REG_PAYLOAD_LENGTH);

	  // check size
	  if ((currentLength + lenght) > MAX_PKT_LENGTH)
	  {
		  lenght = MAX_PKT_LENGTH - currentLength;
	  }

	  // write data
	  for (uint8_t i = 0; i < lenght; i++)
	  {
		  SX1276_WriteRegister(REG_FIFO, buffer[i]);
	  }

	  // update length
	  SX1276_WriteRegister(REG_PAYLOAD_LENGTH, currentLength + lenght);
}
