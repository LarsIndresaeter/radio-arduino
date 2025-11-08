#!/bin/bash

conan editable add interface/test_package raduino-api-test/0.1.0@raduino/test
conan install -if build/interface_test interface/test_package
conan build -bf build/interface_test interface/test_package

