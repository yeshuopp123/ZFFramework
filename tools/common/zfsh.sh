#!bash

WORK_DIR=$(cd "$(dirname "$0")"; pwd)
ZFSH_PATH=$1
if test "x$ZFSH_PATH" = "x" ; then
    echo usage:
    echo   zfsh.sh ZFSH_PATH
    exit 1
fi

ZF_ROOT_PATH=$WORK_DIR/../..
ZFSH_WORK_DIR=${ZFSH_PATH%[/\\]*}
TMP_PATH=zfsh_tmp_${ZFSH_PATH##*[/\\]}.sh
TMP_PATH=$ZF_ROOT_PATH/_tmp/$TMP_PATH

rm -rf "$TMP_PATH" >/dev/null 2>&1
mkdir -p "${TMP_PATH%[/\\]*}" >/dev/null 2>&1

echo "#!bash">>"$TMP_PATH"
echo _ZF_P0=\$0>>"$TMP_PATH"
echo _ZF_P1=\$1>>"$TMP_PATH"
echo _ZF_P2=\$2>>"$TMP_PATH"
echo _ZF_P3=\$3>>"$TMP_PATH"
echo _ZF_P4=\$4>>"$TMP_PATH"
echo _ZF_P5=\$5>>"$TMP_PATH"
echo _ZF_P6=\$6>>"$TMP_PATH"
echo _ZF_P7=\$7>>"$TMP_PATH"
echo _ZF_P8=\$8>>"$TMP_PATH"

while IFS='' read -r line ; do
    line=${line//\\/\/}

    line=${line//<ZF_COMMENT>/#}
    line=${line//<ZF_SH>/sh}
    line=${line//<ZF_SH_EXT>/sh}

    line=${line//<ZF_ROOT_PATH>/$ZF_ROOT_PATH}
    line=${line//<ZF_WORK_DIR>/$ZFSH_WORK_DIR}

    line=${line//<ZF_P0>/\$\{_ZF_P0\}}
    line=${line//<ZF_P1>/\$\{_ZF_P1\}}
    line=${line//<ZF_P2>/\$\{_ZF_P2\}}
    line=${line//<ZF_P3>/\$\{_ZF_P3\}}
    line=${line//<ZF_P4>/\$\{_ZF_P4\}}
    line=${line//<ZF_P5>/\$\{_ZF_P5\}}
    line=${line//<ZF_P6>/\$\{_ZF_P6\}}
    line=${line//<ZF_P7>/\$\{_ZF_P7\}}
    line=${line//<ZF_P8>/\$\{_ZF_P8\}}

    line=${line//<ZF_SET>/}
    line=${line//<ZF_PL>/\$\{}
    line=${line//<ZF_PR>/\}}
    line=${line//<ZF_EXIT_SUCCESS>/exit 0}
    line=${line//<ZF_EXIT_FAILED>/exit 1}
    line=${line//<ZF_IF_SUCCESS_BEGIN>/if test \"\$?\" = \"0\" ; then}
    line=${line//<ZF_IF_SUCCESS_END>/fi}
    line=${line//<ZF_IF_FAILED_BEGIN>/if test ! \"\$?\" = \"0\" ; then}
    line=${line//<ZF_IF_FAILED_END>/fi}

    echo $line>>"$TMP_PATH"
done < "$ZFSH_PATH"

sh "$TMP_PATH" "${@:2}"
RESULT=$?
rm -rf "$TMP_PATH" >/dev/null 2>&1
exit $RESULT

