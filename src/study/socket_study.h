void test_pipe();


void test_select();

/**
 * 基于select的socket服务器
*/
int socket_server_select(int port);

/**
 * 基于epoll的socket服务器
*/
int socket_server_epoll(int port);

/**
 * 测试线程
*/
int test_thread();


int test_unsafe_thread();

int test_safe_thread();