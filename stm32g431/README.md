# stm32g431
![Image](https://github.com/user-attachments/assets/20c4fe8d-da96-4db9-ac8a-a7072161d242)

It also works with the following STM32G4 series.   
- STM32G431   
- STM32G441   

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).   

# Changes from the original

- Changed to local library   
	The original use global library.   
	This example use local library.   

- Generic STM32G431 Development board   
	The original support only NUCLEO-G431KB.   
	This example support Generic STM32G431 Development board.   

- FDCAN port   
	The original support PA11/PA12 and PB8/PB9.   
	This example support PA11/PA12,PB8/PB9 and PD0/PD1.   

# Remap FDCAN port

CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
  settings.mTxPin = PA_12;
  settings.mRxPin = PA_11;
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 32-pin package)   
```
  settings.mTxPin = PB_9;
  settings.mRxPin = PB_8;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 80-pin and 100-pin package)   
```
  settings.mTxPin = PD_1;
  settings.mRxPin = PD_0;
```

# Using Filter   
Please note that BxCan and CAN-FD have completely different filter settings.   
This example sets the filter as follows:   
```
#define ENABLE_FILTER 0x00 // No filters.
//#define ENABLE_FILTER 0x01 // Use the standard filter. 
//#define ENABLE_FILTER 0x02 // Use the extended filter.
//#define ENABLE_FILTER 0x03 // Use both filter.
```

- No filter   
	![Image](https://github.com/user-attachments/assets/a4d79efc-358d-4851-a2e4-56d156f0fddc)

- Use the standard filter   
	![Image](https://github.com/user-attachments/assets/136bf51e-a72b-4762-9f24-421d5ff6461d)

- Use the extended filter   
	![Image](https://github.com/user-attachments/assets/b8b340e5-5b20-4a5c-b72b-01285b6c2ce8)

- Use both filter   
	![Image](https://github.com/user-attachments/assets/f4da1340-9bd0-4ec4-b385-66ed609037cf)


Filter and Receive FIFO are closely related.   
Details can be found [here](https://github.com/pierremolinaro/acanfd-stm32/blob/main/extras/acanfd-stm32.pdf).   

# Serial Output   
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/tree/main/variants/STM32G4xx

```
G431C(6-8-B)T_G441CBT\variant_generic.h(164):   #define PIN_SERIAL_TX         PA2
G431C(6-8-B)U_G441CBU\variant_B_G431B_ESC1.h(213):   #define PIN_SERIAL_TX         PB3
G431C(6-8-B)U_G441CBU\variant_generic.h(171):   #define PIN_SERIAL_TX         PA2
G431C(6-8-B)U_G441CBU\variant_PHOQUE2.h(221):   #define PIN_SERIAL_TX     PB11
G431CBTxZ\variant_generic.h(164):   #define PIN_SERIAL_TX         PA2
G431CBY_G441CBY\variant_generic.h(168):   #define PIN_SERIAL_TX         PA2
G431K(6-8-B)(T-U)_G441KB(T-U)\variant_generic.h(141):   #define PIN_SERIAL_TX         PA2
G431K(6-8-B)(T-U)_G441KB(T-U)\variant_NUCLEO_G431KB.h(138):   #define PIN_SERIAL_TX         PA2
G431M(6-8-B)T_G441MBT\variant_generic.h(202):   #define PIN_SERIAL_TX         PA2
G431R(6-8)(I-T)_G431RB(I-T)x(Z)_G441RB(I-T)\variant_generic.h(188):   #define PIN_SERIAL_TX         PA2
G431R(6-8)(I-T)_G431RB(I-T)x(Z)_G441RB(I-T)\variant_NUCLEO_G431RB.h(167):   #define PIN_SERIAL_TX         PA2
G431V(6-8-B)T_G441VBT\variant_generic.h(222):   #define PIN_SERIAL_TX         PA2
```
