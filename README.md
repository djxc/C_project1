# 学习使用c语言
- 1、使用文本编辑器编写程序，利用gcc编译器进行编译`gcc test.c -o test.out`,运行生成的文件即可。


src是存放源码的
build是make生成文件的地方
CMakeLists.txt中PROJECT是项目名称.当执行到ADD_SUBDIRECTORY( src )时就会解析src下的CMakeList.txt
工程编译:进入build文件下,运行cmake .. 然后运行make进行编译.
- 1、如果在src新增文件夹，需要在新增文件夹中添加CMakeLists.txt文件，在父目录中CMakeList.txt添加ADD_SUBDIRECTORY新创建的文件夹
- 2、在根目录中CMakeLists.txt中TARGET_LINK_LIBRARIES添加新创建的文件。

# 动态连接库
- 1、创建动态链接库， `gcc hello.c -shared -fPIC -o libhello.so`,-shared表示输出为动态链接库， -fPIC 表示编译为位置独立的代码。
- 2、动态链接库的使用，`gcc main.c -L. -lhello -o main`, -L.表示在连接文件在当前目录下，-lhello表示连接的文件，编译器会自动查找libhello.输出为可执行文件。但是可执行文件可能找不到动态链接库，需要将so文件目录放在LD_LIBRARY_PATH环境变量中。

# 编译器原理学习
- 1、程序设计语言通过两方面定义：1）程序模式，即语言的语法；2）程序含义，即语言的语义。中缀表达式转换为后缀表达式，中缀表达式适合人类识别运算，后缀表达式适合计算机识别，因此需要将中缀表达式转换为后缀表达式。
