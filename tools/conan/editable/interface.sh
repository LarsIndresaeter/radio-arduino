#!/bin/bash

export CONAN_USER_HOME=~/priv/

conan editable add interface uart-api/0.0.1@lars/test --layout=tools/conan/layout/interface.txt

conan install -if build/interface interface
conan build -bf build/interface interface
conan package -bf build/interface interface

