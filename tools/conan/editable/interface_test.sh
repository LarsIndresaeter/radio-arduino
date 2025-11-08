#!/bin/bash

conan editable add interface/test_package raduino-api-test/$(conan inspect -a version interface | cut -d ':' -f2 | sed s'/ //g')@raduino/test
conan install -if build/interface_test interface/test_package
conan build -bf build/interface_test interface/test_package

