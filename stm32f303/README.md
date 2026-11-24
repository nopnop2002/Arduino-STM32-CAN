# stm32f303
![F303](https://user-images.githubusercontent.com/6020549/170388138-d9a89538-34fc-41db-b859-37478162d4df.JPG)

Any 72MHz STM32F3 series will work.   
- STM32F302
- STM32F303
- STM32F328
- STM32F334
- STM32F358
- STM32F373
- STM32F378
- STM32F398

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

# Remap CAN port

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
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F3xx/

```
F303C(6-8)T_F334C(4-6-8)T\variant_generic.h(140):   #define PIN_SERIAL_TX         PA2
F303C(B-C)T\variant_BLACKPILL_F303CC.h(149):   #define PIN_SERIAL_TX         PA9
F303C(B-C)T\variant_generic.h(158):   #define PIN_SERIAL_TX         PA2
F303C(B-C)T\variant_SPARKY_F303CC.h(150):   #define PIN_SERIAL_TX         PB10
F303C8Y_F334C8Y\variant_generic.h(142):   #define PIN_SERIAL_TX         PA2
F303K(6-8)T_F334K(4-6-8)T\variant_generic.h(125):   #define PIN_SERIAL_TX         PA2
F303K(6-8)T_F334K(4-6-8)T\variant_NUCLEO_F303K8.h(102):   #define PIN_SERIAL_TX         PA2
F303K(6-8)U_F334K(4-6-8)U\variant_generic.h(123):   #define PIN_SERIAL_TX         PA2
F303R(6-8)T_F334R(6-8)T\variant_generic.h(158):   #define PIN_SERIAL_TX         PA2
F303R(6-8)T_F334R(6-8)T\variant_NUCLEO_F334R8.h(141):   #define PIN_SERIAL_TX         PA2
F303R(B-C)T\variant_generic.h(183):   #define PIN_SERIAL_TX         PA2
F303R(B-C)T\variant_OLIMEXINO_STM32F3.h(213):   #define PIN_SERIAL_TX         PA2
F303R(D-E)T\variant_generic.h(183):   #define PIN_SERIAL_TX         PA2
F303R(D-E)T\variant_NUCLEO_F303RE.h(159):   #define PIN_SERIAL_TX         PA2
F303V(B-C)T\variant_DISCO_F303VC.h(226):   #define PIN_SERIAL_TX         PC4
F303V(B-C)T\variant_generic.h(225):   #define PIN_SERIAL_TX         PA2
F303V(D-E)(H-T)\variant_generic.h(230):   #define PIN_SERIAL_TX         PA2
F303VCY\variant_generic.h(216):   #define PIN_SERIAL_TX         PA2
F303VEY\variant_generic.h(222):   #define PIN_SERIAL_TX         PA2
F303Z(D-E)T\variant_generic.h(259):   #define PIN_SERIAL_TX         PA2
```
