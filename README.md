# 学习使用c语言
- 1、使用文本编辑器编写程序，利用gcc编译器进行编译`gcc test.c -o test.out`,运行生成的文件即可。

## 项目结构
- 1、src是存放源码的
- 2、build是make生成文件的地方
- 3、CMakeLists.txt中PROJECT是项目名称.当执行到ADD_SUBDIRECTORY( src )时就会解析src下的CMakeList.txt

## 工程编译
- 1、进入build文件下,运行cmake .. 然后运行make进行编译.
- 2、如果在src新增文件夹，需要在新增文件夹中添加CMakeLists.txt文件，在父目录中CMakeList.txt添加ADD_SUBDIRECTORY新创建的文件夹
- 3、在根目录中CMakeLists.txt中TARGET_LINK_LIBRARIES添加新创建的文件。

## 动态连接库
- 1、创建动态链接库， `gcc hello.c -shared -fPIC -o libhello.so`,-shared表示输出为动态链接库， -fPIC 表示编译为位置独立的代码。
- 2、动态链接库的使用，`gcc main.c -L. -lhello -o main`, -L.表示在连接文件在当前目录下，-lhello表示连接的文件，编译器会自动查找libhello.输出为可执行文件。但是可执行文件可能找不到动态链接库，需要将so文件目录放在LD_LIBRARY_PATH环境变量中。

## 编译器原理学习
- 1、程序设计语言通过两方面定义：
  - 1）程序模式，即语言的语法；
  - 2）程序含义，即语言的语义。  

中缀表达式转换为后缀表达式，中缀表达式适合人类识别运算，后缀表达式适合计算机识别，因此需要将中缀表达式转换为后缀表达式。

## select与epoll
&emsp;&emsp;多路复用可以通过创建多线程、select以及epoll实现。select存在对所有的文件描述符的循环查看，每次调用select时都需要向其传递监视对象信息。因此如果所需要监视的文件描述符数量较大，select即存在性能瓶颈。其中向select传递监视对象较为耗时，需要向操作系统传递监视对象。而epoll则可以弥补select的缺点，但select具备更好的移植性。对于文件描述符数据较小，且比较活跃，如果使用epoll则会频繁进行系统调用（epoll回调方式），推荐使用select；如果文件描述量大，但不是很活跃，使用epoll较好。

## 线程同步
函数内部多个线程运行引起问题的多条代码块。线程同步解决两种情况的问题：1、同时访问同一内存空间问题；2、访问同一内存的顺序问题。线程同步有两种技术：互斥量以及信号量。

