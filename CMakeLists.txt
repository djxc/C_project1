PROJECT(DEMO2)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
# 指定生成目标,一个为主程序的名称，即为gcc -o xxx的值；其他的值为c原程序
ADD_SUBDIRECTORY(src)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
ADD_EXECUTABLE(main ${DIR_SRCS})

TARGET_LINK_LIBRARIES(main tool readFile geom socket_server socket_client -lm -lz -Wl,-rpath=/usr/local/lib)
