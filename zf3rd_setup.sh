#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZF_ROOT_PATH=$WORK_DIR
ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools

sh "$ZF_TOOLS_PATH/common/zfsh_recursive.sh" "$ZF_ROOT_PATH" "zfautoscript_zf3rd_setup*.zfsh"

