# stm32h503
<img width="864" height="576" alt="stm32h503_DevelopmentBoard" src="https://github.com/user-attachments/assets/33502840-0bfe-492f-a913-05ddbc31574f" />

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# Remap FDCAN1 port

CAN_RX mapped to PA11, CAN_TX mapped to PA12   
```
  settings.mTxPin = PA_12;
  settings.mRxPin = PA_11;
```

CAN_RX mapped to PB3, CAN_TX mapped to PB4 (not available on 36-pin package)   
```
  settings.mTxPin = PB_4;
  settings.mRxPin = PB_3;
```

CAN_RX mapped to PB5, CAN_TX mapped to PB6 (available on 100-pin and 144-pin package)   
```
  settings.mTxPin = PB_6
  settings.mRxPin = PB_5;
```

CAN_RX mapped to PB12, CAN_TX mapped to PB13 (available on 100-pin and 144-pin package)   
```
  settings.mTxPin = PB_13
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
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32H5xx/

