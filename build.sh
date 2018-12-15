#!/bin/bash
readonly BUILDTYPE=debug # debug, release
readonly CHATROOT=$(cd `dirname $0`;pwd)
readonly OUTDIR=$CHATROOT/cmake-build-$BUILDTYPE
readonly BINDIR=$OUTDIR/bin
readonly LIBDIR=$OUTDIR/lib

if [[ ! -d $CHATROOT ]];then
    echo Unknown Wrong.
    exit
fi

myDefines="-DCMAKE_BUILD_TYPE=$BUILDTYPE"
cmake -S "$CHATROOT" -B "$OUTDIR" $myDefines && cd "$OUTDIR" && make

success=$?
if [ $success -eq 0 ];then
    echo Build Success
else
    echo Build Fail
    exit
fi
