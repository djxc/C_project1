src是存放源码的
build是make生成文件的地方
CMakeLists.txt中PROJECT是项目名称.当执行到ADD_SUBDIRECTORY( src )时就会解析src下的CMakeList.txt
工程编译:进入build文件下,运行cmake .. 然后运行make进行解译.
