#!/bin/sh

PARAM_NUM=$#

make_app()
{
        cd build
        rm -rf SAT_ANYKA3918.BIN
        rm CMakeFiles/SAT_ANYKA3918.BIN.dir/home/leo/workspace/CDV810QPT_LIB/outdoor/app/layout/common/user_linphone.c.o

        if [ $PARAM_NUM -gt 0 ] ; then
                mv   CMakeLists.txt ../
                rm ./* -rf
                mv ../CMakeLists.txt ./
        fi
        cmake .;make

        if [ ! -f SAT_ANYKA3918.BIN ] ; then
                exit 0
        fi

        cd -
}

copy_to_rootfs()
{
        rm -rf ./rootfs/rootfs/resource/app/
	  mkdir ./rootfs/rootfs/resource/app
        cp ./build/SAT_ANYKA3918.BIN ./rootfs/rootfs/resource/app/
        cp -r layout/resource/rings rootfs/rootfs/resource/app/rings/
        cp -r layout/resource/onvif rootfs/rootfs/resource/app/onvif/
}

make_rootfs()
{
      cd ./rootfs/upgrade/
      ./make_image.sh
      cd -
}

make_app

if [ $# -ge 1 ] && [ $1 == "rootfs" ]; then

        copy_to_rootfs

        make_rootfs
fi

