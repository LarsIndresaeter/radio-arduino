# Changelog

## notable changes on main branch after last release

- new cli tool raduino-mqtt-bridge which receive commands in json formt and
  publish response from raduino nodes in json format
- new service raduino-mqtt-proxy which act as a proxy for multiple gateways.
  This micro service is for use in a container environment.

## version 2.0.1

- bugfix: initialize gateway in listening for detection of broadcast packages

## version 2.0.0

- new link level commands for discovery and wakeup
- new addressing scheme for radio nodes based on unique id in eeprom
- discovery of new radio nodes
- support for communication with multiple nodes

## Version 1.9.0

- add start script to container
- upgrade gateway automatically if old version detected

## Version 1.8.0

- refactor random generator to use 106 bytes less static ram on arduino
- refactor radio-uart to use 32 bytes less static ram on arduino

## Version 1.7.0

- Build docker images using github actions
- use ghcr.io instead of dockerhub.io for storing docker images

## Version 1.6.0

- publish more data on mqtt
- refactor keep alive behavior

## Version 1.5.0

- add new commands for attached devices
- new command get_lsm303d
- change name of dockerapp image

## Version 1.2.0

- fix doc generator
- option for transport encryption in raduino-test
- refactor docker scripts and containers

## Version 1.1.0

- refactor code generation scripts

## Version 1.0.0

Tag repo after making the repository public

