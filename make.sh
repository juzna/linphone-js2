#!/bin/bash
cd ../../;
#./prepmake.sh;

cd build;
make linphone

cp ./bin/linphone/nplinphone.so /home/juzna/.mozilla/plugins/
