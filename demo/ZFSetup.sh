#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR/../../ZFFramework
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
PROJ_ROOT=$WORK_DIR/..

if ! test -e "$ZF_TOOLS_PATH"; then
    rm -rf "$ZF_ROOT_PATH" >/dev/null 2>&1
    git clone "https://github.com/ZFFramework/ZFFramework.git" "$ZF_ROOT_PATH"
fi

