#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../..
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

if ! test -e "$ZF_ROOT_PATH/_release/iOS/all/lib"; then
    sh "$ZF_TOOLS_PATH/release/release_iOS.sh"
fi

