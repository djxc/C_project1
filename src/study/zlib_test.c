#include<stdio.h>
#include<zlib.h>

void zlib_version_test() {
    char *version = zlibVersion();
    printf("djxc zlib:%s\r\n", version);
}