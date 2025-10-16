#!/bin/bash

# this files is executed inside the docker container

. tools/scripts/export_environment.sh

tools/suite.sh $@

echo ""
echo "done"
