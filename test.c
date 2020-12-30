#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

// 递归进行累加
int iterate(int value) {
	if(value == 1) {
		return 1;
	}else {
		return iterate(value-1) + value;
	}
}

// 读取文件
void read_file(){
	int fd;
	char *path = "test.txt";
	char result[20];
	fd = open(path, O_RDONLY);
	read(fd, result, 10);
	printf("file read %s\n", result);
	close(fd);
}


// 采用stdio中的函数读取文件，创建文件指针，
// 通过fopen函数获取文件指针，通过fgets函数将内容放在字符数组中
void read_file1() {
	FILE *fp = NULL;
	char buff[255];
	fp = fopen("test.txt", "r");
	fgets(buff, 255, fp);
	printf("file content is %s", buff);
	fclose(fp);
}

int main() {
	printf("djxc");
	printf("%d\n", iterate(10));
	read_file();
	read_file1();
	return 1;
}
