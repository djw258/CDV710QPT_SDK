#!/bin/sh


PARAM_NUM=$#

make_app()
{
        cd build
        rm -rf SAT_SSD20X.BIN
        rm CMakeFiles/SAT_SSD20X.BIN.dir/home/leo/workspace/CDV810QPT_LIB/indoor/app/layout/common/layout_common.c.o
        if [ $PARAM_NUM -gt 0 ]; then
                mv   CMakeLists.txt ../
                rm ./* -rf
                mv ../CMakeLists.txt ./
        fi
        cmake .;make -j8

        if [ ! -f SAT_SSD20X.BIN ] ; then
                exit 0
        fi
        cd -
}


copy_to_rootfs()
{
        rm -rf ./rootfs/board/CDV810QPT/app/
        rm -rf ./rootfs/board/CDV810QPT/resource/

        mkdir ./rootfs/board/CDV810QPT/app/
        mkdir ./rootfs/board/CDV810QPT/resource/

        cp ./build/SAT_SSD20X.BIN ./rootfs/board/CDV810QPT/app/

        cp -r ./layout/resource/commax_xml/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/onvif/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/rings/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/ttf/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/ui/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/wallpaper/ ./rootfs/board/CDV810QPT/resource/
        cp -r ./layout/resource/tuya/* ./rootfs/board/CDV810QPT/tuya/
}
make_rootfs()
{	
    	cd  rootfs
        ./build-project.sh
        ./make_sd_upgrade_sigmastar.sh
        cd -
        cp rootfs/image/output/images/CDV70QPT.BIN ./ 

	
       
}

# make_app

if [ $# -ge 1 ] && [ $1 == "rootfs" ]; then

        copy_to_rootfs

        make_rootfs
fi

