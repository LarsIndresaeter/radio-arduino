# naming conventions

## project name

The name `radio-arduino` was selected to be a generic name. Currently only the nrf24l01 radio module is supported but the RadioUart class is designed with though that a variant can be made for a LoRa chip.

## file names

* C++ files have names reflecting the class name
* C++ source files have the .cpp extension
* C++ header files have the .hpp extension
* auto generated C++ header files have the .hxx extensions
* hyphens are used instead of underscore

## variable names

* camelCase is preferred over *snake_case*

## commit messages

In this repo we try to follow [conventional commits](https://www.conventionalcommits.org/en/v1.0.0/)

Commits messages should be imperative and answer the question: "if applied this commit will ..."

