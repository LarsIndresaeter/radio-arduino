# docker app

## install docker (ubuntu 24.04)

```console
sudo apt-get install docker.io
sudo usermod -aG docker $USER
newgrp
```

log out and in again

```console
docker version
docker run hello-world
```

if this fails then find an online tutorial and follow the steps until you can
successfully run the `docker run hello-world` command.

## give user access so serial device

add user to dialout group

```console
sudo usermod -aG dialout $USER
newgrp
```

create file `/etc/udev/rules.d/60-ch340.rules`

```console
ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", ENV{ID_MM_DEVICE_MANUAL_SCAN_ONLY}="1"
SUBSYSTEM=="dialout", MODE="0666", GROUP="dialout", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523"
```

reload udev rules

```console
sudo udevadm control --reload
```

change access to device file

```console
sudo chmod a+rw /dev/ttyUSB0
```

