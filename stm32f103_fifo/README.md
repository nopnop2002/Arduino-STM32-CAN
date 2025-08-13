# stm32f103 filter and fifo example
The BxCan has two receive FIFOs.   
This example demonstrates how to use receive filters and receive fifo.   
All standard IDs are stored in FIFO 0.   
All extended IDs are stored in FIFO 1.   
![Image](https://github.com/user-attachments/assets/87fe2cd1-96f5-44bd-b835-b12f863829ba)

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


