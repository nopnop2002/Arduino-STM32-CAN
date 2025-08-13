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

# Using receive filters   
STM32F103 has four types of reception filters.   
![Image](https://github.com/user-attachments/assets/adc56930-8681-4e57-b102-c8a9ac1105f0)   

When setting a mask, always manipulate the FMR register.   
```
	uint32_t bank1, bank2;
	CAN1->FMR  |=   0x1UL;                // Set to filter initialization mode
	bank1 = your settings
	bank2 = your settings
	CANSetFilter(0, 0, 1, 0, bank1, bank2);
	CAN1->FMR   &= ~(0x1UL);              // Deactivate initialization mode
```

- One 32-Bit Filter - Identifier Mask   
	This can be used for both standard and extended IDs.   
	```
	  // One 32-Bit Filter - Identifier Mask
	  bank1 = 0x101 << 21;
	  bank2 = 0xFFE00006; // Must be IDE=0 RTR=0  
	  CANSetFilter(0, 1, 0, 0, bank1, bank2); // use 1 filter(filter number 0)

	  // One 32-Bit Filter - Identifier Mask
	  bank1 = 0x101 << 3;
	  bank1 = bank1 + 0x04; // Ext
	  bank2 = 0xFFFFFFFE; // Must be IDE=1 RTR=0
	  CANSetFilter(1, 1, 0, 0, bank1, bank2); // use 1 filter(filter number 1)
	```
	![Image](https://github.com/user-attachments/assets/c2b10523-5eb6-4ce3-85e3-026becf89c34)

- Two 32-Bit Filters - Identifier List   
	This can be used for both standard and extended IDs.   
	Note that this use two filters.   
	```
	  // Two 32-Bit Filters - Identifier List
	  bank1 = 0x102 << 21;
	  bank2 = 0x103 << 21;
	  CANSetFilter(0, 1, 1, 0, bank1, bank2); // use 2 filter(filter number 0-1)

	  bank1 = 0x102 << 3;
	  bank1 = bank1 + 0x04; // Ext
	  bank2 = 0x103 << 3;
	  bank2 = bank2 + 0x04; // Ext
	  CANSetFilter(2, 1, 1, 0, bank1, bank2); // use 2 filter(filter number 2-3)
	```
	![Image](https://github.com/user-attachments/assets/81ca9205-d0cb-4e5f-87c7-c7a69f850827)

- Two 16-Bit Filters - Identifier Mask   
	This can only be used with standard IDs.   
	Note that this use two filters.   
	```
	  // Two 16-Bit Filters - Identifier Mask
	  bank1 = 0x104 << 21;
	  bank1 = bank1 | 0xFFFF;
	  bank2 = 0x105 << 21;
	  bank2 = bank2 | 0xFFFF;
	  CANSetFilter(0, 0, 1, 0, bank1, bank2); // use 2 filter(filter number 0-1)
	```
	![Image](https://github.com/user-attachments/assets/b2dfa2bb-659f-480c-af69-895c844a93c3)

- Four 16-Bit Filters - Identifier List   
	This can only be used with standard IDs.   
	Note that this use four filters.   
	```
	  // Four 16-Bit Filters - Identifier List
	  bank1 = 0x104 << 21;
	  bank1 = bank1 | 0x105 << 5;
	  bank2 = 0x106 << 21;
	  bank2 = bank2 | 0x107 << 5;
	  CANSetFilter(0, 0, 1, 0, bank1, bank2); // use 4 filter(filter number 0-3)
	```
	![Image](https://github.com/user-attachments/assets/dc1022da-f9e0-4c32-8e08-c49d240e74bd)

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

# STM32F103 with built-in CAN transceiver   
This board can be connected directly to CAN-BUS.   
CAN_RX mapped to PB8, CAN_TX mapped to PB9.   
This requires a 5V power supply as it has a 5V transceiver mounted on it.   
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

