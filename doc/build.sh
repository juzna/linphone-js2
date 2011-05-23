# remove old and buggy version of linphone from your system (version 3 from Ubuntu 9.10)
sudo apt-get remove liblinphone3 || sudo apt-get remove liblinphone2

# install basic tools
sudo apt-get install g++ cmake make autoconf gcc git-core wget autoconf libtool intltool

# install dependencies for linphone core
sudo apt-get install libspeex-dev libspeexdsp-dev libxml2-dev libosip2-dev libexosip2-dev \
	libavcodec-dev libswscale-dev libsdl-dev libgtk2-dev libx11-dev libxv-dev libx4l-dev

# prepare dependencies
#sudo apt-get build-dep liblinphone2 || sudo apt-get build-dep liblinphone3

# download submodules
git submodule update --init --recursive

# we want debug version
export CFLAGS='-ggdb3 -fPIC'


# build linphone
cd 3rdParty/linphone
./autogen.sh
./configure --enable-gtk_ui=no && make  || exit 1
#sudo make install || exit 1  # optional, probably not needed anymore
cd ../..

# prepare firebreath build
3rdParty/firebreath/prepmake.sh ./ build/


# build plugin
cd build/
make linphone

# copy generated plugin to your firefox plugin folder
mkdir -p ~/.mozilla/plugins/
cp -v bin/linphone/nplinphone.so ~/.mozilla/plugins/
