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
* Temperature sensor w/ one-wire interface

## Compilation

'''bash
mkdir build
cd build
cmake .. -i
cmake ..
make
make program
'''

## Diagnostic Printing

'''bash
cd diagnostic
python diagnostic -o filename.csv
'''

## Change History

### v0.1.0 - tbd

* Initial release



