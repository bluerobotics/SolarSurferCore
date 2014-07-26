[![Build Status](https://travis-ci.org/bluerobotics/SolarSurferCore.svg?branch=ino)](https://travis-ci.org/bluerobotics/SolarSurferCore)

# SolarSurferCore

Embedded software core for the a robotic solar powered surfboard propelled by the [BlueRobotics Thruster-100](http://www.bluerobotics.com/thruster).

## Overview

The SolarSurferCore is designed to run on the [3DR APM2.6](https://store.3drobotics.com/products/apm-2-6-kit-1) with the 3DR UBlox GPS/Compass Unit. The drivers for the onboard hardware were derived from a number of Internet sources and modified for use on this project.

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

Serial port assignments:

* APM UART0 USB/3DR Radio (auto-mux)
* APM UART1 UBlox GPS (through GPS plug on APM)
* APM UART2 RockBLOCK (connected to RX/TX/GND pin in the case)
* APM UART3 Unused
* SoftwareSerial0 (pins X,X) Airmar WS-100WX
* SoftwareSerial1 (pins X,X) Atlas Scientific pH Sensor
* SoftwareSerial2 (pins X,X) Adafruit TTL Serial Camera
* SoftwareSerial3 (pins X,X) BLDC Monitor
* SoftwareSerial4 (pins X,X) Power Monitor

## Setup

This project use [ino](https://github.com/amperka/ino) to compile and upload code. To 

```bash
git clone https://github.com/bluerobotics/SolarSurferCore.git
scripts/install-linux.sh
# or
scripts/install-mac.sh
```

You'll also need to cross-compile the latest message format from the [SolarSurferMessage](https://github.com/bluerobotics/SolarSurferMessage) project. Do that with the update script:

```bash
./scripts/update.sh
```

Once everything is ready to go, building the project is as simple as:

```bash
ino build
ino upload -p /dev/ttyACM1
ino serial -p /dev/ttyACM1
```

## Diagnostic Printing

```bash
cd diagnostic
python diagnostic -p /dev/ttyUSBXXXX -o filename.csv
```

## Change History

This project uses [semantic versioning](http://semver.org/).

### v0.1.0 - 2014/07/17

* Initial release
* Used on Ocean Test No. 2
