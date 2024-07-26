#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "ak_drv_common.h"

#include "../../include/ak_efuse.h"

#define AK_EFUSE_FILE_PATH "/dev/efuse"

static void display_efuse_global_id(unsigned char *global_id, int len)
{
    int tmp;

    printf("Chip globalID: ");
    for (tmp = 0; tmp < len; tmp++) {
        printf("%02X ", global_id[tmp]);
    }
    printf("\n");
}

int ak_drv_read_globalID(void)
{
	unsigned char *global_id;
	int global_id_len;
    int fd = -1;
	int ret = 0;

    fd = open(AK_EFUSE_FILE_PATH, O_RDONLY);
    if (fd < 0) {
        ak_print_error("open efuse fail.\n");
        return -EACCES;
    }

	global_id_len = ioctl(fd, DO_GLOBE_ID_GET_LEN, NULL);
	if (global_id_len <= 0) {
        ak_print_error("none efuse global id.\n");
        ret = -EACCES;
		goto error_get_len;
	}

	global_id = malloc(global_id_len);
	if (!global_id) {
        ak_print_error("malloc fail.\n");
        ret = -ENOMEM;
		goto error_malloc;
	}

    if (ioctl(fd, DO_GLOBE_ID_GET, global_id)) {
        ak_print_error_ex("read fail.\n");
        ret = -EIO;
		goto error_get_id;
    }

    display_efuse_global_id(global_id, global_id_len);

error_get_id:
	free(global_id);
error_malloc:
error_get_len:

    close(fd);

    return ret;
}
