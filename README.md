# audio-device-muter
## Command Specifier
- list -l 
- mute -m
- umute -u
- observe -o

## Device Specifier
- default -d
- all -a
- single device {id}

## Running Console Application with Command Line Arguments
- .\audio-device-muter.exe [Command Specifier] [Device Specifier]

## Command Examples
- List all audio input devices: .\audio-device-muter.exe -l -a
- Mute default audio input device: .\audio-device-muter.exe -m -d
- Observe state changes for single device: .\audio-device-muter.exe -o {ID}

## Remarks
- The device ID can be obtained by running a list command, which will print the device ID.
