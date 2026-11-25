# stm32l452

It also works with the following STM32L4 series.   
- STM32L432
- STM32L433
- STM32L452
- STM32L476

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

# Remap CAN port
STM32L432 has one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX|
|:-:|:-:|:-:|
|CAN1|PA11|PA12|

STM32L433/476 has one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1|Not supported|

STM32L452 has one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB5|PB6||
|CAN1|PB8|PB9||
|CAN1|PB12|PB13||
|CAN1|PD0|PD1|Not supported|

CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN_RX mapped to PB5, CAN_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 1);  // CAN_RX mapped to PB5 , CAN_TX mapped to PB6
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN_RX mapped to PB12, CAN_TX mapped to PB13   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PB12, CAN_TX mapped to PB13
```

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32L4xx/

```
L432K(B-C)U_L442KCU\variant_generic.h(119):   #define PIN_SERIAL_TX         PA2
L432K(B-C)U_L442KCU\variant_NUCLEO_L432KC.h(95):   #define PIN_SERIAL_TX           PA2_ALT1
L433C(B-C)(T-U)_L443CC(T-U)\variant_CYGNET.h(194):   #define PIN_SERIAL_TX           PA9
L433C(B-C)(T-U)_L443CC(T-U)\variant_generic.h(136):   #define PIN_SERIAL_TX         PA2
L433C(B-C)Y_L443CC(F-Y)\variant_generic.h(137):   #define PIN_SERIAL_TX         PA2
L433R(B-C)(I-T-Y)_L443RC(I-T-Y)\variant_generic.h(150):   #define PIN_SERIAL_TX         PA2
L433RCTxP\variant_generic.h(148):   #define PIN_SERIAL_TX         PA2
L433RCTxP\variant_NUCLEO_L433RC_P.h(125):   #define PIN_SERIAL_TX           PA2
L433VC(I-T)_L443VC(I-T)\variant_generic.h(181):   #define PIN_SERIAL_TX         PA2
L452CCU_L452CE(T-U)x(P)_L462CE(T-U)\variant_generic.h(142):   #define PIN_SERIAL_TX         PA0
L452RC(I-T-Y)_L452RE(I-T-Y)x(P)_L462RE(I-T-Y)\variant_generic.h(160):   #define PIN_SERIAL_TX         PA0
L452RC(I-T-Y)_L452RE(I-T-Y)x(P)_L462RE(I-T-Y)\variant_LEAFONY_AP03.h(121):   #define PIN_SERIAL_TX         PA2
L452RC(I-T-Y)_L452RE(I-T-Y)x(P)_L462RE(I-T-Y)\variant_NUCLEO_L452RE.h(133):   #define PIN_SERIAL_TX           PA2
L452RETxP\variant_generic.h(158):   #define PIN_SERIAL_TX         PA0
L452RETxP\variant_NUCLEO_L452RE_P.h(132): #define PIN_SERIAL_TX           PA2
L452V(C-E)(I-T)_L462VE(I-T)\variant_generic.h(191):   #define PIN_SERIAL_TX         PA0
L476J(E-G)Y_L485J(C-E)Y_L486JGY\variant_generic.h(188):   #define PIN_SERIAL_TX         PA0
L476JGYxP\variant_generic.h(185):   #define PIN_SERIAL_TX         PA0
L476M(E-G)Y\variant_generic.h(196):   #define PIN_SERIAL_TX         PA0
L476QEI_L476QGIx(P)_L486QGI\variant_generic.h(240):   #define PIN_SERIAL_TX         PA0
L476VGYxP\variant_generic.h(196):   #define PIN_SERIAL_TX         PA0
L476ZET_L476ZG(J-T)_L486ZGT\variant_generic.h(246):   #define PIN_SERIAL_TX         PA0
L476ZGTxP\variant_generic.h(244):   #define PIN_SERIAL_TX         PA0
```
