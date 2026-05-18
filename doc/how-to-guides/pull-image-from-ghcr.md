# pull image from dockerhub and send commands to gateway

Pull image

```console
docker pull ghcr.io/larsindresaeter/radio-arduino:1.7.0
```

Give access to serial port

```console
sudo chmod a+rw /dev/ttyUSB0
```

Start docker app

```console
docker run -it --rm --device=/dev/ttyUSB0 ghcr.io/larsindresaeter/raduino:latest bash
```

Ping

```console
./bin/raduino-system-commands -p
```

