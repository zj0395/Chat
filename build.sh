#!/bin/bash
# configs
export g_CHAT_ROOT=$(cd `dirname $0`;pwd)

export g_BUILD_TYPE=debug # debug, release
export g_BUILD_PATHFORM=ubuntu18
export g_OUT_ROOT=$g_CHAT_ROOT/output/$g_BUILD_PATHFORM
export g_OUT_DIR=$g_OUT_ROOT/build-$g_BUILD_TYPE
export g_BIN_DIR=$g_OUT_DIR/bin
export g_LIB_DIR=$g_OUT_DIR/lib
export g_INCLUDE_DIR=$g_OUT_DIR/include
export g_PROTOBUF_OUT=$g_OUT_ROOT/extern/protobuf
export g_GTEST_OUT=$g_OUT_ROOT/extern/gtest

readonly TEST_EXE=chat_test
# defines
readonly g_COPY_HEADERS=0

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
    if [ $g_COPY_HEADERS -ne 1 ];then
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
    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make fmt -j3 && cd -

    local_copy_header "$SRC_DIR" "$g_INCLUDE_DIR"
    local_copy_library "$OUT_DIR" "$g_LIB_DIR"
}

function build_chat() {
    if [[ ! -d $g_CHAT_ROOT ]];then
        echo Unknown Wrong.
        exit 1
    fi

    local SRC_DIR=$g_CHAT_ROOT/src
    local OUT_DIR=$g_OUT_DIR
    local myDefines="-DCMAKE_BUILD_TYPE=$g_BUILD_TYPE -DEXECUTABLE_OUTPUT_PATH=$g_BIN_DIR -DLIBRARY_OUTPUT_PATH=$g_LIB_DIR \
        -DPROTOBUF_PATH=$g_PROTOBUF_OUT"
    local_mkdir $OUT_DIR
    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make && cd -

    success=$?
    if [ $success -eq 0 ];then
        echo Build Chat Success
    else
        echo Build Fail
        exit 1
    fi
}

function build_test() {
    clean_test
    local SRC_DIR=$g_CHAT_ROOT/test
    local OUT_DIR=$g_OUT_DIR/test
    local myDefines="-DEXECUTABLE_OUTPUT_PATH=$g_BIN_DIR"
    local_mkdir $OUT_DIR

    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make && cd -
    cd $g_BIN_DIR && ./"$TEST_EXE" && cd -

    success=$?
    if [ $success -eq 0 ];then
        echo Test Success
    else
        echo Test Fail
        exit 1
    fi
}

function build_all() {
    externs="fmt,spdlog,chat,test"
    OLD_IFS="$IFS"
    IFS=","
    externs=($externs)
    IFS="$OLD_IFS" # if you miss this line, you may find a strange bug
    for name in "${externs[@]}"
    do
        build_$name
    done
}

# 编译 gtest 库
function compile_gtest {
    local SRC_DIR=$g_CHAT_ROOT/extern/googletest
    local OUT_DIR=$g_GTEST_OUT
    local myDefines=""
    local_mkdir $OUT_DIR
    cd "$OUT_DIR" && cmake "$SRC_DIR" $myDefines && make && cd -
}

# 编译 protobuf 二进制可执行文件
function compile_protobuf {
    local TAR_FILE="$g_CHAT_ROOT"/extern/protobuf*.tar.gz
    if [[ -e $TAR_FILE ]];then
        echo No protobuf or more than one
        exit 1
    fi

    local TAR_DIR=$g_OUT_ROOT/tmp-protobuf
    local OUT_DIR=$g_PROTOBUF_OUT
    local_mkdir "$TAR_DIR"
    local_mkdir "$OUT_DIR"
    tar -xzvf $TAR_FILE -C $TAR_DIR

    local SRC_DIR="$TAR_DIR"/protobuf-*
    cd $SRC_DIR && ./configure --disable-shared --prefix="$OUT_DIR" CXXFLAGS="-O2" && make -j6 \
        && make check -j6 && make install && cd -

    if [ $? -ne 0 ]; then
        echo "Fail to compile protobuf"
    else
        echo "Success to compile protobuf"
    fi
    rm -rf $TAR_DIR
}

# 把 .proto 协议编译成 c++ 代码
function compile_protocol {
    local SRC_DIR=$g_CHAT_ROOT/src/common/protocol
    local OUT_DIR=$SRC_DIR/protocol
    local_mkdir "$OUT_DIR"
    local protoc="$g_PROTOBUF_OUT/bin/protoc"
    for file in "$SRC_DIR"/*.proto
    do
        if [ -e "$file"  ]; then
            echo "$protoc"
            $protoc -I="$SRC_DIR" "$file" --cpp_out="$OUT_DIR"
            if [ $? -ne 0 ]; then
                echo "Fail to compile $file"
            else
                echo "Success to compile $file"
            fi
        fi
    done
    echo "Compile protocol done."
}

function clean_build() {
    echo rm -rf "$g_OUT_DIR"
    rm -rf "$g_OUT_DIR"
}

function clean_all() {
    clean_build
    echo rm -rf "$g_OUT_ROOT"
    rm -rf "$g_OUT_ROOT"
}

function clean_test() {
    echo rm -rf "$g_BIN_DIR"/"$TEST_EXE"
}

while getopts :bgptaAcC opt
do
    case $opt in
        b) compile_protobuf;; # protobuf only
        g) compile_gtest;; # gtest only
        p) compile_protocol;; # protocol only
        t) build_test;; # do test only
        a) build_all;; # build.*
        A) compile_protobuf; compile_protocol; build_all;;
        c) clean_build;; # delete build-$g_BUILD_TYPE
        C) clean_all;; # delete $g_OUT_ROOT
    esac
done
