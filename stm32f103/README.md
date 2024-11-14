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

You need to check variant_generic.h for your board.    
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

# STM3232F103 with built-in CAN transceiver   
This board can be connected directly to CAN-BUS.   
![STM32VET6-1](https://github.com/user-attachments/assets/8464af16-9563-45c1-aa5a-88cb2b194cc8)
![STM32VET6-3](https://github.com/user-attachments/assets/c66dde18-3f7c-43a2-81b5-f98aec9247ae)

# Clone chip
Doesn't work properly with clone chips.   
https://hackaday.com/2020/10/22/stm32-clones-the-good-the-bad-and-the-ugly/    

[This](https://mecrisp-stellaris-folkdoc.sourceforge.io/bluepill-diagnostics-v1.6.html) is useful for checking if it is a clone or not.   
According to this page, the following clones are currently confirmed:   
- CKS32F103C8T6   
- CKS32F103CBT6   
- APM32F103CBT6   
- GD32F103VE   
- GD32F103VK   
- GD32F103C8T6   

