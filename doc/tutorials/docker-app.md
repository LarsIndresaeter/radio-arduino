# docker app

- prerequisite: [install docker](../how-to-guides/install-docker.md)

## pull image from dockerhub

```console
docker pull lars32/radio-arduino:1.2.0
```

if you do not supply a tag then a list of raduino docker apps will be listed

## running a docker app without cloning this repository

if you have obtained the docker image and do not want to run the raduino toolkit

```console
docker run -it --rm --device=/dev/ttyUSB0 radio-arduino:1.2.0 bash
```

## enter docker app and use toolkit

```console
docker run -it --rm --device=/dev/ttyUSB0 radio-arduino:1.2.0 bash
lars@332c4a94182b:/$ raduino-gateway -z
GET_DEVICE_NAME        :  nameString="rauino-gateway"
lars@332c4a94182b:/$ exit
exit
```

