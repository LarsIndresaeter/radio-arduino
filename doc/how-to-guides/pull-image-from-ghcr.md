# pull image from dockerhub and send commands to gateway

pull image

```console
docker pull ghcr.io/larsindresaeter/radio-arduino:1.7.0
```

give access to serial port

```console
sudo chmod a+rw /dev/ttyUSB0
```

start docker app

```console
docker run -it --rm --device=/dev/ttyUSB0 ghcr.io/larsindresaeter/raduino:1.7.0 bash
```

ping

```console
./bin/raduino-system-commands -p
```

