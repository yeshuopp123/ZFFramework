#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
RES_PATH=$1
if test "x$RES_PATH" = "x" ; then
    echo usage:
    echo   res_fix.sh RES_PATH
    exit 1
fi

_res_fix_action() {
    filename=$(basename "$1")
    fileextension=${filename##*.}
    if test ! "$fileextension" = "mp2" ; then
        rm $1.mp2 >/dev/null 2>&1
        mv $1 $1.mp2 >/dev/null 2>&1
    fi
}
export -f _res_fix_action

find "$RES_PATH" -type f 2>/dev/null | xargs -i bash -c '_res_fix_action $@' _ {} >/dev/null 2>&1

