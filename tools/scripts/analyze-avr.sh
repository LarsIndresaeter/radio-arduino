#!/usr/bin/env bash

grep debug_ranges build/arduino/src/arduino.map

avr-size --mcu=atmega328p --format=avr build/arduino/bin/arduino.elf

