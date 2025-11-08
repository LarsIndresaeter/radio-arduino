#!/bin/bash

conan editable add interface raduino-api/$(conan inspect -a version interface | cut -d ':' -f2 | sed s'/ //g')@raduino/test --layout=tools/conan/layout/interface.txt

conan install -if build/interface interface
conan build -bf build/interface interface
conan package -bf build/interface interface

