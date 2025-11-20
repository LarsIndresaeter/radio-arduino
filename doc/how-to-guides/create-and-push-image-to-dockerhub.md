# create and push docker image

```console
cd radio-arduino/
./tools/devbox.sh 
./tools/devbox.sh clean
./tools/devbox.sh build
./tools/devbox.sh run
```

inside the devbox

```console
raduino all
exit
```

you have now built the binaries, time to make the docker app

```console
source tools/setup.sh 
raduino dockerapp build
```

and finally push it to dockerhub

```console
docker login -u lars32
docker tag radio-arduino:1.2.0 lars32/radio-arduino:1.2.0
docker push lars32/radio-arduino:1.2.0
```

