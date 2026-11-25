# stm32f722

It also works with the following STM32F7 series.   
- STM32F722
- STM32F723

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   


# Remap CAN port
STM32F722/723 has one CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1||
|CAN1|PH13|PH14|Not supported|

CAN1_RX mapped to PA11, CAN1_TX mapped to PA12   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN1_RX mapped to PB8 , CAN1_TX mapped to PB9   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN1_RX mapped to PD0 , CAN1_TX mapped to PD1   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
```

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F7xx/

```
F722I(C-E)(K-T)_F732IE(K-T)\variant_generic.h(269):   #define PIN_SERIAL_TX         PA0
F722R(C-E)T_F730R8T_F732RET\variant_generic.h(178):   #define PIN_SERIAL_TX         PA0
F722V(C-E)T_F730V8T_F732VET\variant_generic.h(211):   #define PIN_SERIAL_TX         PA0
F722Z(C-E)T_F732ZET\variant_generic.h(243):   #define PIN_SERIAL_TX         PA0
F722Z(C-E)T_F732ZET\variant_NUCLEO_F722ZE.h(220):   #define PIN_SERIAL_TX         PD8
F723I(C-E)(K-T)_F730I8K_F733IE(K-T)\variant_DISCO_F723IE.h(264):   #define PIN_SERIAL_TX         PC6
F723I(C-E)(K-T)_F730I8K_F733IE(K-T)\variant_generic.h(263):   #define PIN_SERIAL_TX         PA0
F723R(C-E)V\variant_generic.h(175):   #define PIN_SERIAL_TX         PA0
F723V(C-E)(T-Y)_F733VE(T-Y)\variant_generic.h(204):   #define PIN_SERIAL_TX         PA0
F723Z(C-E)(I-T)_F730Z8T_F733ZE(I-T)\variant_generic.h(237):   #define PIN_SERIAL_TX         PA0
```
