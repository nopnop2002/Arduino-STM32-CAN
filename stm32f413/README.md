# stm32f413

It also works with the following STM32F4 series.   
- STM32F412
- STM32F413

I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

- CAN1 and CAN2   
In this example, you can use CAN1 and CAN2 port.   


# Remap CAN port
STM32F412/413 has two CAN ports.   
You can use only CAN1 or both CAN1 and CAN2, but you cannot use only CAN2.   

These GPIOs can be used as CAN.   
|CAN|RX|TX||
|:-:|:-:|:-:|:-:|
|CAN1|PA11|PA12||
|CAN1|PB8|PB9||
|CAN1|PD0|PD1||
|CAN1|PG0|PG1|not available on 48/64-pin package|
|CAN2|PB5|PB6||
|CAN2|PB12|PB13||
|CAN2|PG11|PG12||

CAN1_RX mapped to PA11, CAN1_TX mapped to PA12   
CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6   
```
CANInit(CAN_1000KBPS, 0);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

CAN1_RX mapped to PG0 , CAN1_TX mapped to PG1   
CAN2_RX mapped to PG11, CAN2_TX mapped to PG12   
```
CANInit(CAN_1000KBPS, 1);  // CAN_RX mapped to PG0 , CAN_TX mapped to PG1
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
