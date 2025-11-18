# develop on raspberrypi

tested on Raspberry Pi 3, os: Debian 12

## install docker

```console
sudo apt-get install docker.io
sudo usermod -aG docker $USER
newgrp
sudo shutdown -r now
```

## build with devbox

```console
cd radio-arduino
./tools/devbox.sh build
```

## access to serial port

```console
sudo usermod -aG dialout $USER
```

- create file `/etc/udev/rules.d/60-ch340.rules`

```text
ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", ENV{ID_MM_DEVICE_MANUAL_SCAN_ONLY}="1"
SUBSYSTEM=="dialout", MODE="0666", GROUP="dialout", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523"
```

```console
sudo udevadm control --reload
```

