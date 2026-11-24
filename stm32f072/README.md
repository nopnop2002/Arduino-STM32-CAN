# stm32f072
Any 48MHz STM32F0 series will work.   
- STM32F042
- STM32F072
- STM32F091
- STM32F098

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

# Remap CAN port

STM32F042 have one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX|
|:-:|:-:|:-:|
|CAN1|PA11|PA12|
|CAN1|PB8|PB9|


STM32F072/091/098 have one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX|
|:-:|:-:|:-:|
|CAN1|PA11|PA12|
|CAN1|PB8|PB9|
|CAN1|PD0|PD1|


CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 100-pin and 144-pin package)   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
```

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F0xx/

```
F072C8(T-U)_F072CB(T-U-Y)\variant_ELEKTOR_F072Cx.h(100): #define PIN_SERIAL_TX           PA9
F072C8(T-U)_F072CB(T-U-Y)\variant_generic.h(135):   #define PIN_SERIAL_TX         PA0
F072R8T_F072RB(H-I-T)\variant_DISCO_F072RB.h(173):   #define PIN_SERIAL_TX         PA9
F072R8T_F072RB(H-I-T)\variant_generic.h(151):   #define PIN_SERIAL_TX         PA0
F072R8T_F072RB(H-I-T)\variant_NUCLEO_F072RB.h(126):   #define PIN_SERIAL_TX         PA2
F072R8T_F072RB(H-I-T)\variant_PYBSTICK26_DUINO.h(127): #define PIN_SERIAL_TX           PA2
F072V(8-B)(H-T)\variant_generic.h(187):   #define PIN_SERIAL_TX         PA0
```
