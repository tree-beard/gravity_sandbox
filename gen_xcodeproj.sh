#!/bin/bash

SCRIPTDIR=$(dirname "$0")
DIRBUILD=$SCRIPTDIR"/build"

if [ ! -d "$DIRBUILD" ]
then
    echo "create directory build…"
    mkdir "$DIRBUILD"
fi

cd "$DIRBUILD" || exit 1
echo "begin create Xcode project…"

cmake -G Xcode \
      -DCMAKE_BUILD_TYPE=Debug \
      "$@" \
      ..
echo "end create Xcode project."

exit 0
