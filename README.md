# LoRa communication between WL55JC1 and E32-900T20D

This small project is an atemp to test LoRa communication between a [Nucleo WL55JC1](https://www.st.com/en/evaluation-tools/nucleo-wl55jc.html) 
([STM32WL55JC](https://www.st.com/en/microcontrollers-microprocessors/stm32wl55jc.html) from ST Microelectronics) and an 
EBYTE [E32-900T20D](https://www.cdebyte.com/products/E32-900T20D/1) LoRa/UART module 
([SX1276](https://www.semtech.com/products/wireless-rf/lora-connect/sx1276) from Semtech).  

The Nucleo board is running a modified example from ST's repository that can be found on CubeMX, named "SubGHz_Phy_PingPong".  
The changes, necessary to match the configuration of the LoRa transceivers, were:  
- @file    subghz_phy_app.h:
```
#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
#define LORA_BANDWIDTH                       2         /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_SPREADING_FACTOR               11         /* [SF7..SF12] */
#define LORA_CODINGRATE                      1         /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_PREAMBLE_LENGTH                 8         /* Same for Tx and Rx */
#define LORA_SYMBOL_TIMEOUT                  5         /* Symbols */
#define LORA_FIX_LENGTH_PAYLOAD_ON           false
#define LORA_IQ_INVERSION_ON                 false
```

- @file    sys_conf.h:  
```
#define VERBOSE_LEVEL                        VLEVEL_H
#define DEBUGGER_ENABLED                     1
#define LOW_POWER_DISABLE                    1
```
  
The E32 module is configured as follows:  
(img)  
  
The module is then connected to the PC via a UART/USB converter and data can be sent manually using a terminal application (in this case, I'm using [Termite](https://www.compuphase.com/software_termite.htm) for the E32).  
(img)  
  
The Nucleo board is also connected to the PC and the ST-Link also includes a COM port connected to the MCU's UART. 
With modifications shown before, running the software on the Nucleo board and connecting a terminal to the UART (in this case, [CoolTerm](https://freeware.the-meiers.org/)), 
it is possible to see when messeges from the E32 module are received by the STM32. 
To improve reception from the STM32, the message from the E32 should be sent as soon as the STM32 starts its RX cycle (the following message is sent from STM32 via UART: "*elapsed time*:Rx start").  
  
(img)  

  
