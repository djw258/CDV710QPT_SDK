cmake_minimum_required(VERSION 3.5.4)

set(CMAKE_C_COMPILER "/home/leo/workspace/CDV810QPT_LIB/outdoor/gcc/bin/arm-anykav500-linux-uclibcgnueabi-gcc")
set(CMAKE_CXX_COMPILER "/home/leo/workspace/CDV810QPT_LIB/outdoor/gcc/bin/arm-anykav500-linux-uclibcgnueabi-g++")

# 设置编译器，此处为ARM平台，如果你安装了编译链就用名称，没安装请用绝对路径
set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)

SET(leo_proj SAT_ANYKA3918.BIN)
project(${leo_proj})

# 工程名
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_C_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -s -Wall -std=gnu11  -Werror")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -s -Wall -std=c++1y -Werror")

#设置资源路径
add_definitions(
#	-DRESOURCE_UI_PATH="A:/appconfigs/run/resource/ui"
#	-DRESOURCE_WALLPAPER_PATH="A:/appconfigs/run/resource/wallpaper/"
#	-DLV_FREETYPE_PATH="/appconfigs/run/resource/ttf/sat_leo.ttf"
	-DMAC_ETH0_PATH="/etc/config/mac.txt"
	-DMAC_ETH1_PATH="/appconfigs/run/eth1_mac"
	-DMAC_ETH0_BACKUP_PATH="/etc/config/mac_backup.txt"
#	-DRESOURCE_UI_PATH="A:/tmp/nfs/ssd20x/project/layout/resource/ui/"
#	-DRESOURCE_WALLPAPER_PATH="A:/tmp/nfs/ssd20x/project/layout/resource/wallpaper/"
#	-DLV_FREETYPE_PATH="/tmp/nfs/ssd20x/project/layout/resource/sat_leo.ttf"
	-DONVIF_XML_PATH="/app/onvif/"
	-DRESOURCE_RING_PATH="/app/rings/"
	-DCIPD20DKS
)
# layout
include_directories(
	${PROJECT_PATH}/common/
	${PROJECT_PATH}/layout/
	${PROJECT_PATH}/res/
)
# include
include_directories(
	${PROJECT_PATH}/include/
	${PROJECT_PATH}/share/
)
# lvgl
include_directories(
#	${PROJECT_PATH}/share/lvgl-8.3.1/
)
# jpeglib
include_directories(
#	${PROJECT_PATH}/share/jpeg-9d/
)
# freetype
include_directories(
#	${PROJECT_PATH}/share/freetype/include/
)
# linphone
add_definitions(
	-DOSIP_MT
	-DENABLE_DEBUG
	-DENABLE_TRACE
	-DIN_LINPHONE
	-DLEO_ADD_LINPHONE
	-DLEO_MULTICASE_RTP
)
# zlib
include_directories(
	${PROJECT_PATH}share/zlib/
)
# onvifi
include_directories(
	${PROJECT_PATH}share/onvif/
)
#tuya api
include_directories(
	${PROJECT_PATH}share/tuya/
)
#dds
#include_directories(
#	${PROJECT_PATH}share/cyclonedds-0.10.2/src/
#	${PROJECT_PATH}share/cyclonedds-0.10.2/src/ddsrt/include/
#	${PROJECT_PATH}share/cyclonedds-0.10.2/src/core/ddsc/include/
#	${PROJECT_PATH}share/cyclonedds-0.10.2/src/ddsrt/include/
#	${PROJECT_PATH}share/cyclonedds-0.10.2/build/src/core/include/
#	${PROJECT_PATH}share/cyclonedds-0.10.2/build/src/ddsrt/include/
#)
# 添加src目录
aux_source_directory(${PROJECT_PATH}layout/ DIR_SRCS)
aux_source_directory(${PROJECT_PATH}layout/common DIR_SRCS)

#add_subdirectory(${PROJECT_PATH}share ${CMAKE_CURRENT_SOURCE_DIR}/share/)
#add_subdirectory(${PROJECT_PATH}common ${CMAKE_CURRENT_SOURCE_DIR}/common/)

#set(VAR ${VAR} lvgl)
#set(VAR ${VAR} jpeg)
#set(VAR ${VAR} freetype)
#set(VAR ${VAR} zlib)
#set(VAR ${VAR} user_common)
#set(VAR ${VAR} ffmpeg)

#set(VAR ${VAR} exosip2)
#set(VAR ${VAR} linphone)
#set(VAR ${VAR} mediastreamer2)
#set(VAR ${VAR} ortp)
#set(VAR ${VAR} osip2)

#set(VAR ${VAR} speex)
#set(VAR ${VAR} onvif)
#set(VAR ${VAR} mxml)
#set(VAR ${VAR} sha1)
#set(VAR ${VAR} base64)
#set(VAR ${VAR} live555)
#set(VAR ${VAR} xls)
#set(VAR ${VAR} tuya)
#set(VAR ${VAR} cyclonedds)

# set(VAR ${VAR} pjsip)
add_executable(${leo_proj} ${DIR_SRCS})
message("VAR : ${VAR}")

set(SHARE_LIB_PATH ${PROJECT_PATH}/lib/share)
set(COMMON_LIB_PATH ${PROJECT_PATH}/lib/common)
set(PLATFORM_LIB_PATH ${PROJECT_PATH}/lib/anyka)
#set(OPENSSL_LIB_PATH ${PROJECT_PATH}/lib/openssl)
#set(TUYA_LIB_PATH ${PROJECT_PATH}/lib/tuya)

target_link_libraries(${leo_proj} -Wl,--start-group

	# share lib
	#${VAR}
	stdc++
	# stand lib
	m
	pthread
	dl

	${COMMON_LIB_PATH}/libuser_common.a
	#share
	${SHARE_LIB_PATH}/libbase64.a
	${SHARE_LIB_PATH}/libexosip2.a
	${SHARE_LIB_PATH}/libffmpeg.a
	#${SHARE_LIB_PATH}/libfreetype.a
	${SHARE_LIB_PATH}/liblinphone.a
	${SHARE_LIB_PATH}/liblive555.a
	#${SHARE_LIB_PATH}/liblvgl.a
	${SHARE_LIB_PATH}/libmediastreamer2.a
	${SHARE_LIB_PATH}/libmxml.a
	${SHARE_LIB_PATH}/libonvif.a
	${SHARE_LIB_PATH}/libortp.a
	${SHARE_LIB_PATH}/libosip2.a
	${SHARE_LIB_PATH}/libsha1.a
	${SHARE_LIB_PATH}/libspeex.a
	#${SHARE_LIB_PATH}/libtuya.a
	#${SHARE_LIB_PATH}/libxls.a
	#${SHARE_LIB_PATH}/libjpeg.a
	${SHARE_LIB_PATH}/libzlib.a
	
	# system lib
	${PLATFORM_LIB_PATH}/libplat_ao.so
	${PLATFORM_LIB_PATH}/libplat_thread.so
	${PLATFORM_LIB_PATH}/libplat_vqe.so
	${PLATFORM_LIB_PATH}/libplat_mem.so
	${PLATFORM_LIB_PATH}/libplat_common.so
	${PLATFORM_LIB_PATH}/libplat_osal.so
	${PLATFORM_LIB_PATH}/libplat_ai.so
	${PLATFORM_LIB_PATH}/libplat_vi.so
	${PLATFORM_LIB_PATH}/libakaudiofilter.so
	${PLATFORM_LIB_PATH}/libplat_isp_sdk.so
	${PLATFORM_LIB_PATH}/libplat_log.so
	${PLATFORM_LIB_PATH}/libplat_dbg.so
	${PLATFORM_LIB_PATH}/libplat_vpss.so
	${PLATFORM_LIB_PATH}/libmpi_venc.so
	${PLATFORM_LIB_PATH}/libakv_encode.so

	-Wl,--end-group)
