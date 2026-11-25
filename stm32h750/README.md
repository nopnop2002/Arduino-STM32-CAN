# stm32h750
<img width="864" height="576" alt="stm32h750_DevelopmentBoard" src="https://github.com/user-attachments/assets/c6423ddb-9da9-4254-9366-fc20c2b78102" />

It also works with the following STM32H7 series.   
- STM32H743   
- STM32H750   

I based this on [this](https://github.com/pierremolinaro/acanfd-stm32).

# FDCAN port   
The STM32H750 has two FDCAN ports.   
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
The output destination of Serial.print differs depending on the Board Part Number.   
You need to check variant_generic.h for your board.    
https://github.com/stm32duino/Arduino_Core_STM32/blob/main/variants/STM32H7xx/

```
H742I(G-I)(K-T)_H743I(G-I)(K-T)_H750IB(K-T)_H753II(K-T)\variant_DAISY_PATCH_SM.h(243):   #define PIN_SERIAL_TX         PA0
H742I(G-I)(K-T)_H743I(G-I)(K-T)_H750IB(K-T)_H753II(K-T)\variant_DAISY_PETAL_SM.h(138): #define PIN_SERIAL_TX           PB6
H742I(G-I)(K-T)_H743I(G-I)(K-T)_H750IB(K-T)_H753II(K-T)\variant_DAISY_SEED.h(133): #define PIN_SERIAL_TX           PB6
H742I(G-I)(K-T)_H743I(G-I)(K-T)_H750IB(K-T)_H753II(K-T)\variant_generic.h(279):   #define PIN_SERIAL_TX         PA0
H742V(G-I)(H-T)_H743V(G-I)(H-T)_H750VBT_H753VI(H-T)\variant_DevEBoxH7xx.h(229):   #define PIN_SERIAL_TX         PA9
H742V(G-I)(H-T)_H743V(G-I)(H-T)_H750VBT_H753VI(H-T)\variant_generic.h(219):   #define PIN_SERIAL_TX         PA0
H742V(G-I)(H-T)_H743V(G-I)(H-T)_H750VBT_H753VI(H-T)\variant_WeActMiniH7xx.h(228):   #define PIN_SERIAL_TX         PA9
H742X(G-I)H_H743X(G-I)H_H745X(G-I)H_H747X(G-I)H_H750XBH_H753XIH_H755XIH_H757XIH\variant_generic.h(323):   #define PIN_SERIAL_TX         PA0
H742X(G-I)H_H743X(G-I)H_H745X(G-I)H_H747X(G-I)H_H750XBH_H753XIH_H755XIH_H757XIH\variant_STM32H747I_DISCO.h(297):   #define PIN_SERIAL_TX         PA9
H742Z(G-I)T_H743Z(G-I)T_H747A(G-I)I_H747I(G-I)T_H750ZBT_H753ZIT_H757AII_H757IIT\variant_generic.h(253):   #define PIN_SERIAL_TX         PA0
H742Z(G-I)T_H743Z(G-I)T_H747A(G-I)I_H747I(G-I)T_H750ZBT_H753ZIT_H757AII_H757IIT\variant_NUCLEO_H743ZI.h(362):   #define PIN_SERIAL_TX         PD8
H742Z(G-I)T_H743Z(G-I)T_H747A(G-I)I_H747I(G-I)T_H750ZBT_H753ZIT_H757AII_H757IIT\variant_NUCLEO_H753ZI.h(234):   #define PIN_SERIAL_TX         PD8
```
