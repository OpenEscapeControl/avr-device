Requirements: arduino-mk

How to build for device 1?
$ cd firmware
$ ./build.sh 1 1

How to build for devices 1-15?
$ cd firmware
$ ./build.sh 1 15

How to install to Arduino?
1. Link EscapeControl to sketchbook/libraries
2. Add device_n.cpp file to EscapeControl/examples/EscapeControl with contents:
int device_n = 1; // for device 1
3. Open Arduino IDE -> examples -> EscapeControl
