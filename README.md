# audio-device-muter
# Command Specifier
- list -l 
- mute -m
- umute -u
- obersve -o

# Device Specifier
- default -d
- all -a
- single device {id}

# Running Console Application with command line arguements
- .\audio-device-muter.exe [Command Specifier] [Device Specifier]

# Examples
- List all audio input devices: .\audio-device-muter.exe -l -a
- Mute default audio input device: .\audio-device-muter.exe -m -d
- Observe state changes for single device: .\audio-device-muter.exe -o {ID}

# Remarks
- The device ID can be obtained by running a list command, which will print the device ID.
