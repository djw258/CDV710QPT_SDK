#!/bin/sh
make_rootfs()
{
        cd ../rootfs/
        ./rootfs_build.sh
        cd -
}

create_platform()
{
	if [ -d "platform" ]; then
                echo "remove platform dir"
                rm -rf "platform"
                mkdir "platform"
        else
                echo "mkdir "rootfs""
                mkdir "platform"
        fi
        chmod 777 "platform" -R
}
#制作镜像文件
make_jffs2_images()
{
        echo -n "build config.jffs2 [y/n]:"
        read name
        if [ $name == "y" ]; then

                ./mkfs.jffs2 -d ../rootfs/rootfs/etc/config -s 256 -e 4096 -m none -o platform/config.jffs2
        fi
}
make_squashfs_images()
{
        mkdir -p tmpfs
        cp -af ../rootfs/rootfs/* tmpfs

        echo -n "build usr.sqsh4 [y/n]:"
        read name
        if [ $name == "y" ]; then
                 ./mksquashfs tmpfs/usr platform/usr.sqsh4 -noappend -comp xz
        fi 

        echo -n "build app.sqsh4 [y/n]:"
        read name
        if [ $name == "y" ]; then
	        ./mksquashfs tmpfs/app platform/app.sqsh4 -noappend -comp xz
        fi

        echo -n "build root.sqsh4 [y/n]:"
        read name

        if [ $name == "y" ]; then
                rm -rf tmpfs/usr/* tmpfs/etc/config/* tmpfs/app/*
	        ./mksquashfs tmpfs/ platform/root.sqsh4 -noappend -comp xz
	fi
	rm -rf tmpfs
}

copy_kernel_images()
{
        echo -n "copy u-boot.bin [y/n]:"
        read name
        if [ $name == "y" ]; then
                cp  ../kernel/u-boot.bin platform/
        fi


        echo -n "copy C300_SQ38_AK3918EV300_GC2063_V1.0.0.dtb [y/n]:"
        read name
        if [ $name == "y" ]; then
                 cp  ../kernel/C300_SQ38_AK3918EV300_GC2063_V1.0.0.dtb platform/
        fi

        echo -n "copy uImage [y/n]:"
        read name
        if [ $name == "y" ]; then
                cp  ../kernel/uImage platform/
        fi


        echo -n "copy env_v330.img [y/n]:"
        read name
        if [ $name == "y" ]; then
                cp  ../kernel/env_v330.img platform/
        fi
}

images_compress()
{
        cd platform/
        rm -rf SAT_ANYKA3918OS
        tar -zcvf SAT_ANYKA3918OS *
	mv SAT_ANYKA3918OS ../../../
	cd ../
        cp -r platform ../../../tools/burntool/
}

make_rootfs
create_platform
make_jffs2_images
make_squashfs_images
copy_kernel_images

images_compress


