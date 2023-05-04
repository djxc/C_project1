#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/tool.h"
#include "src/readFile.h"
#include "src/geom.h"
#include "src/study/socket_study.h"
#include "src/study/socket_client.h"

// 主函数的参数：
// 1、argc（int）参数的个数
// 2、argv（char *[]）参数的字符指针数组
// 3、envp环境变量的字符指针数组
int main(int argc, char *argv[])
{
	int server_port = 8091;
	if(argc < 2) {
		// 输出带颜色的字体，可以多种效果进行组合通过分号分割开
		printf("\033[31;1merror\033[0m: plase set run type!\n1、server is socket server;\n2、client is socket client\n");
		exit(0);
	}
	if (strcmp(argv[1], "server") == 0) {
		// socket_server_select(server_port);
		socket_server_epoll(server_port);
	} else if (strcmp(argv[1], "client") == 0) {
		create_socket_client(server_port);
	} else if (strcmp(argv[1], "thread") == 0) {
		// test_thread();
		// test_unsafe_thread();
		test_safe_thread();
	}
	// hello();
	// test_select();
	// test_pipe();
	// int sum = add(1, 8);
	// printf("%d\n", sum);
	// Point point1 = createPoint(10.2, 13.1);
	// Point point2 = createPoint(12.2, 14.1);
	// printf("point x: %f\n", distance(&point1, &point2));

	// PointArray pointArray = parsePointsStr("10.2 13.2,24.2 15.4,30.2 13.2,44.2 15.4,50.2 13.2,64.2 15.4,70.2 13.2,84.2 15.4,90.2 13.2,104.2 15.4");
	// int pointSize = pointArray.size;
	// Point *point3 = pointArray.points;
	// printf("point3 x: %f, y:%f\n", point3[pointSize -1].x, point3[pointSize - 1].y);

	// printf("param num: %d\n", argc);
	// for(int i = 0; i < argc; i++){
	// 	printf("param is: %s\n", argv[i]);	
	// }
	// read_file(argv[1]);
	// if(argc < 3) {
	// 	// 输出带颜色的字体，可以多种效果进行组合通过分号分割开
	// 	printf("\033[31;1merror\033[0m: plase input the str to write!\n");
	// 	exit(0);
	// }

	// write_file(argv[1], argv[2]);	
	return 0;
}
