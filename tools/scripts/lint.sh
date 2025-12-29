
#!/bin/bash

PARAM=$1

REPO_BASE_DIR=$(git rev-parse --show-toplevel)

echo "lint markdown: mdl ${REPO_BASE_DIR}"

mdl ${REPO_BASE_DIR}

