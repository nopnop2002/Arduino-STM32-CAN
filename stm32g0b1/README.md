# stm32g0b1
It also works with the following STM32F4 series.   
- STM32G0B1   
- STM32G0C1   

![Image](https://github.com/user-attachments/assets/88e6b331-5397-477f-bf85-cf86c8700685)
I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# Changes from the original

- Change to local library   
The original use global library.   
This example use local library.   

- Generic STM32G0B1 Development board   
The original support only NUCLEO-G0B1RE.   
This example support Generic STM32G0B1 Development board.   

- FDCAN1 port   
The original support PA11/PA12, PB8/PB9, PC4/PC5 and PD0/PD1.   
This example support PA11/PA12, PB8/PB9, PC4/PC5, PD0/PD1 and PD12/PD13.   

- FDCAN2 port   
The original support PB0/PB1, PB5/PB6, PB12/PB13 and PC2/PC3.   
This example support PB0/PB1, PB5/PB6, PB12/PB13, PC2/PC3 and PD14/PD15.   

# FDCAN port   
The STM32G0B1 has two FDCAN ports.   
You specify which port to use.   
It is possible to use multiple ports simultaneously, but this example does not support this.   
```
#define FDCAN_PORT 0x01 // Using FDCAN1
//#define FDCAN_PORT 0x02 // Using FDCAN2
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

CAN_RX mapped to PC4, CAN_TX mapped to PC5 (not available on 32-pin package and 48-pin package)   
```
  settings.mTxPin = PC_5;
  settings.mRxPin = PC_4;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1   
```
  settings.mTxPin = PD_1;
  settings.mRxPin = PD_0;
```

CAN_RX mapped to PD12, CAN_TX mapped to PD13 (available on 80-pin and 100-pin package)   
```
  settings.mTxPin = PD_13;
  settings.mRxPin = PD_12;
```

# Remap FDCAN2 port

CAN_RX mapped to PB0, CAN_TX mapped to PB1   
```
  settings.mTxPin = PB_1;
  settings.mRxPin = PB_0;
```

CAN_RX mapped to PB5, CAN_TX mapped to PB6   
```
  settings.mTxPin = PB_6;
  settings.mRxPin = PB_5;
```

CAN_RX mapped to PB12, CAN_TX mapped to PB13 (not available on 32-pin package)   
```
  settings.mTxPin = PB_13;
  settings.mRxPin = PB_12;
```

CAN_RX mapped to PC2, CAN_TX mapped to PC3 (available on 64-pin and 80-pin and 100-pin package)   
```
  settings.mTxPin = PC_3;
  settings.mRxPin = PC_2;
```

CAN_RX mapped to PD14, CAN_TX mapped to PD15 (available on 80-pin and 100-pin package)   
```
  settings.mTxPin = PD_15;
  settings.mRxPin = PD_14;
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
The output destination for Serial.print is PA0.   

You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/tree/main/variants/STM32G0xx

# Known issues   
If we use a 3.3V transceiver such as the VP230, transmission will not work, only reception will work.   
The 5V transceiver works fine for both transmission and reception.   
I don't know the reason. Please let me know if you find out the reason.   
