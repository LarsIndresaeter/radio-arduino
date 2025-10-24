#!/bin/bash

export CXX=avr-g++
export CC=avr-gcc
export AS="avr-gcc"
export MCU=atmega328p
export BAUD=57600
export PROG_TYPE=avrispmkII
export F_CPU=16000000UL

conan editable add arduino raduino-avr/$(conan inspect -a version arduino | cut -d ':' -f2 | sed s'/ //g')@raduino/test --layout=tools/conan/layout/arduino.txt
conan install -if build/arduino -pr:h tools/conan/profiles/gcc-avr arduino
conan build -bf build/arduino arduino
conan package -bf build/arduino arduino

