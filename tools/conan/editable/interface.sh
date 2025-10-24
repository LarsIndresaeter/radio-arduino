#!/bin/bash

conan editable add interface raduino-api/0.0.1@raduino/test --layout=tools/conan/layout/interface.txt

conan install -if build/interface interface
conan build -bf build/interface interface
conan package -bf build/interface interface

