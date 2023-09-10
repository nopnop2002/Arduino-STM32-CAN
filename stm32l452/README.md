# stm32l452

It also works with the following STM32L4 series.   
- STM32L432
- STM32L433
- STM32L452
- STM32L476
- STM32L496
- STM32L4A6
- STM32L496
- STM32L4A6

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

STM32L496/4A6 has two CAN ports.   
These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1|Not supported|
|CAN1|PI9|PH13|Not supported|
|CAN2|PB5|PB6|Not supported|
|CAN2|PB12|PB13|Not supported|


CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN_RX mapped to PB5, CAN_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 1);  // CAN_RX mapped to PB5 , CAN_TX mapped to PB6
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)   
```
CANInit(CAN_1000KBPS, 2);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```

CAN_RX mapped to PB12, CAN_TX mapped to PB13 (available on 100-pin and 144-pin package)   
```
CANInit(CAN_1000KBPS, 3);  // CAN_RX mapped to PB12, CAN_TX mapped to PB13
```
