# stm32f405   
I used this board.   
Source code is the same as stm32f407.   
![F405](https://user-images.githubusercontent.com/6020549/170388891-1d26c489-5fa2-4bf5-81c3-ff4a442368b8.JPG)

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
