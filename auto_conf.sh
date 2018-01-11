#!/bin/bash

### 检测是redhat系列还是debian系列
apt=`which apt 2>&1 | awk '{print $1}' | grep apt`
yum=`which yum 2>&1 | awk '{print $1}' | grep yum`

### 检测是32位还是64位
arch=`arch`

if [ "$arch" == "x86_64" ];then
    lib=./tts_libs/x64/libmsc.so.bk
else
    lib=./tts_libs/x86/libmsc.so.bk
fi

### redhat 系列
if [ -x "$yum" ];then
    if [ "$arch" == "x86_64" ];then
        cp $lib /usr/lib64/libmsc.so
    else
        cp $lib /usr/lib/libmsc.so
    fi
fi

### debian 系列
if [ -x "$apt" ];then
    if [ "$arch" == "x86_64" ];then
        cp $lib /lib/x86_64-linux-gnu/libmsc.so
    else
        cp $lib /lib/libmsc.so
    fi
fi

sudo ldconfig
sudo phpize && ./configure LDFLAGS="-L./tts_libs/x64 -lmsc -lrt -ldl -lpthread"
