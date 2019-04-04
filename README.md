# Actuator Control Board

## Dependecies
Install required packages  
- Debian (Ubuntu) `sudo apt-get install binutils-avr gcc-avr avr-libc avrdude`
- Arch `sudo pacman -S avr-binutils avr-gcc avr-libc avrdude`

## Compilation
The following command is required for compilation:
```
make TARGET=sensor_control_board
```
Replace `sensor_control_board` with any board from `./boards` directory!
  
Other imporant parameters you can use:
- `MCU` to change microcontroller (eg. `make TARGET=sensor_control_board MCU=at90can128`),
- `F_CPU` to change frequency (eg. `make TARGET=sensor_control_board F_CPU=16000000`),
- `CSTANDARD` to change C standard,
- and for more check `Makefile`.

## Flashing
```
sudo make program
``` 
By default it uses `usbasp` but you can change it to:
- **JTAG** by running `sudo make program AVRDUDE_PROGRAMMER=jtag1 AVRDUDE_PORT=/dev/ttyUSB0`
- and for other flashing methods check `Makefile`.

## Fuse bits
By default at90can128 uses internal oscillator (8MHz) and to configure external 
oscillator we have to configure fuse bits using command:
```
avrdude -p at90can128 -c usbasp -U lfuse:w:0x9F:m
```
