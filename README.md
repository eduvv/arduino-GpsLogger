[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
# GPS Logger
A local, offline gps logger that writes gps location and meta data to an SD card.
## Components used
- 1x Arduino Uno Rev3 (ATmega328P)
- 1x [RobotDyn MicroSD-card Module](https://robotdyn.com/microsd-card-module.html)
- 2x 5mm 20mA LED
- 1x SanDisk Ultra 16GB MicroSD
- 1x GY-NEO6MV2 GPS Module
## Libraries used
This project used 2 libraries:
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus): used for requesting and converting NMEA data.
- [SD](https://github.com/arduino-libraries/SD): used to read/write to the SDcard module.
## Diagram
![Fritzing diagram of the project.](https://i.imgur.com/29PSno7.png "GPS Logger")