#!/bin/bash
# 解压缩根文件系统
tar_rootfs()
{
        if [ -d "rootfs" ]; then
                echo "remove rootfs dir"
                rm -rf "rootfs"
                mkdir "rootfs"
        else
                echo "mkdir "rootfs""
                mkdir "rootfs"
        fi
        
        echo "tar zxvf rootfs.tar.gz -C rootfs/"
        tar zxvf rootfs.tar.gz -C rootfs/
        cp -rf resource/* rootfs
        find rootfs -name .gitkeep | xargs rm -f
}
# 安装ko
install_modlues()
{
        if test -d rootfs/usr/modules ; then
                # 删除文件统中遗留的文件
                rm -rf rootfs/usr/modules/*
                rm -rf rootfs/lib/modules/*
        else
		mkdir rootfs/usr/modules
	fi  

        cp -rf ko/external/ak_saradc.ko rootfs/usr/modules/
        cp -rf ko/external/ak_adc_key.ko rootfs/usr/modules/
        cp -rf ko/external/ak_crypto.ko rootfs/usr/modules/
        cp -rf ko/external/ak_efuse.ko rootfs/usr/modules/
        cp -rf ko/external/ak_eth.ko rootfs/usr/modules/
        cp -rf ko/external/ak_hcd.ko rootfs/usr/modules/
        cp -rf ko/external/ak_i2c.ko rootfs/usr/modules/
        cp -rf ko/external/ak_ion.ko rootfs/usr/modules/
        cp -rf ko/external/ak_isp.ko rootfs/usr/modules/
        cp -rf ko/external/ak_mci.ko rootfs/usr/modules/
        cp -rf ko/external/ak_pcm.ko rootfs/usr/modules/
        cp -rf ko/external/ak_pwm_char.ko rootfs/usr/modules/
        cp -rf ko/external/ak_rtc.ko rootfs/usr/modules/
        cp -rf ko/external/ak_udc.ko rootfs/usr/modules/
        cp -rf ko/external/ak_uio.ko rootfs/usr/modules/
        cp -rf ko/external/exfat.ko rootfs/usr/modules/

        cp -rf ko/external/sensor_gc2063.ko rootfs/usr/modules/

        cp -rf ko/internal/lib rootfs/
        rm -rf rootfs/lib/modules/4.4.*/kernel/crypto/
        rm -rf rootfs/lib/modules/4.4.*/kernel/drivers/staging/rtl8188eu
        rm -rf rootfs/lib/modules/4.4.*/kernel/net/wireless/mac80211.ko
}

#复制脚本文件
copy_utils()
{
	cp -rf utils/* rootfs/
}



#第一步：解压根文件系统
tar_rootfs

#第二步：复制驱动模块
install_modlues

#第二步：复制脚本
copy_utils