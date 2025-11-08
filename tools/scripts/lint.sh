
#!/bin/bash

PARAM=$1

REPO_BASE_DIR=$(git rev-parse --show-toplevel)

if [ "${PARAM}" == "doc" ]
then
    echo "lint rst: rst-lint ${REPO_BASE_DIR}"

    mdl ${REPO_BASE_DIR}
fi

