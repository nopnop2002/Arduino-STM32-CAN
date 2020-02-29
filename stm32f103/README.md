# stm32f103
I forked from [here](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32).

# Changes from the original
The original support only Base frame format: with 11 identifier bits.   
This example support Extended frame format: with 29 identifier bits.   

In this example, you can select the CAN port.   

No remap.   
```
CANInit(CAN_1000KBPS, false);  // CAN_RX mapped to PA11, CAN_TX mapped to PA12
```

With remap.   
```
CANInit(CAN_1000KBPS, true);  // CAN_RX mapped to PB8, CAN_TX mapped to PB9
```
