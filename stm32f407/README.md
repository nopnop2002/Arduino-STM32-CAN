# stm32f407
I used [this](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32) as a reference.

# Changes from the original

- frame format   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
In this example, you can select the CAN port.   

- CAN1 and CAN2   
In this example, you can use CAN1 and CAN2 port.   


# Remap CAN port

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
