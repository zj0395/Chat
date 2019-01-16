#!/bin/bash
# configs
readonly g_BUILD_TYPE=debug # debug, release
readonly g_CHAT_ROOT=$(cd `dirname $0`;pwd)
readonly g_OUT_DIR=$g_CHAT_ROOT/cmake-build-$g_BUILD_TYPE
readonly g_BIN_DIR=$g_OUT_DIR/bin
readonly g_LIB_DIR=$g_OUT_DIR/lib
readonly g_INCLUDE_DIR=$g_OUT_DIR/include

function local_mkdir() {
    if [[ $# -lt 1 ]];then
        echo "---Error call local_mkdir $*"
        return
    fi
    local DST_DIR=$1
    mkdir -p "$DST_DIR"
}

function local_copy_header() {
    if [[ $# -lt 2 ]];then
        echo "---Error call copy_header $*"
        return
    fi
    local SRC_DIR=$1
    local DST_DIR=$2
    local_mkdir "$DST_DIR"
    cp "$SRC_DIR"/include/* "$DST_DIR"/ -r
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
    local SRC_DIR=$g_CHAT_ROOT/extern/spdlog
    local OUT_DIR=$g_OUT_DIR/extern/spdlog
    local myDefines=""

    local_copy_header "$SRC_DIR" "$g_INCLUDE_DIR"
}

function build_fmt() {
    local SRC_DIR=$g_CHAT_ROOT/extern/fmt
    local OUT_DIR=$g_OUT_DIR/extern/fmt
    local myDefines=""
    local_mkdir "$OUT_DIR"
    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make fmt

    local_copy_header "$SRC_DIR" "$g_INCLUDE_DIR"
    local_copy_library "$OUT_DIR" "$g_LIB_DIR"
}

function build_chat() {
    if [[ ! -d $g_CHAT_ROOT ]];then
        echo Unknown Wrong.
        exit 1
    fi

    local SRC_DIR=$g_CHAT_ROOT
    local OUT_DIR=$g_OUT_DIR
    local myDefines="-DCMAKE_BUILD_TYPE=$g_BUILD_TYPE -DEXECUTABLE_OUTPUT_PATH=$g_BIN_DIR -DLIBRARY_OUTPUT_PATH=$g_LIB_DIR -DMY_INCLUDE_PATH=$g_INCLUDE_DIR"
    local_mkdir $OUT_DIR
    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make

    success=$?
    if [ $success -eq 0 ];then
        echo Build Success
    else
        echo Build Fail
        exit 1
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
