# stm32g491
<img width="3456" height="2304" alt="stm32g491_DevelopmentBoard" src="https://github.com/user-attachments/assets/a4abb7f7-9cb5-4418-9cd4-f0ad71958844" />

It also works with the following STM32G4 series.   
- STM32G491   
- STM32G4A1   

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# FDCAN port   
The STM32G491 has two FDCAN ports.   
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

CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 36-pin package)   
```
  settings.mTxPin = PB_9;
  settings.mRxPin = PB_8;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 100-pin and 144-pin package)   
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

CAN_RX mapped to PB12, CAN_TX mapped to PB13 (not available on 36-pin package)   
```
  settings.mTxPin = PB_13;
  settings.mRxPin = PB_12;
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
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32G4xx/

# Known issues   
If we use a 3.3V transceiver such as the VP230, transmission will not work, only reception will work.   
The 5V transceiver works fine for both transmission and reception.   
I don't know the reason. Please let me know if you find out the reason.   
