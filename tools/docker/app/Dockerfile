FROM ubuntu:22.04

RUN apt-get update && apt-get install -y --only-upgrade libstdc++6
RUN apt-get update && apt-get install -y avrdude
RUN apt-get update && apt-get install -y libpaho-mqtt1.3
RUN apt-get update && apt-get install -y openssl

COPY bin/* /bin/
COPY tools/docker/app/raduino-flash.sh /bin/

RUN useradd -ms /bin/bash raduino

RUN echo 'echo ""' >> /home/raduino/.bashrc
RUN echo 'echo "raduino dockerapp"' >> /home/raduino/.bashrc
RUN echo 'echo "           commands :"' >> /home/raduino/.bashrc
RUN echo 'echo "   raduino-flash.sh : flash arduino"' >> /home/raduino/.bashrc
RUN echo 'echo "raduino-personalize : configure device"' >> /home/raduino/.bashrc
RUN echo 'echo " aduino-mqtt-client : integration with mqtt broker"' >> /home/raduino/.bashrc
RUN echo 'echo "    raduino-gateway : communicate with gateway"' >> /home/raduino/.bashrc
RUN echo 'echo "       raduino-node : communicate with node"' >> /home/raduino/.bashrc
RUN echo 'echo ""' >> /home/raduino/.bashrc

USER raduino
