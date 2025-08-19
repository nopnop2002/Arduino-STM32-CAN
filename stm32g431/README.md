# stm32g431
![Image](https://github.com/user-attachments/assets/85013bac-8e1e-45eb-aa72-3b504fa7d8a0)   
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
  settings.mTxPin = PA12;
  settings.mRxPin = PA11;
```

CAN_RX mapped to PB8, CAN_TX mapped to PB9 (not available on 32-pin package)   
```
  settings.mTxPin = PB9;
  settings.mRxPin = PB8;
```

CAN_RX mapped to PD0, CAN_TX mapped to PD1 (available on 80-pin and 100-pin package)   
```
  settings.mTxPin = PD1;
  settings.mRxPin = PD0;
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
The output destination for Serial.print is PA2.   
However, this may vary depending on the Board Part Number.   

You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32G4xx/


# Known issues   
If we use a 3.3V transceiver such as the VP230, transmission will not work, only reception will work.   
The 5V transceiver works fine for both transmission and reception.   
I don't know the reason. Please let me know if you find out the reason.   
