# stm32f103
I forked from [here](https://github.com/seeers/CAN-Bus-Arduino_Core_STM32).

# Changes from the original

- frame format   
The original support only Base frame format: with 11 identifier bits.   
This example support Extended frame format: with 29 identifier bits.   

- CAN port   
The original uses PB8 and PB9 as CAN ports.   
In this example, you can select the CAN port.   

- CAN Timing   
The original uses a fixed value regardless of clock frequency.   
In this example, we calculate the timing depending on the clock frequency.   

 NOTE:   
 Arduino core support for STM32 based boards uses different clock frequencies depending on the Board Part Number.   
 BlackPill & BluePill use 72Mhz.   
 Generic board use use 64Mhz.   

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


# Using PlatformIO   

## Blue Pill
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f103
pio init --board bluepill_f103c8
cp stm32f103.ino src/
pio run -t upload
```

## Black Pill
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f103
pio init --board blaclpill_f103c8
cp stm32f103.ino src/
pio run -t upload
```

## Generic board
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f103
pio init --board genericSTM32F103C8
cp stm32f103.ino src/
pio run -t upload
```
