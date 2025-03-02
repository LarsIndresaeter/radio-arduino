#!/bin/bash

export CXX=avr-g++
export CC=avr-gcc
export AS="avr-gcc"
export MCU=atmega328p
export BAUD=57600
export PROG_TYPE=avrispmkII
export F_CPU=16000000UL

conan editable add arduino uart-avr/$(conan inspect -a version arduino | cut -d ':' -f2 | sed s'/ //g')@lars/test --layout=tools/conan/layout/arduino.txt
conan install -if build/arduino -pr:h tools/conan/profiles/gcc-avr -o RX_SLAVE=False arduino
conan build -bf build/arduino arduino
conan package -bf build/arduino arduino

conan install -if build/arduino_slave -pr:h tools/conan/profiles/gcc-avr -o RX_SLAVE=True arduino
conan build -bf build/arduino_slave arduino
conan package -bf build/arduino_slave arduino

