# stm32f405   
Source code is the same as stm32f407.   
I used this board.   
This board is called a pyboard.   
Pin out is [here](https://www.pianshen.com/article/8473671420/).   
GPIO is irregular, but you can use it in the Arduino environment.   
![F405](https://user-images.githubusercontent.com/6020549/170388891-1d26c489-5fa2-4bf5-81c3-ff4a442368b8.JPG)

# Remap CAN port
This board does not have PA11/PA12 pin headers.   
You definitely need to remap.   
CAN1_RX mapped to PB8 , CAN1_TX mapped to PB9 (not available on 36-pin package)   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13(not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

# Serial Output with Virtual COM port   
STM32F405 can use a Virtual COM port.   
If you enable Virtual COM port, you can use the USB port as a serial port.   
Basic Extended CAN(BxCan) and Virtual COM port can be used at the same time.   

USB port can be used for power supply.   
__Do not supply power from both the USB port and the ST-LINK module.__   

# Serial Output using GPIO   
The output destination of Serial.print differs depending on the Board Part Number.   
- genericSTM32F405RG:PA0   

You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F4xx/


# Using PlatformIO   
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f407
pio init --board genericSTM32F405RG
cp _platformio.ini platformio.ini
cp stm32f407.ino src/

# Without Virtual COM port
pio run -t upload -e genericSTM32F405RG

# With Virtual COM port
pio run -t upload -e genericSTM32F405RG_usbcon
```
