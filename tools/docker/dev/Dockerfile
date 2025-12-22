FROM ubuntu:22.04

RUN apt-get update && apt-get install -y build-essential cmake ccache 
RUN apt-get update && apt-get install -y gcc-avr binutils-avr avr-libc avrdude
RUN apt-get update && apt-get install -y ninja-build
RUN apt-get update && apt-get install -y git vim bc
RUN apt-get update && apt-get install -y python3 python3-pip
RUN apt-get update && apt-get install -y libpaho-mqtt1.3
RUN apt-get update && apt-get install -y openssl

RUN pip3 install conan==1.57

ENV CONAN_USER_HOME="/home/raduino/"
ENV HOME="/home/raduino/"

RUN useradd -ms /bin/bash raduino
USER raduino

WORKDIR /home/raduino/


