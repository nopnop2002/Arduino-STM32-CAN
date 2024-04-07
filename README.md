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
- CAN Transceiver.   
MCP2551/2561/2562(5V)   
TJA1040/1050/1051/1055(5V)   
SN65HVD230/231/232(3.3V)   

__NOTE__   
3V CAN Transceivers are fully interoperable with 5V CAN Transceivers.   
Check [here](http://www.ti.com/lit/an/slla337/slla337.pdf).


# Software requirements

Arduino core support for STM32 based boards.   
https://github.com/stm32duino/Arduino_Core_STM32

![](https://img.shields.io/badge/_IMPORTANT-important)  
__There is some core library for STM32.__   
__It doesn't work with [Arduino STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32).__

# Wirering   
- USB power supply (board with 5V pin)   
 STM32 CAN RX GPIO has 5V tolerant.   
![STM32F103-CAN-5V](https://user-images.githubusercontent.com/6020549/88116844-85794f00-cbf4-11ea-8dfa-6b9db7d2e8f2.jpg)

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

# Receiver   
![STM32-Receive](https://user-images.githubusercontent.com/6020549/75561089-c2984580-5a89-11ea-80d0-90a2af235b80.jpg)


# Communication with Arduino-UNO
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.

# Communication with Arduino-DUE
You can use [this](https://github.com/collin80/due_can) library.
![STM32F103-CAN-DUO](https://user-images.githubusercontent.com/6020549/88116991-e7d24f80-cbf4-11ea-9111-1978edc8281f.jpg)

# Communication with ESP8266
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.
![STM32F103-CAN-ESP8266](https://user-images.githubusercontent.com/6020549/88117018-f6206b80-cbf4-11ea-9bd4-964c557492f8.jpg)

# Communication with ESP32
ESP-IDE has a CAN Network example.   
https://github.com/espressif/esp-idf/tree/master/examples/peripherals/twai/twai_network   
You can use [it](https://github.com/nopnop2002/esp-idf-candump).   
![STM32F103-CAN-ESP32](https://github.com/nopnop2002/Arduino-STM32-CAN/assets/6020549/84f625fd-737c-41de-8a71-daf5faf1e405)

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

![STM32F103-CAN-RPI](https://user-images.githubusercontent.com/6020549/197652542-8d08ad97-fb10-4654-a818-ad2761134235.jpg)

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

### About transmission
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

### About reception
STM32 has two receive(RX) mailboxes.   
Each RX Mailbox allows access to a 3-level depth FIFO, the access being offered only to the oldest received message in the FIFO.   
This sample code uses only the first RX mailbox.   
The RX mailbox is closely related to the receive filter settings.   
By properly setting the receive filter, you can sort the received messages into two RX Mailboxes.   
For example, high priority messages can be stored in the first RX MailBox and low priority messages can be stored in the second RX MailBox.   
Read the reference manual carefully and modify the source code as needed.   

# Reference

https://github.com/nopnop2002/Robotell-USB-CAN-Python

https://github.com/nopnop2002/esp-idf-CANBus-Monitor


