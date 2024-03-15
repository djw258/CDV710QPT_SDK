
#include "layout_define.h"
#include "layout_mac_common.h"
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>
static char indoor_mac_fimd_file[128] = {0};
static char outdoor_mac_fimd_file[128] = {0};
/***
**   日期:2022-05-27 08:13:17
**   作者: leo.liu
**   函数作用：遍历txt文件是否存在
**   参数说明:
***/
static bool indoor_mac_txt_exist_check(void)
{
  struct tm tm;
  struct dirent *pdirent;
  int find_count = 0;
  system("mount -t vfat /dev/mmcblk0 /tmp/tf");
  DIR *d_info = opendir(INDOOR_MAC_TXT_PATH);
  if (d_info)
  {
    while ((pdirent = readdir(d_info)) != NULL)
    {
      if ((pdirent->d_type & DT_REG) && (strstr(pdirent->d_name, ".txt")) && (strlen(pdirent->d_name) == 24))
      {
        sscanf(pdirent->d_name, "%04d-%02d-%02d-CIP-70QPT.txt", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
        if ((tm.tm_year > 2021) && (tm.tm_year < 2099) && (tm.tm_mon > 0) && (tm.tm_mon < 13) && (tm.tm_mday > 0) && (tm.tm_mday < 32))
        {
          strncpy(indoor_mac_fimd_file, pdirent->d_name, sizeof(indoor_mac_fimd_file));
          find_count++;
        }
      }
    }
    closedir(d_info);
  }
  bool reslut = false;
  if (find_count == 1)
  {
    reslut = true;
  }
  return reslut;
}

bool outdoor_mac_txt_exist_check(void)
{
  struct tm tm;
  struct dirent *pdirent;
  int find_count = 0;
  system("mount -t vfat /dev/mmcblk0 /tmp/tf");
  DIR *d_info = opendir(OUTDOOR_MAC_TXT_PATH);
  if (d_info)
  {
    while ((pdirent = readdir(d_info)) != NULL)
    {
      if ((pdirent->d_type & DT_REG) && (strstr(pdirent->d_name, ".txt")) && (strlen(pdirent->d_name) == 24))
      {
        sscanf(pdirent->d_name, "%04d-%02d-%02d-CIP-D20YS.txt", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
        if ((tm.tm_year > 2021) && (tm.tm_year < 2099) && (tm.tm_mon > 0) && (tm.tm_mon < 13) && (tm.tm_mday > 0) && (tm.tm_mday < 32))
        {
          strncpy(outdoor_mac_fimd_file, pdirent->d_name, sizeof(outdoor_mac_fimd_file));
          find_count++;
        }
      }
    }
    closedir(d_info);
  }
  bool reslut = false;
  if (find_count == 1)
  {
    reslut = true;
  }
  return reslut;
}

int is_mac_address(const char *str)
{
  int i, count = 0;

  // 遍历字符串
  for (i = 0; str[i]; i++)
  {
    // 检查字符是否为十六进制数字或冒号
    if (!(isxdigit(str[i]) || str[i] == ':'))
    {
      return 0; // 非法字符
    }

    // 统计冒号的数量
    if (str[i] == ':')
    {
      count++;
    }
  }

  // MAC地址应有5个冒号，共6个字节
  if (count != 5)
  {
    return 0;
  }

  // MAC地址长度应为17个字符
  if (i != 17)
  {
    return 0;
  }

  return 1; // 符合MAC地址格式
}

/************************************************************
** 函数说明: 根据索引号生成mac地址
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
// 根据索引号生成MAC地址
static void generate_mac_address(int index, const char *base_mac, char *mac_address)
{
  if (index < 1 || index > 65536)
  {
    // 参数超出范围，给一个默认值
    index = 1;
  }
  if (!is_mac_address(base_mac))
  {
    return;
  }

  unsigned char mac_bytes[6]; // 存储MAC地址的每个字节
  sscanf(base_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
         &mac_bytes[0], &mac_bytes[1], &mac_bytes[2],
         &mac_bytes[3], &mac_bytes[4], &mac_bytes[5]);

  // 将起始MAC地址的最低字节加上参数
  int incremented_value = mac_bytes[5] + index - 1;
  mac_bytes[5] = (unsigned char)incremented_value;

  // 处理低字节溢出，向高字节进位
  while (incremented_value >= 256)
  {
    incremented_value /= 256; // 计算进位值
    mac_bytes[4] += incremented_value;
  }

  // 将新的MAC地址格式化为字符串
  sprintf(mac_address, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac_bytes[0], mac_bytes[1], mac_bytes[2],
          mac_bytes[3], mac_bytes[4], mac_bytes[5]);
}

static bool mac_address_save(char *mac_address)
{
  if (access(MAC_ADDRESS_CONF_PATH, F_OK) == 0) // 无作用，以前实现的时候会用这个路径来保存，再复制到真正的路径，只是为了重新烧录mac的时候，清除掉这个路径的备份文件
  {
    system("rm -rf " MAC_ADDRESS_CONF_PATH);
  }
  // system("mkdir " MAC_ADDRESS_CONF_PATH);
  // char conf_path[64] = {0};
  // snprintf(conf_path, sizeof(conf_path), "%seth0_mac.conf", MAC_ADDRESS_CONF_PATH);
  // int fd = open(conf_path, O_CREAT | O_WRONLY);
  // if (fd < 0)
  // {
  //   printf("open %s failed \n", conf_path);
  //   return false;
  // }

  // char buffer[1024] = {0};
  // memset(buffer, 0, sizeof(buffer));
  // int len = sprintf(buffer, "%s", mac_address);
  // write(fd, buffer, len);
  // close(fd);

  // char cmd[128] = {0};
  // sprintf(cmd, "cp %s %s", conf_path, APPCONFIG_MAC_ADDRESS_CONF_PATH);
  // system(cmd);
  remove(APPCONFIG_MAC_ADDRESS_CONF_PATH);
  char cmd[256] = {0};
  sprintf(cmd, "echo %s >> %s", mac_address, APPCONFIG_MAC_ADDRESS_CONF_PATH);
  system(cmd);
  return true;
}

/************************************************************
** 函数说明: 读取txt起始地址
** 作者: xiaoxiao
** 日期：2023-12-05 13:44:45
** 参数说明:
** 注意事项：
************************************************************/
static bool mac_txt_base_mac_read(char *base_mac, bool indoor)
{
  if ((base_mac == NULL))
  {
    return false;
  }
  char conf_path[256] = {0};
  if (indoor)
  {
    snprintf(conf_path, sizeof(conf_path), "%s%s", INDOOR_MAC_TXT_PATH, indoor_mac_fimd_file);
  }
  else
  {
    snprintf(conf_path, sizeof(conf_path), "%s%s", OUTDOOR_MAC_TXT_PATH, outdoor_mac_fimd_file);
  }

  int fd = open(conf_path, O_RDONLY);
  if (fd < 0)
  {
    printf("open %s failed \n", conf_path);
    return false;
  }
  char buffer[18] = {0};
  memset(buffer, 0, sizeof(buffer));
  read(fd, buffer, sizeof(buffer));
  close(fd);
  sscanf(buffer, "%s", base_mac);
  return true;
}

/***
**   日期:2022-05-27 16:35:28
**   作者: leo.liu
**   函数作用：保存指定的mac地址
**   参数说明:
***/
bool local_mac_address_set(int index)
{
  char mac_address[18];
  // 起始MAC地址
  char base_mac[18] = {0};
  mac_txt_base_mac_read(base_mac, true);

  generate_mac_address(index, base_mac, mac_address);

  if (mac_address_save(mac_address) == false)
  {
    printf("save mac address faild \n");
    return false;
  }

  return true;
}

/************************************************************
** 函数说明: 获取本地mac
** 作者: xiaoxiao
** 日期：2023-10-23 08:21:24
** 参数说明:
** 注意事项：
************************************************************/
bool local_mac_address_get(char *mac_address)
{
  if ((mac_address == NULL))
  {
    return false;
  }
  int fd = open(APPCONFIG_MAC_ADDRESS_CONF_PATH, O_RDONLY);
  if (fd < 0)
  {
    printf("open %s failed \n", APPCONFIG_MAC_ADDRESS_CONF_PATH);
    return false;
  }
  char buffer[1024] = {0};
  memset(buffer, 0, sizeof(buffer));
  read(fd, buffer, sizeof(buffer));
  close(fd);
  sscanf(buffer, "%s", mac_address);
  return true;
}

/************************************************************
** 函数说明: 门口机mac地址生成获取
** 作者: xiaoxiao
** 日期：2023-12-05 13:38:11
** 参数说明:
** 注意事项：
************************************************************/
bool outdoor_mac_address_get(int index, char *mac_address)
{
  // 起始MAC地址
  char base_mac[18] = {0};

  mac_txt_base_mac_read(base_mac, false);

  generate_mac_address(index, base_mac, mac_address);
  return true;
}

/************************************************************
** 函数说明:
** 作者: xiaoxiao
** 日期：2023-12-05 11:59:56
** 参数说明:
** 注意事项：
************************************************************/
bool indoor_mac_init(int index)
{
  if (indoor_mac_txt_exist_check())
  {
    local_mac_address_set(index);
    return true;
  }
  return false;
}