# stm32h562
<img width="864" height="576" alt="stm32h562_DevelopmentBoard" src="https://github.com/user-attachments/assets/fe487833-b791-41e7-8a0c-e057b6f44610" />

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# Remap FDCAN1 port

CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
  settings.mTxPin = PA_12;
  settings.mRxPin = PA_11;
```

CAN_RX mapped to P83, CAN_TX mapped to PB9 (not available on 36-pin package)   
```
  settings.mTxPin = PB_9;
  settings.mRxPin = PB_8;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 100-pin and 144-pin package)   
```
  settings.mTxPin = PD_1
  settings.mRxPin = PD_0;
```

CAN_RX mapped to PE0, CAN_TX mapped to PE1 (available on 100-pin and 144-pin package)   
```
  settings.mTxPin = PE_1;
  settings.mRxPin = PE_0;
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
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32H5xx/

