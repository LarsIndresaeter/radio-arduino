#!/bin/bash

# this files is executed inside the docker container

. tools/helper_scripts/export_environment.sh

tools/suite.sh $@

echo ""
echo "done"
