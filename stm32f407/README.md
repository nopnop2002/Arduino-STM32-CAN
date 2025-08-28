# stm32f407   
![F407](https://user-images.githubusercontent.com/6020549/170389354-ff0673ff-2ab7-4774-ad3d-b03744b33f50.JPG)

Any 168MHz STM32F4 series will work.   
- STM32F405
- STM32F407
- STM32F415
- STM32F417

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

- CAN1 and CAN2   
In this example, you can use CAN1 and CAN2 port.   

# CAN Port   
STM32F407 has two CAN ports.   
You specify which port to use.   
It is possible to use multiple ports simultaneously, but this example does not support this.   
```
#define CAN_PORT CAN1 // Using CAN1
//#define CAN_PORT CAN2 // Using CAN2
```

CAN2 is a slave port of CAN1.   
If you use the CAN2 port, you need a transceiver for both CAN1 and CAN2.   
```
 Transceiver              STM32
+-----------+         +-----------+ 
|        CRX|---------|CAN1       |
|        CTX|---------|CAN1       |
+-----------+         |           |
                      |           |
 Trancseiver          |           |
+-----------+         |           |
|        CRX|---------|CAN2       |
|        CTX|---------|CAN2       |
+-----------+         +-----------+
```

# Remap CAN port   

STM32F405/407/415/417 have two CAN ports.   
You can use only CAN1 or both CAN1 and CAN2, but you cannot use only CAN2.   

These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1||
|CAN1|PI9|PH13|Not supported|
|CAN2|PB5|PB6||
|CAN2|PB12|PB13||

CAN1_RX mapped to PA11, CAN1_TX mapped to PA12   
CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN1_RX mapped to PB8 , CAN1_TX mapped to PB9 (not available on 36-pin package)   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13(not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN1_RX mapped to PD0 , CAN1_TX mapped to PD1 (available on 100-pin and 144-pin package)   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13(not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
```

# Serial Output with Virtual COM port   
STM32F407 can use a Virtual COM port.   
If you enable Virtual COM port, you can use the USB port as a serial port.   
Basic Extended CAN(BxCan) and Virtual COM port can be used at the same time.   
__When using the USB port, do not supply power from the ST-LINK module.__   

# Serial Output without Virtual COM port   
The output destination of Serial.print differs depending on the Board Part Number.   
This is a sample of some boards.    
- genericSTM32F407xxxx:PA0   
- diymore_f407vgt:PA9   
- black_f407xx:PA9   
- disco_f407vg:PA2   

You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F4xx/


# Using PlatformIO   
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f407
pio init --board genericSTM32F407VET6
cp _platformio.ini platformio.ini
cp stm32f407.ino src/

# Without Virtual COM port
pio run -t upload -e genericSTM32F407VET6

# With Virtual COM port
pio run -t upload -e genericSTM32F407VET6_usbcon
```
