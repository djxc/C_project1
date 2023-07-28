#include<stdio.h>
#include "logger.h"

void info(char * msg) {
    printf("\033[31;1minfo\033[0m: %s\n", msg);
}