# LoRa communication: WL55JC1 and E32-900T20D

This small project is an atemp to test LoRa communication between a [Nucleo WL55JC1](https://www.st.com/en/evaluation-tools/nucleo-wl55jc.html) 
([STM32WL55JC](https://www.st.com/en/microcontrollers-microprocessors/stm32wl55jc.html) from ST Microelectronics) and an 
EBYTE [E32-900T20D](https://www.cdebyte.com/products/E32-900T20D/1) LoRa/UART module 
([SX1276](https://www.semtech.com/products/wireless-rf/lora-connect/sx1276) from Semtech).  

  (img)
  
The Nucleo board is running a modified example from ST's repository that can be found on CubeMX, 
named "[SubGHz_Phy_PingPong](https://github.com/STMicroelectronics/STM32CubeWL/tree/7950099d0d2502f55ac8e189270119800af4dd55/Projects/NUCLEO-WL55JC/Applications/SubGHz_Phy/SubGHz_Phy_PingPong)".  
The changes, necessary to match the configuration of the LoRa transceivers, were:  
- @file    subghz_phy_app.h:
```
#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
#define LORA_BANDWIDTH                2        /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved] */
#define LORA_SPREADING_FACTOR        11        /* [SF7..SF12] */
#define LORA_CODINGRATE               1        /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_PREAMBLE_LENGTH          8        /* Same for Tx and Rx */
#define LORA_SYMBOL_TIMEOUT           5        /* Symbols */
#define LORA_FIX_LENGTH_PAYLOAD_ON    false
#define LORA_IQ_INVERSION_ON          false
```

- @file    sys_conf.h:  
```
#define VERBOSE_LEVEL                 VLEVEL_H
#define DEBUGGER_ENABLED              1
#define LOW_POWER_DISABLE             1
```
  
The E32 module is configured as follows:  
  
![E32 setting](./img/ebyte_setting.png)

Therefore, the LoRa configuration in this test is: 
- Frequency:         868 MHz
- Bandwidth:         500 kHz
- Spreading factor:  SF11
- Coding rate:       4/5  
    
The E32 module is then connected to the PC via a UART/USB converter and data can be sent manually using a terminal application 
(in this case, I'm using [Termite](https://www.compuphase.com/software_termite.htm) for the E32 - **COM7**).  
  
![E32 connected via Termite](./img/termite.png) 
  
The Nucleo board is also connected to the PC and the ST-Link also includes a COM port connected to the MCU's UART. 
With modifications shown before, 
running the software on the Nucleo board and connecting a terminal to the UART 
(in this case, [CoolTerm](https://freeware.the-meiers.org/) - **COM6**), 
it is possible to see when messeges from the E32 module are received by the STM32. 
To improve reception from the STM32, 
the message from the E32 should be sent as soon as the STM32 starts its RX cycle 
(the following message is sent from STM32 via UART: "*elapsed time*:Rx start").  
  
![STM32 connected via CollTerm](./img/coolterm.png) 

However, as highlighted on the image above, 
data sent from the E32 module is encrypted and packaged with more information 
regarding the lenght, channel, address and some type of checksum/CRC calculation.  
I didn't find informatio about this encryption on the module's dataheet, 
but after some research I've found some interesting clues of how data is treated.  
[This discussion](https://github.com/sandeepmistry/arduino-LoRa/issues/203) is very helpful and also led me to 
[this blog post](https://www.sanglierlab.fr/2023/02/11/communiquer-entre-un-module-ebyte-e32-et-un-module-ra01-02-sx1278/).
