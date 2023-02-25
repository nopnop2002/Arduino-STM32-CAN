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
Generic board use 64Mhz instead of 72Mhz.   
So the original doesn't work properly on the Generic board.   

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
But Basic Extended CAN(BxCan) and Virtual COM port cannot be used at the same time.   
__Don't use USB Support.__   
If you want to use a Serial object, you need to use a USB-UART conversion module.   
Note:   
For STM32F405/407, BxCan and Virtual COM port can be used at the same time.   
The reason the F103 chip cannot use BxCan and Virtual COM port at the same time is because they share memory buffers.   

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   

- BluePill:PA9   
- BlackPill:PA9   
- Generic F103:PA2   

You need to check PeripheralPins.c for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32F1xx/


# Using PlatformIO   
Arduino-IDE only supports ST-LINK V2.1 adapters, but OpenOCD used by PlatformIO supports both V2.0 and V2.1.   
With ST-LINK, there is no need to change boot mode when writing firmware.   
PlatformIO allows you to use cheap Chinese ST-LINK adapters like this one.   
You can get it at a low price (about $2).   
![ST-LINK-1](https://user-images.githubusercontent.com/6020549/221345715-b86e0a93-bdf4-46dd-a9b9-c05a27b042a4.JPG)
![ST-LINK-2](https://user-images.githubusercontent.com/6020549/221345711-7749b557-d55f-442f-8390-3632c63d5239.JPG)

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
pio init --board blackpill_f103c8
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

# Clone chip
Doesn't work properly with clone chips.   
https://hackaday.com/2020/10/22/stm32-clones-the-good-the-bad-and-the-ugly/

