# stm32f103
I forked from [here](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32).

# Changes from the original

- frame format   
The original support only Base frame format: with 11 identifier bits.   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
The original uses PB8 and PB9 as CAN ports.   
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

# Using Virtual COM port   
STM32F103 can use a Virtual COM port.   
If you enable Virtual COM port, you can use the USB port as a serial port.   
But Basic Extended CAN(BxCan) and USB Support cannot be used at the same time.   
__Don't use USB Support.__   
If you want to use a Serial object, you need to use a USB-UART conversion module.   


# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   

- BluePill:PA9   
- BlackPill:PA9   
- Generic F103:PA2   

You need to check PeripheralPins.c for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F1xx/
