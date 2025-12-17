#!/bin/bash

ACTION=$1
PARAM=$2

TAG_LATEST=$(git describe --tags --abbrev=0)
TAG_LATEST_MAJOR=$(echo ${TAG_LATEST} | cut -d '.' -f1)
TAG_LATEST_MINOR=$(echo ${TAG_LATEST} | cut -d '.' -f2)
TAG_LATEST_PATCH=$(echo ${TAG_LATEST} | cut -d '.' -f3)

BUMBED_LATEST_MAJOR=$(echo ${TAG_LATEST_MAJOR} + 1 | bc)
BUMBED_LATEST_MINOR=$(echo ${TAG_LATEST_MINOR} + 1 | bc)
BUMBED_LATEST_PATCH=$(echo ${TAG_LATEST_PATCH} + 1 | bc)

NEW_TAG=""

if [ "${ACTION}" == "" ] 
then
    echo "no action provided"
    echo "raduino release <action> :"
    echo "           bump <number> : bump number <major|minor|patch> and create tag"
    echo "                    push : push latest tag to origin"
    echo "                  detect : detect change but do not create tag"
    echo "             detect auto : detect change and create tag"
    echo "                  latest : print latest tag"
    echo "                  create : create release"
    exit 1
fi

if [ "${ACTION}" == "detect" ] 
then
    COUNT_BREAKING=$(git log ${TAG_LATEST}..HEAD --oneline --pretty=format:%s | grep -c "^BREAKING")
    COUNT_FEAT=$(git log ${TAG_LATEST}..HEAD --oneline --pretty=format:%s | grep -c "^feat")
    COUNT_FIX=$(git log ${TAG_LATEST}..HEAD --oneline --pretty=format:%s | grep -c "^fix")

    if [ ${COUNT_BREAKING} -gt 0 ]
    then
        echo "${COUNT_BREAKING} breaking change: commits. Bump major!"
        if [ "${PARAM}" == "auto" ] 
        then
            ACTION="bump"
            PARAM="major"
        fi
        echo "raduino release bump major"
    elif [ ${COUNT_FEAT} -gt 0 ]
    then
        echo "${COUNT_FEAT} new feat: commits. Bump minor!"
        if [ "${PARAM}" == "auto" ] 
        then
            ACTION="bump"
            PARAM="minor"
        fi
        echo "raduino release bump minor"
    elif [ ${COUNT_FIX} -gt 0 ]
    then
        echo "${COUNT_FIX} new fix: commits. Bump patch!"
        if [ "${PARAM}" == "auto" ] 
        then
            ACTION="bump"
            PARAM="patch"
        fi
        echo "raduino release bump patch"
    fi
fi

if [ "${ACTION}" == "bump" ] 
then
    if [ "${PARAM}" == "major" ] 
    then
        NEW_TAG=$(echo ${BUMBED_LATEST_MAJOR}.0.0)
    elif [ "${PARAM}" == "minor" ] 
    then
        NEW_TAG=$(echo ${TAG_LATEST_MAJOR}.${BUMBED_LATEST_MINOR}.0)
    elif [ "${PARAM}" == "patch" ] 
    then
        NEW_TAG=$(echo ${TAG_LATEST_MAJOR}.${TAG_LATEST_MINOR}.${BUMBED_LATEST_PATCH})
    fi

    if [ "${NEW_TAG}" != "" ] 
    then
        echo "new tag: ${NEW_TAG}"
        git tag -a "${NEW_TAG}" -m "${NEW_TAG}"
    fi
elif [ "${ACTION}" == "push" ] 
then
    git push origin ${TAG_LATEST}
elif [ "${ACTION}" == "latest" ] 
then
    echo "$TAG_LATEST"
elif [ "${ACTION}" == "create" ]
then
    echo "Steps:"
    echo "  1. ./tools/devbox.sh run"
    echo "  2. raduino all"
    echo "  3. exit"
    echo "  3. raduino dockerapp build"
    echo "  4. docker login -u lars32"
    echo "  5. docker tag radio-arduino:${TAG_LATEST} lars32/radio-arduino:${TAG_LATEST}"
    echo "  6. docker push lars32/radio-arduino:${TAG_LATEST}"
fi

