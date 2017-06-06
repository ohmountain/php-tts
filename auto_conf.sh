#!/bin/bash
cp ./tts_libs/x64/libmsc.so.bk ./tts_libs/x64/libmsc.so
cp ./tts_libs/x64/libmsc.so /lib/x86_64-linux-gnu/
phpize && ./configure LDFLAGS="-L./tts_libs/x64 -lmsc -lrt -ldl -lpthread"
rm ./tts_libs/x64/libmsc.so
