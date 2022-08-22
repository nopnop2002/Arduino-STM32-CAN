# stm32f405   
Source code is the same as stm32f407.   
I used this board.   
![F405](https://user-images.githubusercontent.com/6020549/170388891-1d26c489-5fa2-4bf5-81c3-ff4a442368b8.JPG)

# Remap CAN port
This board don't have PA11/PA12.   
You definitely need to remap.   
CAN1_RX mapped to PB8 , CAN1_TX mapped to PB9 (not available on 36-pin package)   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13(not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```


# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
- genericSTM32F405RG:PA0   

You need to check PeripheralPins.c for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F4xx/


# Using PlatformIO   
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f407
pio init --board genericSTM32F405RG
vi platform.ini
---------------------------------------
[env:genericSTM32F405RG]
platform = ststm32
board = genericSTM32F405RG
framework = arduino
upload_protocol = stlink ---> Add this
---------------------------------------
cp stm32f407.ino src/
pio run -t upload
```
