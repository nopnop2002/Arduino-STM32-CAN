# stm32g474
![Image](https://github.com/user-attachments/assets/145c1189-efd3-4738-ad77-69c817020750)

It also works with the following STM32G4 series.   
- STM32G473   
- STM32G474   
- STM32G483   
- STM32G484   

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# Changes from the original

- Change to local library   
The original use global library.   
This example use local library.   

- Generic STM32G474 Development board   
The original support only NUCLEO-G474RE.   
This example support Generic STM32G474 Development board.   

- FDCAN1 port   
The original support PA11/PA12 and PB8/PB9.   
This example support PA11/PA12,PB8/PB9 and PD0/PD1.   

# FDCAN port   
The STM32G474 has three FDCAN ports.   
You specify which port to use.   
It is possible to use multiple ports simultaneously, but this example does not support this.   
```
#define FDCAN_PORT 0x01 // Using FDCAN1
//#define FDCAN_PORT 0x02 // Using FDCAN2
//#define FDCAN_PORT 0x03 // Using FDCAN3
```

# Remap FDCAN1 port

CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
  settings.mTxPin = PA_12;
  settings.mRxPin = PA_11;
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9   
```
  settings.mTxPin = PB_9;
  settings.mRxPin = PB_8;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (not available on 48-pin package and 64-pin package)   
```
  settings.mTxPin = PD_1;
  settings.mRxPin = PD_0;
```

# Remap FDCAN2 port

CAN_RX mapped to PB5, CAN_TX mapped to PB6   
```
  settings.mTxPin = PB_6;
  settings.mRxPin = PB_5;
```

CAN_RX mapped to PB12, CAN_TX mapped to PB13   
```
  settings.mTxPin = PB_13;
  settings.mRxPin = PB_12;
```

# Remap FDCAN3 port

CAN_RX mapped to PA8, CAN_TX mapped to PA15   
```
  settings.mTxPin = PA_15;
  settings.mRxPin = PA_8;
```

CAN_RX mapped to PB3, CAN_TX mapped to PB4   
```
  settings.mTxPin = PB_4;
  settings.mRxPin = PB_3;
```



# Using Filter   
```
#define ENABLE_FILTER 0x00 // No filters are used.
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

# Serial Output   
The output destination for Serial.print is PA2.   

You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/tree/main/variants/STM32G4xx

```
G473C(B-C-E)T_G474C(B-C-E)T_G483CET_G484CET\variant_generic.h(169):   #define PIN_SERIAL_TX         PA2
G473C(B-C-E)U_G474C(B-C-E)U_G483CEU_G484CEU\variant_generic.h(176):   #define PIN_SERIAL_TX         PA2
G473C(B-C-E)U_G474C(B-C-E)U_G483CEU_G484CEU\variant_WEACT_G474CE.h(176):   #define PIN_SERIAL_TX         PA2
G473M(B-C-E)T_G474M(B-C-E)T_G483MET_G484MET\variant_generic.h(222):   #define PIN_SERIAL_TX         PA2
G473MEY_G474MEY_G483MEY_G484MEY\variant_generic.h(225):   #define PIN_SERIAL_TX         PA2
G473P(B-C-E)I_G474P(B-C-E)I_G483PEI_G484PEI\variant_generic.h(274):   #define PIN_SERIAL_TX         PA2
G473Q(B-C)T_G473QETx(Z)_G474Q(B-C-E)T_G483QET_G484QET\variant_generic.h(279):   #define PIN_SERIAL_TX         PA2
G473R(B-C)T_G473RETx(Z)_G474R(B-C-E)T_G483RET_G484RET\variant_generic.h(195):   #define PIN_SERIAL_TX         PA2
G473R(B-C)T_G473RETx(Z)_G474R(B-C-E)T_G483RET_G484RET\variant_NUCLEO_G474RE.h(171):   #define PIN_SERIAL_TX         PA2
G473V(B-C-E)(H-T)_G474V(B-C-E)(H-T)_G483VE(H-T)_G484VE(H-T)\variant_generic.h(257):   #define PIN_SERIAL_TX         PA2
```
