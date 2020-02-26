# Arduino-STM32-CAN
Can Example for Arduino Core STM32

# Hardware requirements

- STM32F103 Development Board.   
- STM32F303 Development Board.   
- CAN Transceiver.   
MCP2551(5V)   
SN65HVD230(3.3V)   

# Software requirements

STM32 core support for Arduino.   
https://github.com/stm32duino/Arduino_Core_STM32

# Wirering   
- Powered by USB   
![STM32F103-CAN-1](https://user-images.githubusercontent.com/6020549/75340817-87094a00-58d6-11ea-9067-9b7c0a50fee8.jpg)

- Powered by ST-Link   
![STM32F103-CAN-2](https://user-images.githubusercontent.com/6020549/75340826-8a043a80-58d6-11ea-8f78-fdd1e8145cd9.jpg)


# Transfer
- STM32F103   
![STM32F103-Transfer](https://user-images.githubusercontent.com/6020549/75340563-119d7980-58d6-11ea-9e70-00d56f7f1234.jpg)

- STM32F303   
![STM32F303-Transfer](https://user-images.githubusercontent.com/6020549/75340579-195d1e00-58d6-11ea-8fdd-f8fea31e9b7d.jpg)



# Receive
![STM32-Receive](https://user-images.githubusercontent.com/6020549/75340537-00ed0380-58d6-11ea-8a7c-15c2bbda87a9.jpg)


# Communication with Arduino-UNO
You can use [this](https://github.com/linux-can/can-utils) library.

# Communication with Arduino-DUE
You can use [this](https://github.com/collin80/due_can) library.

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

Install can-util.
```
$ sudo apt-get install can-utils

#Receiver
$ candump can0

#Transfer
$ cansend can0 123#11223344AABBCCDD
```

