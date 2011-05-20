#!/bin/bash
export CFLAGS='-ggdb3 -fPIC'
cd ../../;
./prepmake.sh;

cd build;
make linphone

cp ./bin/linphone/nplinphone.so /home/juzna/.mozilla/plugins/
