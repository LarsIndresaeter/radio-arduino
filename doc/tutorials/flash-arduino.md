# flash arduino

## add your user to the dialout group

On linux the user need to be in the dialout group to use the serial port

```console
sudo usermod -a -G dialout $USER
```

## add udev rules

create the file `/etc/udev/rules.d/99-FTDI.rules`

```console
ACTION=="add", SUBSYSTEM=="usb", ATTRS{idVendor}=="0403", ATTRS={idProduct}=="6001", OWNER="user", MODE="0777", GROUP="dialout"
```

reload udev rules

```console
sudo udevadm control --reload
```

## setup tools

before you can use the provided `raduino` tool you must source the environment
tool. This has to be done for each terminal window where you want to use the
`raduino` tool.

```console
source tools/setup.sh
```

## flash using docker

if you are using docker to build and flash then you might need to change access
rights to the serial device

```console
sudo chmod a+rw /dev/ttyUSB0
./tools/devbox.sh
```

## flash command

The command for flashing is `raduino flash <type> <device>`. Use the provided
help menu to learn more. For example an arduino nano-pro as a `gateway` can be
programmed using this command. type can be either rf-nano or nano-pro.

```console
raduino flash nano-pro gateway
```

