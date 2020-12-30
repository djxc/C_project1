# 学习使用c语言
- 1、使用文本编辑器编写程序，利用gcc编译器进行编译`gcc test.c -o test.out`,运行生成的文件即可。


src是存放源码的
build是make生成文件的地方
CMakeLists.txt中PROJECT是项目名称.当执行到ADD_SUBDIRECTORY( src )时就会解析src下的CMakeList.txt
工程编译:进入build文件下,运行cmake .. 然后运行make进行解译.
