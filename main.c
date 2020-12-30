#include<stdio.h>
#include <stdlib.h>

#include "src/tool.h"
#include "src/readFile.h"

// 主函数的参数：
// 1、argc（int）参数的个数
// 2、argv（char *[]）参数的字符指针数组
// 3、envp环境变量的字符指针数组
int main(int argc, char *argv[])
{
	hello();
	int sum = add(1, 8);
	printf("%d\n", sum);

	printf("param num: %d\n", argc);
	for(int i = 0; i < argc; i++){
		printf("param is: %s\n", argv[i]);	
	}
	if(argc < 2) {
		// 输出带颜色的字体，可以多种效果进行组合通过分号分割开
		printf("\033[31;1merror\033[0m: plase input the file to read!\n");
		exit(0);
	}
	read_file(argv[1]);
	if(argc < 3) {
		// 输出带颜色的字体，可以多种效果进行组合通过分号分割开
		printf("\033[31;1merror\033[0m: plase input the str to write!\n");
		exit(0);
	}

	write_file(argv[1], argv[2]);	
	return 0;
}
