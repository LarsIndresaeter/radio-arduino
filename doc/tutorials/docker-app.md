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

if this fails then find an online tutorial and follow the steps until you can successfully run the `docker run hello-world` command.

## give user access so serial device

add user to dialout group

```console
sudo usermod -aG dialout $USER
newgrp
```

create file `/etc/udev/rules.d/60-ch340.rules`

```
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

## build docker app

```console
raduino dockerapp build
```

## run docker app

```console
raduino dockerapp run <tag>
```

if you do not supply a tag then a list of raduino docker apps will be listed

## loading an image from a tar file

```console
docker image load -i raduino-0.2.0.tar
```

## running a docker app without the toolkit

if you have obtained the docker image and do not want to run the raduino toolkit

```console
docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash
```

run a command

```console
docker run -it --rm --device=/dev/ttyUSB0 raduino:0.2.0 bash
lars@332c4a94182b:/$ raduino-gateway -z
GET_DEVICE_NAME        :  nameString="rauino-gateway"
lars@332c4a94182b:/$ exit
exit
```

