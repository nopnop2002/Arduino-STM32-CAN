# Arduino-STM32-CAN
Can Example for Arduino Core STM32.   


# Hardware requirements

- STM32F103 Development Board.   
- STM32F303 Development Board.   
- STM32F405 Development Board.   
- STM32F407 Development Board.   
- STM32F446 Development Board.   
- CAN Transceiver.   
MCP2551/2561(5V)   
TJA1040/1050/1055(5V)   
SN65HVD230/231/232(3.3V)   
ISO1050(3.3V)   

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

# Transmitter

- from STM32F103   
![STM32F103_Send](https://user-images.githubusercontent.com/6020549/80896902-3d9e0680-8d2e-11ea-9add-0a102f43c3a7.jpg)

- from STM32F303   
![STM32F303_Send](https://user-images.githubusercontent.com/6020549/80896905-4262ba80-8d2e-11ea-9c3b-3f4871a947bb.jpg)

- from STM32F407   
![STM32F405_Send](https://user-images.githubusercontent.com/6020549/80896908-45f64180-8d2e-11ea-91a3-c34fdb48725b.jpg)

# Receiver   
Serial printing goes to PA9.   
![STM32-Receive](https://user-images.githubusercontent.com/6020549/75561089-c2984580-5a89-11ea-80d0-90a2af235b80.jpg)


# Communication with Arduino-UNO
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.

# Communication with Arduino-DUE
You can use [this](https://github.com/collin80/due_can) library.
![STM32F103-CAN-DUO](https://user-images.githubusercontent.com/6020549/75880245-43778880-5e60-11ea-9688-6523d85c10fc.jpg)

# Communication with ESP8266
You can use [this](https://github.com/coryjfowler/MCP_CAN_lib) library.
![STM32F103-CAN-ESP8266](https://user-images.githubusercontent.com/6020549/79196756-9a16b000-7e6b-11ea-8d60-5afee96794bd.jpg)

# Communication with ESP32
ESP-IDE has a CAN Network example.   
https://github.com/espressif/esp-idf/tree/master/examples/peripherals/can/can_network
![STM32F103-CAN-ESP32](https://user-images.githubusercontent.com/6020549/79196765-9e42cd80-7e6b-11ea-8fa9-a31c3ec9334b.jpg)

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

#Transmitter
$ cansend can0 123#11223344AABBCCDD
```

![STM32F103-CAN-RPI](https://user-images.githubusercontent.com/6020549/75351666-d60daa00-58eb-11ea-8eca-ba74cf916439.jpg)


# Troubleshooting   
If the value of the terminating resistor is incorrect, the following error will occur.   
The solution was to change the terminating resistance from 120 ohms to 150 ohms.   
![SendFail](https://user-images.githubusercontent.com/6020549/80897131-98d0f880-8d30-11ea-96b6-05e50ac740a3.jpg)

