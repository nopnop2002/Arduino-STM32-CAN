# stm32l496

It also works with the following STM32L4 series.   
- STM32L496
- STM32L4A6

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

# Remap CAN port

STM32L496/4A6 has two CAN ports.   
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


CAN1_RX mapped to PA11, CAN_TX mapped to PA12   
CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN1_RX mapped to PB8, CAN_TX mapped to PB9   
CAN2_RX mapped to PB12, CAN_TX mapped to PB13   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN1_RX mapped to PD0 , CAN1_TX mapped to PD1   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
```

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32L4xx/

```
L496A(E-G)I_L4A6AGI\variant_generic.h(269):   #define PIN_SERIAL_TX         PA0
L496AGIxP_L4A6AGIxP\variant_generic.h(267):   #define PIN_SERIAL_TX         PA0
L496QEI_L496QGIx(S)_L4A6QGI\variant_generic.h(243):   #define PIN_SERIAL_TX         PA0
L496QGIxP_L4A6QGIxP\variant_generic.h(240):   #define PIN_SERIAL_TX         PA0
L496R(E-G)T_L4A6RGT\variant_generic.h(185):   #define PIN_SERIAL_TX         PA0
L496RGTxP\variant_generic.h(182):   #define PIN_SERIAL_TX         PA0
L496V(E-G)T_L4A6VGT\variant_generic.h(216):   #define PIN_SERIAL_TX         PA0
L496VGTxP_L4A6VGTxP\variant_generic.h(213):   #define PIN_SERIAL_TX         PA0
L496VGYxP_L4A6VGYxP\variant_generic.h(212):   #define PIN_SERIAL_TX         PA0
L496VGY_L4A6VGY\variant_generic.h(214):   #define PIN_SERIAL_TX         PA0
L496WGYxP\variant_generic.h(220):   #define PIN_SERIAL_TX         PA0
L496Z(E-G)T_L4A6ZGT\variant_generic.h(249):   #define PIN_SERIAL_TX         PA0
L496Z(E-G)T_L4A6ZGT\variant_NUCLEO_L496ZG.h(239):   #define PIN_SERIAL_TX           PG7
L496ZGTxP_L4A6ZGTxP\variant_generic.h(246):   #define PIN_SERIAL_TX         PA0
L496ZGTxP_L4A6ZGTxP\variant_NUCLEO_L496ZG_P.h(237):   #define PIN_SERIAL_TX           PG7
```
