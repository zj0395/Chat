#!/bin/bash
# configs
readonly BUILDTYPE=debug # debug, release
readonly CHATROOT=$(cd `dirname $0`;pwd)
readonly OUTDIR=$CHATROOT/cmake-build-$BUILDTYPE
readonly BINDIR=$OUTDIR/bin
readonly LIBDIR=$OUTDIR/lib
readonly INCLUDEDIR=$OUTDIR/include

# defines
readonly COPY_H=1

function local_mkdir() {
    if [[ $# -lt 1 ]];then
        echo "---Error call local_mkdir $*"
        return
    fi
    if [[ ! -d "$DST_DIR" ]];then
        mkdir -p "$DST_DIR"
    fi
}

function local_copy_header() {
    if [[ $# -lt 2 ]];then
        echo "---Error call copy_header $*"
        return
    fi
    local SRC_DIR=$1
    local DST_DIR=$2
    local_mkdir "$DST_DIR"
    if [[ $COPY_H -eq 1 ]];then
        cp "$SRC_DIR"/include/* "$DST_DIR"/ -r
    fi
}

function local_copy_library() {
    if [[ $# -lt 2 ]];then
        echo "---Error call copy_library $*"
        return
    fi
    local SRC_DIR=$1
    local DST_DIR=$2
    local_mkdir "$DST_DIR"
    for file in "$SRC_DIR"/lib*.a "$SRC_DIR"/lib*.so
    do
        if [ -e "$file" ];then
            cp "$file" "$DST_DIR/"
        fi
    done
}

function build_spdlog() {
    local SRC_DIR=$CHATROOT/extern/spdlog
    local OUT_DIR=$OUTDIR/extern/spdlog
    local myDefines=""

    local_copy_header "$SRC_DIR" "$INCLUDEDIR"
}

function build_fmt() {
    local SRC_DIR=$CHATROOT/extern/fmt
    local OUT_DIR=$OUTDIR/extern/fmt
    local myDefines=""
    cmake -S "$SRC_DIR" -B "$OUT_DIR" $myDefines && cd "$OUT_DIR" && make

    local_copy_header "$SRC_DIR" "$INCLUDEDIR"
    local_copy_library "$OUT_DIR" "$LIBDIR"
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
    externs="fmt,spdlog,chat"
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
