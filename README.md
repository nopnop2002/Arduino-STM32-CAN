# Arduino-STM32-CAN
Can Example for Arduino Core STM32.   
I forked from [here](https://github.com/geosmall/UAVCAN-for-STM32-Arduino).


# Hardware requirements

- STM32F103 Development Board.   
- STM32F303 Development Board.   
- CAN Transceiver.   
MCP2551(5V)   
SN65HVD230(3.3V)   

__NOTE__:3V CAN Trasnceviers are fully interoperable with 5V CAN trasnceviers.   
Check [here](http://e2e.ti.com/cfs-file.ashx/__key/communityserver-discussions-components-files/142/4747.2012_2D00_11_2D00_02-CAN-Transceiver-3V-and-5V-Mixed-Network-Basics.pdf) and [here](http://www.ti.com/lit/an/slla337/slla337.pdf).


# Software requirements

STM32 core support for Arduino.   
https://github.com/stm32duino/Arduino_Core_STM32

# Wirering   
- USB power supply (board with 5V pin)   
![STM32F103-CAN-1](https://user-images.githubusercontent.com/6020549/75343857-b02cd900-58dc-11ea-9022-1a6e4530ffc8.jpg)

- Board without 5V pin, such as BlackPill, or ST-Link power supply  
![STM32F103-CAN-2](https://user-images.githubusercontent.com/6020549/75353583-03a82280-58ef-11ea-890d-ea2c52930453.jpg)


# API
- Initialize   
void initCanard(uint32_t speed);   

- Send CAN Packet from queue   
void sendCanard(void);   

- Receive CAN Packet   
bool receiveCanard(uint32_t * can_id, uint8_t * payload, uint8_t * payload_size);

- Push CAN Packet to TX queue   
void queueCanard(uint32_t can_id, uint8_t * payload, uint8_t payload_size);

# Transfer
- STM32F103   
CAN ID:0x103xx   

![STM32F103-Transfer](https://user-images.githubusercontent.com/6020549/75340563-119d7980-58d6-11ea-9e70-00d56f7f1234.jpg)

- STM32F303   
CAN ID:0x303xx   

![STM32F303-Transfer](https://user-images.githubusercontent.com/6020549/75340579-195d1e00-58d6-11ea-8fdd-f8fea31e9b7d.jpg)

# Receive   
Serial printing goes to PA9.   
![STM32-Receive](https://user-images.githubusercontent.com/6020549/75340537-00ed0380-58d6-11ea-8a7c-15c2bbda87a9.jpg)


# Communication with Arduino-UNO
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.

# Communication with Arduino-DUE
You can use [this](https://github.com/collin80/due_can) library.
![STM32F103-CAN-DUO](https://user-images.githubusercontent.com/6020549/75351657-d312b980-58eb-11ea-97e6-83b6eb409b88.jpg)

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

#Receiver
$ candump can0

#Transfer
$ cansend can0 123#11223344AABBCCDD
```

![STM32F103-CAN-RPI](https://user-images.githubusercontent.com/6020549/75351666-d60daa00-58eb-11ea-8eca-ba74cf916439.jpg)
