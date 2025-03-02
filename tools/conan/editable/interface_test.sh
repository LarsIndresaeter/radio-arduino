#!/bin/bash

conan editable add interface/test_package uart-api-test/0.0.1@lars/test
conan install -if build/interface_test interface/test_package
conan build -bf build/interface_test interface/test_package

