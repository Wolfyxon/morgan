# Morgan
A tool for using (computer) keyboards as MIDI inputs with different octaves.

## Support
Currently, Morgan is fully supported on Linux X11/Xorg. Wayland is not supported at the moment.

Windows is supported, but may be unstable.

## Command line usage
**Note**: All flags have a short version, for example: `--help` == `-h`.
### Normal run
Simply run the program and it will open a MIDI channel and start listening for inputs.
```
morgan
```
### Setup
On the first run, you'll be met with a configuration wizard asking you to press keys on the keyboards you'd like to confugre.
You can redo this setup using.
```
morgan --setup
```
**NOTE:** Running the setup again will overwrite your existing configuration!

### Listing keyboards & their IDs
To list keyboards and their IDs for configuration, use
```
morgan --keyboards
```
### Overriding config
You can use a different config path than default by using the `--config` flag
```
morgan --config path/to/your/config.ini
```

## Configuration
The config is stored at:  
- Linux: `~/.config/morgan/morgan.conf`
- Windows `%AppData%\morgan\morgan.conf`

It uses the ini format.

### Configuring keyboards
To configure a keyboard, add:
```ini
[keyboard_<id>]
octave = <octave numbner>
```
For example:
```ini
[keyboard_3]
octave = 4
```

You can view keyboard IDs by using
```
morgan --keyboards
```

## Building & testing
Install `make` and `gcc`.

### Your current platform
Build
```
make
```
Test
```
make test
```

### Linux
Build
```
make linux
```
Test
```
make test-linux
```

### Windows
Build
```
make windows
```
Test (NOTE: If you're testing on Linux, you will need `wine`)
```
make test-windows
```

### All platforms
Build
```
make all
```
Test
```
make test-all
```
