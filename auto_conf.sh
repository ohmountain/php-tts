#!/bin/bash
sudo cp ./tts_libs/x64/libmsc.so.bk ./tts_libs/x64/libmsc.so
sudo cp ./tts_libs/x64/libmsc.so /lib/x86_64-linux-gnu/
sudo ldconfig
sudo phpize && ./configure LDFLAGS="-L./tts_libs/x64 -lmsc -lrt -ldl -lpthread"
sudo rm ./tts_libs/x64/libmsc.so
