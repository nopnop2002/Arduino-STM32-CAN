# stm32f767

It also works with the following STM32F7 series.   
- STM32F746
- STM32F756
- STM32F767

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

- CAN1 and CAN2   
In this example, you can use CAN1 and CAN2 port.   

# CAN Port   
STM32F746/756 has two CAN ports.   
STM32F767 has three CAN ports.   
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
 Transceiver          |           |
+-----------+         |           |
|        CRX|---------|CAN2       |
|        CTX|---------|CAN2       |
+-----------+         +-----------+
```

# Remap CAN port
STM32F746/756 has two CAN ports.   
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


STM32F767 has three CAN ports.   
You can use only CAN1 or both CAN1 and CAN2, but you cannot use only CAN2.   

These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1||
|CAN1|PH13|PH14|Not supported|
|CAN2|PB5|PB6||
|CAN2|PB12|PB13||
|CAN3|PA8|PA15|Not supported|
|CAN3|PB3|PB4|Not supported|

CAN1_RX mapped to PA11, CAN1_TX mapped to PA12   
CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN1_RX mapped to PB8 , CAN1_TX mapped to PB9   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN1_RX mapped to PD0 , CAN1_TX mapped to PD1   
CAN2_RX mapped to PB12, CAN2_TX mapped to PB13   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PD0, CAN_TX mapped to PD1
```
