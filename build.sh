#!/bin/bash
readonly BUILDTYPE=debug # debug, release
readonly CHATROOT=$(cd `dirname $0`;pwd)
readonly OUTDIR=$CHATROOT/cmake-build-$BUILDTYPE
readonly BINDIR=$OUTDIR/bin
readonly LIBDIR=$OUTDIR/lib

function build_fmt() {
    local FMT_DIR=$CHATROOT/extern/fmt
    cd "$FMT_DIR"
    local SRC_DIR=$FMT_DIR
    local OUT_DIR=$OUTDIR/extern/fmt
    local myDefines=""
    cmake -S "$SRC_DIR" -B "$OUT_DIR" $myDefines && cd "$OUT_DIR" && make
}

function build_chat() {
    if [[ ! -d $CHATROOT ]];then
        echo Unknown Wrong.
        exit
    fi

    local SRC_DIR=$CHATROOT
    local OUT_DIR=$OUTDIR
    local myDefines="-DCMAKE_BUILD_TYPE=$BUILDTYPE -DEXECUTABLE_OUTPUT_PATH=$BINDIR -DLIBRARY_OUTPUT_PATH=$LIBDIR"
    cmake -S "$SRC_DIR" -B "$OUT_DIR" $myDefines && cd "$OUT_DIR" && make

    success=$?
    if [ $success -eq 0 ];then
        echo Build Success
    else
        echo Build Fail
        exit
    fi
}

function build_all() {
    externs="chat,fmt"
    OLD_IFS="$IFS"
    IFS=","
    externs=($externs)
    IFS="$OLD_IFS" # if you miss this line, you may find a strange bug
    for name in "${externs[@]}"
    do
        build_$name
    done
}

build_all
