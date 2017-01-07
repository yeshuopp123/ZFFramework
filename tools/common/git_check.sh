#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
PROJ_GIT=$1
DST_PATH=$2
if test "x$PROJ_GIT" = "x" || test "x$DST_PATH" = "x" ; then
    echo usage:
    echo   git_check.sh PROJ_GIT DST_PATH
    exit 1
fi

_OLD_DIR=$(pwd)
_TIMEOUT=86400
_GIT_VALID=0

if test -e "$DST_PATH/.git"; then
    _GIT_PATH_DESIRED="$DST_PATH/.git"
    cd "$_GIT_PATH_DESIRED" >/dev/null 2>&1
    _GIT_PATH_DESIRED=$(pwd -P)
    cd "$_OLD_DIR"

    cd "$DST_PATH" >/dev/null 2>&1
    _GIT_PATH_EXIST=`git rev-parse --show-toplevel 2>/dev/null`/.git
    cd "$_OLD_DIR"
    cd "$_GIT_PATH_EXIST" >/dev/null 2>&1
    _GIT_PATH_EXIST=$(pwd)
    cd "$_OLD_DIR"

    if test "$_GIT_PATH_DESIRED" = "$_GIT_PATH_EXIST" ; then
        _GIT_VALID=1
    fi
fi

if test "$_GIT_VALID" = "1"; then
    sh "$WORK_DIR/timestamp_check.sh" "$DST_PATH/.git" $_TIMEOUT

    if test ! "$?" = "0" ; then
        cd "$DST_PATH"
        git checkout .
        git reset --hard
        git clean -xdf
        git pull
        _SUCCESS="$?"
        cd "$_OLD_DIR"

        if test "$_SUCCESS" = "0"; then
            sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" $_TIMEOUT
        fi
    fi
else
    rm -rf "$DST_PATH" >/dev/null 2>&1
    mkdir -p "$DST_PATH" >/dev/null 2>&1
    git clone "$PROJ_GIT" "$DST_PATH"

    if test "$?" = "0"; then
        sh "$WORK_DIR/timestamp_save.sh" "$DST_PATH/.git" $_TIMEOUT
    fi
fi

