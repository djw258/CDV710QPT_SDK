#!/bin/sh


#v4l2_install() 
#{
#    modprobe media.ko
#    modprobe videobuf2-core.ko
#    modprobe videobuf2-memops.ko
#    modprobe videodev.ko
#    modprobe videobuf2-dma-contig.ko
#    modprobe v4l2-common.ko
#    modprobe videobuf2-v4l2.ko
#}
#
#v4l2_uninstall() 
#{
#    modprobe -r videobuf2-v4l2.ko
#    modprobe -r v4l2-common.ko
#    modprobe -r videobuf2-dma-contig.ko
#    modprobe -r videodev.ko
#    modprobe -r videobuf2-memops.ko
#    modprobe -r videobuf2-core.ko
#    modprobe -r media.ko
#}

install()
{
    echo "install sc5238 driver"
#    v4l2_install
    /sbin/insmod /usr/modules/ak_isp.ko internal_pclk=100000000
    /sbin/insmod /usr/modules/sensor_sc5238.ko
    if test ! -f /etc/config/isp_sc5238_mipi_2lane_h3b.conf ; then
        rm /etc/config/isp_*.conf
        cp /etc/isp_sc5238_mipi_2lane_h3b.conf /etc/config/
    fi
}

uninstall()
{
    echo "uninstall sc5238 driver"
    /sbin/rmmod ak_isp.ko
    /sbin/rmmod sensor_sc5238.ko
#    v4l2_uninstall
}

usage()
{
    echo "usage :" 
    echo "    ./sensor.sh <install/uninstall>"
    echo "    default : install"

}

if test $# -gt 1 ; then
    usage
elif test $# -eq 0 ; then
    install
elif test $# -eq 1 ; then
    if test $1 = "install" ; then
        install
    elif test $1 = "uninstall" ; then
        uninstall
    else
        usage
    fi
fi

