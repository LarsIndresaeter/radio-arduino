#!/bin/bash

if [ -f /.dockerenv ];
then
    echo "build tool is running inside docker"
else
    echo "using local tools (not docker)"
fi
