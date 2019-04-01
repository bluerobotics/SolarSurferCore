[![Build Status](https://travis-ci.org/bluerobotics/SolarSurferCore.svg?branch=ino)](https://travis-ci.org/bluerobotics/SolarSurferCore)

# NOTICE: This project is no longer maintained.
Blue Robotics has chosen to retire this project, it is no longer maintained or supported. Bug reports and feature requests are no longer accepted.  The repository has been archived and made read-only. The source is still available and can be forked, but this project may be permanently removed from GitHub in the future. For questions and further information, check out the Blue Robotics [discussion forums](https://discuss.bluerobotics.com) and [Gitter channel](https://gitter.im/bluerobotics/discussion).

# SolarSurferCore

Embedded software core for the a robotic solar powered surfboard propelled by the [BlueRobotics Thruster-100](http://www.bluerobotics.com/thruster).

## Overview

The SolarSurferCore is designed to run on the [3DR APM2.6](https://store.3drobotics.com/products/apm-2-6-kit-1) with the 3DR UBlox GPS/Compass Unit. The drivers for the onboard hardware were derived from a number of Internet sources and modified for use on this project.

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

## Dependencies

This project requires several Arduino libraries that are not included in the standard Arduino distribution. These are:

* TinyGPS++ for NMEA-0183 communication with the wind sensor
* NewSoftSerial (if Arduino Version < 1.0)

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

## Building with CMake

The project can also be built using CMake.

```bash
git clone http://github.com/bluerobotics/SolarSurferCore.git
cd SolarSurferCore
mkdir build
cmake -i ../src/
cmake ../src/
make
make program
```

Any .cpp files that are added to the project must be listed in the CMakeLists.txt file in the src/ directory.

## Diagnostic Printing

```bash
cd scripts
python diagnostic.py -p /dev/ttyUSBXXXX -o filename.csv
```

## Change History

This project uses [semantic versioning](http://semver.org/).

### v0.2.0 - 2014/08/08

* Fully functional navigation and communication.
* Used on Santa Monica Canyon Test.

### v0.1.0 - 2014/07/17

* Initial release
* Used on Ocean Test No. 2
