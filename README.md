# Arduino-STM32-CAN
Basic Extended CAN(BxCan) communication example for Arduino Core STM32.   


# Hardware requirements

- STM32F072 Development Board.   
- STM32F103 Development Board.   
- STM32F303 Development Board.   
- STM32F405 Development Board.   
- STM32F407 Development Board.   
- STM32F446 Development Board.   
- CAN Transceiver.   
MCP2551/2561/2562(5V)   
TJA1040/1050/1055(5V)   
SN65HVD230/231/232(3.3V)   

__NOTE__   
3V CAN Trasnceviers are fully interoperable with 5V CAN trasnceviers.   
Check [here](http://www.ti.com/lit/an/slla337/slla337.pdf).


# Software requirements

STM32 core support for Arduino.   
https://github.com/stm32duino/Arduino_Core_STM32

![](https://img.shields.io/badge/_IMPORTANT-important)  
__There is some core library for STM32.__   
__It doesn't work with [Arduino STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32).__

# Wirering   
- USB power supply (board with 5V pin)   
 __Do not use the Windows USB port.__   

![STM32F103-CAN-5V](https://user-images.githubusercontent.com/6020549/88116844-85794f00-cbf4-11ea-8dfa-6b9db7d2e8f2.jpg)

- Board without 5V pin, such as BlackPill, or ST-Link power supply  
![STM32F103-CAN-3V3](https://user-images.githubusercontent.com/6020549/88116865-95912e80-cbf4-11ea-9fdd-43c60e9b1b7c.jpg)

# Transmitter

- from STM32F072   
![STM32F072_Send](https://user-images.githubusercontent.com/6020549/109580416-e8917380-7b3d-11eb-915e-5d13c6f5fe3f.jpg)

- from STM32F103   
![STM32F103_Send](https://user-images.githubusercontent.com/6020549/80896902-3d9e0680-8d2e-11ea-9add-0a102f43c3a7.jpg)

- from STM32F303   
![STM32F303_Send](https://user-images.githubusercontent.com/6020549/80896905-4262ba80-8d2e-11ea-9c3b-3f4871a947bb.jpg)

- from STM32F407   
![STM32F405_Send](https://user-images.githubusercontent.com/6020549/80896908-45f64180-8d2e-11ea-91a3-c34fdb48725b.jpg)

- from STM32F446   
![stm32f446_transfer](https://user-images.githubusercontent.com/6020549/110085501-8900c580-7dd4-11eb-9fb7-311c21800719.jpg)

# Receiver   
Serial printing goes to PA9.   
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
![STM32F103-CAN-ESP32](https://user-images.githubusercontent.com/6020549/88114386-b5255880-cbee-11ea-89b4-9b215413fc8a.jpg)

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

![STM32F103-CAN-RPI](https://user-images.githubusercontent.com/6020549/88116161-e1db6f00-cbf2-11ea-8892-0e2cc0728a94.jpg)



# Using PlatformIO   
```
git clone https://github.com/nopnop2002/Arduino-STM32-CAN
cd Arduino-STM32-CAN/stm32f103
pio init --board bluepill_f103c8
cp stm32f103.ino src/
pio run -t upload
```


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
- Stub length in CAN bus is too long.   
 Though the ISO11898 Standard specifies a maximum bus length of 40m and maximum stub length of 0.3m at 1Mbps.
 See [here](https://e2e.ti.com/support/interface-group/interface/f/interface-forum/378932/iso1050-can-bus-stub-length).   

# Reference

https://github.com/nopnop2002/Robotell-USB-CAN-Python

https://github.com/nopnop2002/esp-idf-CANBus-Monitor


