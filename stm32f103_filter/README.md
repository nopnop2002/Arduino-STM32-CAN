# stm32f103 filter example
This example demonstrates how to use receive filters.   
Please enable one of the following filters:   
```
#if 0
  // One 32-Bit Filter - Identifier Mask
  int fsc = 1;
  int fbm = 0;
  bank1 = 0x101 << 21;
  bank2 = 0xFFE00006; // Must be IDE=0 RTR=0  
  CANSetFilter(0, fsc, fbm, 0, bank1, bank2);

  // One 32-Bit Filter - Identifier Mask
  bank1 = 0x101 << 3;
  bank1 = bank1 + 0x04; // Ext
  bank2 = 0xFFFFFFFE; // Must be IDE=1 RTR=0
  CANSetFilter(1, fsc, fbm, 0, bank1, bank2);
#endif

#if 0
  // Two 32-Bit Filters - Identifier List
  int fsc = 1;
  int fbm = 1;
  bank1 = 0x102 << 21;
  bank2 = 0x103 << 21;
  CANSetFilter(0, fsc, fbm, 0, bank1, bank2);

  bank1 = 0x102 << 3;
  bank1 = bank1 + 0x04; // Ext
  bank2 = 0x103 << 3;
  bank2 = bank2 + 0x04; // Ext
  CANSetFilter(1, fsc, fbm, 0, bank1, bank2);
#endif

#if 0
  // Two 16-Bit Filters - Identifier Mask
  int fsc = 0;
  int fbm = 0;
  bank1 = 0xFFFF << 16;
  bank1 = bank1 | 0x104 << 5;
  bank2 = 0xFFFF << 16;
  bank2 = bank1 | 0x105 << 5;
  CANSetFilter(0, fsc, fbm, 0, bank1, bank2);
#endif

#if 0
  // Four 16-Bit Filters - Identifier List
  int fsc = 0;
  int fbm = 1;
  bank1 = 0x104 << 21;
  bank1 = bank1 | 0x105 << 5;
  bank2 = 0x106 << 21;
  bank2 = bank2 | 0x107 << 5;
  CANSetFilter(0, fsc, fbm, 0, bank1, bank2); // use 4 filter(filter number 0-3)
#endif
```

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


