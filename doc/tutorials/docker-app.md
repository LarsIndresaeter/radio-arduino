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

## pull image from dockerhub

```console
docker pull lars32/radio-arduino:1.2.0
```

if you do not supply a tag then a list of raduino docker apps will be listed

### devbox

devbox is an acronym for building and running the dockerapp.

```console
cd radio-arduino/
./tools/devbox.sh build
./tools/devbox.sh run
```

## running a docker app without cloning this repository

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

