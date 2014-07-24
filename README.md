# SolarSurferCore

Embedded software core for the a robotic solar powered surfboard propelled by the [BlueRobotics Thruster-100](http://www.bluerobotics.com/thruster). 

## Hardware

The SolarSurferCore is designed to run on the [3DR APM2.6](https://store.3drobotics.com/products/apm-2-6-kit-1) with the 3DR UBlox GPS/Compass Unit. The drivers for the onboard hardware were derived from a number of internet sources and modified for use on this project.

The following hardware is used:
* Invensense MPU-6000 for compass tilt compensation
* UBlox GPS using binary communication
* HMC5883 compass
* [BLDC Monitor](https://github.com/rjehangir/bldc_monitor)
* TTL Serial camera
* [Rock Seven RockBLOCK](http://rockblock.rock7mobile.com/) for satellite communications

Sensors:
* Atlas Scientific pH sensor
* Water temperature sensor w/ one-wire interface
* Airmar WS-100WX wind/temperature/pressure sensor

## Wiring

Serial ports:
* APM UART0 USB/3DR Radio (auto-mux)
* APM UART1 UBlox GPS (through GPS plug on APM)
* APM UART2 RockBLOCK (connected to RX/TX/GND pin in the case)
* APM UART3 Unused
* SoftwareSerial0 (pins X,X) Airmar WS-100WX
* SoftwareSerial1 (pins X,X) Atlas Scientific pH Sensor
* SoftwareSerial2 (pins X,X) Adafruit TTL Serial Camera
* SoftwareSerial3 (pins X,X) BLDC Monitor
* SoftwareSerial4 (pins X,X) Power Monitor

## Compilation

```bash
mkdir build
cd build
cmake .. -i
cmake ..
make
make program
```

## Diagnostic Printing

```bash
cd diagnostic
python diagnostic -p /dev/ttyUSBXXXX -o filename.csv
```

## Change History

### v0.1.0 - Used on Ocean Test No. 2

* Initial release



