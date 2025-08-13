# Arduino-STM32-CAN
Basic Extended CAN(BxCan) communication example for Arduino Core STM32.   


# Hardware requirements

- STM32F072 Development Board.   
- STM32F103 Development Board.   
- STM32F303 Development Board.   
- STM32F405 Development Board.   
- STM32F407 Development Board.   
- STM32F413 Development Board.   
- STM32F446 Development Board.   
- STM32F722 Development Board.   
- STM32F767 Development Board.   
- STM32L452 Development Board.   
- STM32L496 Development Board.   
- STM32G431 Development Board.   
- CAN Transceiver.   
MCP2551/2561/2562(5V)   
TJA1040/1050/1051/1055(5V)   
SN65HVD230/231/232(3.3V)   

__NOTE__   
3V CAN Transceivers are fully interoperable with 5V CAN Transceivers.   
Check [here](http://www.ti.com/lit/an/slla337/slla337.pdf).


# Software requirements
- Arduino IDE   
 I used V1.8.19   
 ![Arduino_V1_8_19](https://github.com/user-attachments/assets/f3179568-cce6-4762-99e3-509852c7fa5d)

- Arduino core support for STM32 based boards.   
 https://github.com/stm32duino/Arduino_Core_STM32   
 ___Note for Core version___   
 Core version 2.8 or later requires Arduino IDE 2.x.   
 I used version 2.7.1.   
 This is the final version available for Arduino IDE 1.x.   
 ![](https://img.shields.io/badge/_IMPORTANT-important)  
 __There is some core library for STM32.__   
 __It doesn't work with [Arduino STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32).__
 ![STM32_Core_2 7 1](https://github.com/user-attachments/assets/e135c6b2-61f5-4ac8-a4e5-8878a29a2be4)

# Wirering   
- USB power supply (board with 5V pin)   
 STM32 CAN RX GPIO has 5V tolerant.   
![STM32F103-CAN-5V](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/b344eaaf-9b73-4321-a84b-d0715269126c)

- Board without 5V pin, such as BlackPill, or ST-Link power supply  
![STM32F103-CAN-3V3](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/4bf2508f-ac6f-4b5b-8ded-e700beced1ca)

# My test circuit
The Nano on the left is a receiver with a terminating resistor.   
The Nano on the right is a transmitter without a terminating resistor.   
The STM32 transceiver has a terminating resistor.   
- Using SN65HVD230   
![Test-Circuit-SN65HVD230](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/cd69745e-c7c9-4419-aeb0-4611c17f800a)

- Using TJA1051   
![Test-Circuit-TJA1051](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/1cddb8be-1a83-473e-ae6c-d63528da4e81)

# Transmitter

- from STM32F072   
![STM32F072_Send](https://user-images.githubusercontent.com/6020549/109580416-e8917380-7b3d-11eb-915e-5d13c6f5fe3f.jpg)

- from STM32F103   
![STM32F103_Send](https://user-images.githubusercontent.com/6020549/80896902-3d9e0680-8d2e-11ea-9add-0a102f43c3a7.jpg)

- from STM32F303   
![STM32F303_Send](https://user-images.githubusercontent.com/6020549/80896905-4262ba80-8d2e-11ea-9c3b-3f4871a947bb.jpg)

- from STM32F407   
![STM32F405_Send](https://user-images.githubusercontent.com/6020549/80896908-45f64180-8d2e-11ea-91a3-c34fdb48725b.jpg)

- from STM32F413   
![stm32f413_transfer](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/29dc0e5b-e7ab-4aaf-aee2-e57b6185b2a3)

- from STM32F446   
![stm32f446_transfer](https://user-images.githubusercontent.com/6020549/110085501-8900c580-7dd4-11eb-9fb7-311c21800719.jpg)

- from STM32F767   
![stm32f767_transfer](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/39d5fc41-8b22-4f28-be5d-e65fa936e0ac)

- from STM32L452   
![stm32l452_transfer](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/d92ab5c4-b708-4b5f-8114-7a59914402c8)

- from STM32G431   
![Image](https://github.com/user-attachments/assets/bea22f0a-1ff9-4ba9-9e54-f83333b5370a)

# Receiver   
![STM32-Receive](https://user-images.githubusercontent.com/6020549/75561089-c2984580-5a89-11ea-80d0-90a2af235b80.jpg)

# Using the BxCan receiving filter   
The bxCAN Controller provides 14 or 28 configurable and scalable filter banks to the application.   
The number of filter banks varies by model and is specified in the Reference manual.   
On models with only one CAN port, all filters are available on this port.   
On models with two CAN ports, they are shared between ports 1 and 2.   
In this example, filter banks 0 to 13 are used on port 1, and filter banks 14 to 27 are used on port 2.   

The bxCAN controller has four types of reception filters.   
![Image](https://github.com/user-attachments/assets/adc56930-8681-4e57-b102-c8a9ac1105f0)   

When configuring a filter, you must set to filter initialization mode.   
```
	uint32_t bank1, bank2;
	CAN1->FMR  |=   0x1UL;                // Set to filter initialization mode
	bank1 = your settings
	bank2 = your settings
	CANSetFilter(0, 0, 1, 0, bank1, bank2);
	CAN1->FMR   &= ~(0x1UL);              // Deactivate initialization mode
```

This is the simplest usage.   
The filter and FIFO work together.   
Please refer to the reference manual for each model for more information.   

- One 32-Bit Filter - Identifier Mask   
	This can be used for both standard and extended IDs.   
	```
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
	```
	![Image](https://github.com/user-attachments/assets/c2b10523-5eb6-4ce3-85e3-026becf89c34)

- Two 32-Bit Filters - Identifier List   
	This can be used for both standard and extended IDs.   
	```
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
	```
	![Image](https://github.com/user-attachments/assets/81ca9205-d0cb-4e5f-87c7-c7a69f850827)

- Two 16-Bit Filters - Identifier Mask   
	This can only be used with standard IDs.   
	```
	  // Two 16-Bit Filters - Identifier Mask
	  int fsc = 0;
	  int fbm = 0;
	  bank1 = 0xFFFF << 16;
	  bank1 = bank1 | 0x104 << 5;
	  bank2 = 0xFFFF << 16;
	  bank2 = bank1 | 0x105 << 5;
	  CANSetFilter(0, fsc, fbm, 0, bank1, bank2);
	```
	![Image](https://github.com/user-attachments/assets/b2dfa2bb-659f-480c-af69-895c844a93c3)

- Four 16-Bit Filters - Identifier List   
	This can only be used with standard IDs.   
	```
	  // Four 16-Bit Filters - Identifier List
	  int fsc = 0;
	  int fbm = 1;
	  bank1 = 0x104 << 21;
	  bank1 = bank1 | 0x105 << 5;
	  bank2 = 0x106 << 21;
	  bank2 = bank2 | 0x107 << 5;
	  CANSetFilter(0, fsc, fbm, 0, bank1, bank2);
	```
	![Image](https://github.com/user-attachments/assets/dc1022da-f9e0-4c32-8e08-c49d240e74bd)

STM32 has two receive(RX) FIFOs.   
Each RX FIFO has three mailboxes and provides access only to the oldest received message in the mailbox.   
This sample code uses only the FIFO 0.   
The RX FIFO is closely related to the receive filter settings.   
By properly setting the receive filter, you can sort the received messages into two RX FIFOs.   
For example, high priority messages can be stored in the FIFO0 and low priority messages can be stored in the FIFO1.   
[Here](https://github.com/nopnop2002/Arduino-STM32-CAN/tree/master/stm32f103_fifo) is an example for STM32F103.   

# Communication with Arduino-UNO R3
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.

# Communication with Arduino-UNO R4
Arduino-UNO R4 has a built-in CAN controller.    
You can use [this](https://github.com/arduino/ArduinoCore-renesas/tree/main/libraries/Arduino_CAN) library.   
![Image](https://github.com/user-attachments/assets/d482ea79-bcea-4a40-b9cd-f60f937b6794)

# Communication with Arduino-DUE
You can use [this](https://github.com/collin80/due_can) library.   
![STM32F103-CAN-DUO](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/7bd4e8dd-77b2-4b25-9123-a3596a006e5f)

# Communication with ESP8266
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.   
![STM32F103-CAN-ESP8266](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/b7046d49-7e4a-40b0-bbc4-7a55fc5dc427)

# Communication with ESP32
ESP-IDE has a CAN Network example.   
https://github.com/espressif/esp-idf/tree/master/examples/peripherals/twai/twai_network   
You can use [it](https://github.com/nopnop2002/esp-idf-candump).   
![STM32F103-CAN-ESP32](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/fbf48367-d134-43f3-9891-ff714b38eb32)

__Notes on ESP32 bitrate__   
In ESP-IDF V4, CONFIG_ESP32_REV_MIN >= 2 means "halve communication speed".   
This is to support slower bitrates below 25Kbps.   
This fuature can be controlled by CONFIG_ESP32_REV_MIN.   
This fuature is enabled when CONFIG_ESP32_REV_MIN >= 2.   
See [here](https://www.esp32.com/viewtopic.php?t=15581) for detail.   
This specification is not available in ESP-IDF V5.   


# Communication with Raspberry Pi
Edit /boot/config.txt and reboot.   
```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```


Make sure the device is ready after reboot.
```
$ dmesg | grep mcp251x
[   19.992025] mcp251x spi0.0 can0: MCP2515 successfully initialized.

$ ls /sys/bus/spi/devices/spi0.0
driver  modalias  net  of_node  power  statistics  subsystem  uevent

$ ls /sys/bus/spi/devices/spi0.0/net
can0

$ sudo /sbin/ip link set can0 up type can bitrate 1000000

$ dmesg | grep can0
[   19.992025] mcp251x spi0.0 can0: MCP2515 successfully initialized.
[ 1309.525795] IPv6: ADDRCONF(NETDEV_CHANGE): can0: link becomes ready

$ sudo ifconfig can0
can0: flags=193<UP,RUNNING,NOARP>  mtu 16
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 10  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

$ sudo ifconfig can0 txqueuelen 1000
```

Install can-utils.
```
$ sudo apt-get install can-utils

# Receiver
$ candump can0

# Transmitter
$ cansend can0 123#11223344AABBCCDD
```

![STM32F103-CAN-RPI](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/bebd8b14-5565-4cc8-b6ba-82e23c1ef14c)

# Troubleshooting   
There is a module of SN65HVD230 like this.   
![SN65HVD230-1](https://user-images.githubusercontent.com/6020549/80897499-4d204e00-8d34-11ea-80c9-3dc41b1addab.JPG)

There is a __120 ohms__ terminating resistor on the left side.   
![SN65HVD230-22](https://user-images.githubusercontent.com/6020549/89281044-74185400-d684-11ea-9f55-830e0e9e6424.JPG)

A transmission fail will occur.   
![SendFail](https://user-images.githubusercontent.com/6020549/80897131-98d0f880-8d30-11ea-96b6-05e50ac740a3.jpg)

I have removed the terminating resistor.   
And I used a external resistance of __150 ohms__.   
A transmission fail is fixed.   
![SN65HVD230-33](https://user-images.githubusercontent.com/6020549/89280710-f7857580-d683-11ea-9b36-12e36910e7d9.JPG)

If the transmission fails, these are the possible causes.   
- There is no receiving app on CanBus.
- The speed does not match the receiver.
- There is no terminating resistor on the CanBus.
- There are three terminating resistors on the CanBus.
- The resistance value of the terminating resistor is incorrect.
- 3.3V transceiver and 5V transceiver are mixed.   
 Use +1V Ground Shift using Split Termination.   
 Check out Figure 7 [here](http://www.ti.com/lit/an/slla337/slla337.pdf).   
- Stub length in CAN bus is too long.   
 ISO11898 Standard specifies a maximum bus length of 40m and maximum stub length of 0.3m at 1Mbps.
 See [here](https://e2e.ti.com/support/interface-group/interface/f/interface-forum/378932/iso1050-can-bus-stub-length).   
- Noise effects.   
 It occurs when the wire cable is thin or wire cable is crossing.   
 It is preferable to use wires that are noise-proof.   

I changed the single wire to twisted wire. It's fixed.   
![CAN-SendFail](https://user-images.githubusercontent.com/6020549/185782335-66d1f534-5220-40a2-aee9-5d4a438c59a0.jpg)

# Using PlatformIO   
Arduino-IDE only supports ST-LINK V2.1 adapters, but OpenOCD used by PlatformIO supports both V2.0 and V2.1.   
With ST-LINK, there is no need to change boot mode when writing firmware.   
PlatformIO allows you to use cheap Chinese ST-LINK adapters like this one.   
You can get it at a low price (about $2).   
![ST-LINK-1](https://user-images.githubusercontent.com/6020549/221345715-b86e0a93-bdf4-46dd-a9b9-c05a27b042a4.JPG)
![ST-LINK-2](https://user-images.githubusercontent.com/6020549/221345711-7749b557-d55f-442f-8390-3632c63d5239.JPG)

Connect the ST-LINK adapter and the STM32 development board as follows.
|ST-LINK|STM32|
|:---|:---|
|3.3V|3.3V|
|GND|GND|
|SWDIO|SWIO(=PA13)|
|SWCLK|SWCLK(=PA14)|

![platformio-2](https://user-images.githubusercontent.com/6020549/221346022-c199e146-2f17-4021-871c-5e1b502ffde7.JPG)

# Advanced use of BxCan   
Skip if you don't need advanced usage.   
Advanced usage has different purposes depending on the requirements of the application.   
So I can't give you a concrete code example.   

STM32 has three transmit(TX) mailboxes.   
Three TX mailboxes are provided to the software for setting up messages.   
This sample code uses only the first TX mailbox and blocks the application until the send is successfully completed.   
There are two ways to know if the send was successful without blocking.   
Read the reference manual carefully and modify the source code as needed.   

- Uses a transmission completion interrupt   
 You need to set the CAN interrupt enable register (CAN_IER) appropriately.   

- Check TX mailbox space before sending   
 An application can store transmission requests in three TX mailboxes simultaneously.   
 When there are requests for multiple TX mailboxes, the transmission scheduler decides which mailbox has to be transmitted first.   
 You can specify which mailbox has priority using the CAN Master Control Register (CAN_MCR).   
 You can tell if a mailbox is free using the CAN Transmit Status Register (CAN_TSR).   
 When all three TX mailboxes are pending, no new messages can be sent until the transmit is canceled or completed.   
 When all three TX mailboxes are pending, you can choose to cancel the pending transmission, wait until the transmission is complete, or give up on the new transmission.   
 This choice depends on your application requirements.   
 When all three TX mailboxes are pending for a long time, it could be a transmission error.   
 CAN Error Status Register (CAN_ESR) should be checked.   


# Reference

https://github.com/nopnop2002/Robotell-USB-CAN-Python

https://github.com/nopnop2002/esp-idf-CANBus-Monitor


