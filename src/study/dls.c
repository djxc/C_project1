#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>      // 目录操作
#include "logger.h"
#include "dls.h"

/**
 * 列出给定目录的内容
 * 1、DIR 是目录的数据流，首先通过opendir获取到DIR，读取目录失败返回NULL
 * 2、dirent 目录对象的结构体，然后对目录数据流遍历读取，获取每个文件的dirent，结构体内包含文件名
 * 3、最后需要关闭目录数据流。
 */
int ls(char* dir_path) {
    DIR *dp;
    struct dirent * dirp;

    dp = opendir(dir_path);
    if (dp == NULL) {
        printf("can not open dir %s\n", dir_path);
        info("can not open dir ");
        return -1;
    } else {
        while ((dirp = readdir(dp)) != NULL) {
            printf("%s\n", dirp -> d_name);
        }        
    }
    closedir(dp);
    return 0;
}