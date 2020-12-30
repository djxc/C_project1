/**
 * 读取文件
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern int errno;

// 递归进行累加
int iterate(int value) {
	if(value == 1) {
		return 1;
	}else {
		return iterate(value-1) + value;
	}
}

// 读取文件，通过采用系统的open进行文件的读取，open相比较于fopen更加底层。
void read_file_(){
	int fd;
	char *path = "test.txt";
	char result[20];
	fd = open(path, O_RDONLY);
	read(fd, result, 10);
	printf("file read %s\n", result);
	close(fd);
}

// 打开文件
// 1、如果打开文件成功则返回文件指针
// 2、如果打开文件失败则返回空指针，并且输出错误信息
// @param {char*} file_path 要读取的文件路i径
// @param {char*} mode 打开文件的类型，只读还是写入
// @return fp {FILE*} 文件结构体指针
FILE* open_file(char *file_path, char *mode) {
	int errnum;
	FILE *fp = NULL;
	// 只读形式打开文件
	fp = fopen(file_path, mode);
	if(fp == NULL) {
		printf("faild to read file!\n");
		errnum = errno;
		fprintf(stderr, "错误号: %d\n", errno);
    	//perror("通过 perror 输出错误");
	    fprintf(stderr, "打开文件错误: %s\n", strerror( errnum ));
	}
	return fp;
}


// 采用stdio中的函数读取文件，创建文件指针，
// 通过fopen函数获取文件指针，如果打开失败则指针为NULL；
// 通过fgets函数将内容放在字符数组中
// c中没有try/catch，可以引入errno文件，全局变量errno记录了错误码，可以通过strerror解释错误码
void read_file(char *path) {
	FILE *fp = open_file(path, "r");
	char buff[255];
	if(fp != NULL) {
		printf("this file content is:\n");
		// 如果在while判断是否结尾了会重复最后一行
		while(1) {
			fgets(buff, 255, fp);
			if(feof(fp)) {
				break;
			}
			printf("\033[32m%s", buff);
		}
		fclose(fp);	
	}
}

// 写入文件
// @param str {char*} 待写入的字符串
void write_file(char *file_path, char *str){
	FILE *fp = open_file(file_path, "a+");
	if(fp != NULL) {
		fputs(str, fp);
		fputs("\n", fp);
		fclose(fp);
	}

}

