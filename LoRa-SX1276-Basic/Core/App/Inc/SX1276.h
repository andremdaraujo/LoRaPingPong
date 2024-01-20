/*
 * SX1276.h
 *
 *  Created on: Jan 19, 2024
 *      Author: Eng. André A. M. Araújo
 */

// Based on: https://github.com/sandeepmistry/arduino-LoRa
// https://www.electroniclinic.com/lora-sx1278-arduino-hello-world-sensor-monitoring-projects/

//#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
//#define LORA_BANDWIDTH                              2         /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
//#define LORA_SPREADING_FACTOR                       11        /* [SF7..SF12] */
//#define LORA_CODINGRATE                             1         /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
//#define LORA_PREAMBLE_LENGTH                        8         /* Same for Tx and Rx */
//#define LORA_SYMBOL_TIMEOUT                         5         /* Symbols */
//#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
//#define LORA_IQ_INVERSION_ON                        false

#ifndef APP_INC_SX1276_H_
#define APP_INC_SX1276_H_

#include "main.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define SPI_TIMEOUT 100

#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_RSSI_VALUE           0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06
#define MODE_CAD                 0x07

// PA config
#define PA_BOOST                 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40
#define IRQ_CAD_DONE_MASK          0x04
#define IRQ_CAD_DETECTED_MASK      0x01

#define RF_MID_BAND_THRESHOLD    525E6
#define RSSI_OFFSET_HF_PORT      157
#define RSSI_OFFSET_LF_PORT      164

#define MAX_PKT_LENGTH           255

extern 	SPI_HandleTypeDef hspi1;

void 	SX1267_Init(void);

uint8_t	SX1276_ReadRegister (uint8_t address);
void 	SX1276_WriteRegister(uint8_t address, uint8_t data);

uint8_t SX1276_GetVersion(void);

void 	SX1276_Sleep(void);
void 	SX1276_Standby(void);
void	SX1276_SetFrequency(uint32_t frequency);
void	SX1276_SetTxPower(uint8_t power);
void	SX1276_SetOCP(uint8_t current);

void	SX1276_SetSpreadingFactor(uint8_t sf);
void 	SX1276_SetSignalBandwidth(uint32_t sbw);
void 	SX1276_SetCodingRate4(uint8_t denominator);
void 	SX1276_SetPreambleLength(uint32_t length);

void 	SX1276_SetLdoFlag(void);

uint32_t	SX1276_GetSignalBandwidth(void);
uint8_t 	SX1276_GetSpreadingFactor(void);

uint8_t	SX1276_StartPacket(uint8_t implicitHeader);
uint8_t SX1276_FinishPacket(void);

uint8_t	SX1276_IsTransmitting(void);

void 	SX1176_SetExplicitHeader (void);
void 	SX1276_SetImplicitHeader(void);

void 	SX1276_Print(uint8_t * buffer, uint8_t lenght);

#endif /* APP_INC_SX1276_H_ */
