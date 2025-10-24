FROM ubuntu:22.04
RUN apt-get update && apt-get install -y build-essential
RUN apt-get update && apt-get install -y cmake
RUN apt-get update && apt-get install -y tree
RUN apt-get update && apt-get install -y vim

RUN apt-get update && apt-get install -y gcc-avr binutils-avr avr-libc avrdude

RUN apt-get install -y python3
RUN apt-get install -y python3-pip
RUN pip3 install conan==1.57
#RUN pip install conan==1.57.0 --break-system-packages

RUN useradd -ms /bin/bash lars

RUN apt-get install -y cmake ccache gcc-avr avr-libc avrdude
RUN apt-get update && apt-get install -y git
RUN apt-get update && apt-get install -y bc

USER lars
WORKDIR /home/lars/
