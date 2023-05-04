#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
 
#define BUF_SIZE  20
#define EPOLL_SIZE 50

#define THREAD_NUM 100

/**
 * 从域名获取ip信息
 */
int getIpFromDomainName() {
    struct hostent *hos; 
    hos = gethostbyname("www.baidu.com");
    if (hos == NULL) {
        printf("gethostbyname error for host: %s\n", hstrerror(h_errno));
        return -1;
    }
    printf("\tofficial: %s\n", hos->h_name);
    for (int i = 0; hos->h_aliases[i]; i++) {
        printf("IP addr %d:%s\n", i+1, hos->h_aliases[i]);
    }
    if (hos->h_addrtype == AF_INET) {
        printf("IPV4\n"); 
    } else {
        printf("IPV6\n"); 
    }
    for (int i = 0; hos->h_addr_list[i]; i++) {
        printf("IP addr %d:%s\n", i+1, 
            inet_ntoa(*(struct in_addr*)hos->h_addr_list[i]));
    }
    return 0;
}

/**
 * 创建进程
 * 1、当调用fork之后会存在父子两个进程，两个进程内存完全一样，之后两个进程执行的代码则出现分歧
 */
int test_fork() {
    printf("test fork process\n");
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        printf("this is child process\n");
        sleep(15);
        return 10;
    } else {
        while (!waitpid(-1, &status, WNOHANG))
        {
            sleep(3);
            printf("parent sleep 3 second\n");
        }
        if (WIFEXITED(status)) {
            printf("get child value:%d\n", WEXITSTATUS(status));
        }        
    }
    return 0;
}


/**
 * 定时任务
*/
void test_timeout(int sig) {
    if (sig == SIGALRM) {
        printf("Time out\n");
    }
    alarm(2);
}

/**
 * 测试ctrl+C
*/
void test_keycontrol(int sig) {
    if (sig == SIGINT) {
        printf("keycontrol");
    }
}

void test_signal() {
    signal(SIGALRM, test_timeout);
    alarm(2);
    for (int i = 0; i< 3; i++) {
        puts("wait....");
        sleep(10);
    }
}

/**
 * 通过匿名管道实现进程间通信
 * 1、匿名管道的使用需要在父子线程之间，共享同一个管道引用
*/
void test_pipe() {
    int files[2];
    char name[] = "djxc jm";
    char age[] = "djxc age is 31";

    pipe(files);
    char buf[BUF_SIZE];

    pid_t pid = fork();
    if (pid == 0) {
        write(files[1], name, sizeof(name));
        sleep(3);
        read(files[0], buf, BUF_SIZE);
        printf("this is child process, revice parent process message: %s\n", buf);
    } else {
        read(files[0], buf, BUF_SIZE);
        printf("this is parent process, revice child process message: %s\n", buf);
        write(files[1], age, sizeof(age));
        sleep(5);
    }
}

/**
 * 测试select
*/
void test_select() {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);     // 检测标准输入文件描述
        if (result == -1) {
            puts("select() error");
            break;
        } else if(result == 0) {
            puts("time out!");
        } else {
            if (FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return;
    
}

/**
 * 启动服务器
 * @param port 端口
 * @return server_socket 服务监听socket
*/
int startup_server(int port) {
    int server_socket;
    struct sockaddr_in server_addr;
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}
    memset(&server_addr, 0, sizeof(server_addr));	//bzero(&server_addr, sizeof(server_addr));	 
	//初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序	 
	server_addr.sin_family = AF_INET;	 
	server_addr.sin_port = htons(port);	 
	//ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址	 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	 
	//对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)	 
	//bind三个参数：服务器端的套接字的文件描述符，	 
	if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)	 
	{	 
		perror("bind error");	 
		return -1;	 
	}
	 
	//设置服务器上的socket为监听状态	 
	if(listen(server_socket, 5) < 0)	 
	{	 
		perror("listen error");	 
		return -1;	 
	}	 
    return server_socket;
}

/**
 * 基于select创建socket服务器
 * 相对于多线程socket服务器，select服务无需创建线程、线程切换开销，
*/
int socket_server_select(int port) {
    //调用socket函数返回的文件描述符
	int server_socket, client_socket;
	//声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
	struct sockaddr_in client_addr;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

	server_socket = startup_server(port);
    if (server_socket == -1) {
        return -1;
    }
    FD_ZERO(&reads);
    FD_SET(server_socket, &reads);
    fd_max = server_socket;
	while(1)	 
	{	 
		printf("select run");	 
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        // 如果文件描述符发生变化则select会立即返回，如果没有发生变化，则在timeout之后会返回0
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) {
            break;
        }

        if (fd_num == 0) {
            continue;
        }

        for (i = 0; i < fd_max+1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == server_socket) {
                    adr_sz = sizeof(client_addr);
                    client_socket = accept(server_socket,  (struct sockaddr*)&client_addr, (socklen_t*)&adr_sz);
                    FD_SET(client_socket, &reads);      // 将客户端添加到reads中
                    if (fd_max < client_socket) {
                        fd_max = client_socket;
                    }
                    printf("connect client: %d\n", client_socket);
                } else {
                    // 处理请求
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d\n", i);
                    } else {
                        write(i, buf, str_len);
                    }
                }
            }
        }
	}
	close(server_socket);
	return 0;
}


/**
 * 基于epoll创建socket服务器
 * 1、首先创建文件描述符的空间，epoll_create，由操作系统管理，返回文件描述符
 * 2、注册需要监视的文件描述符，epoll_ctl,增加、修改或删除需要监听的文件描述符指定的事件。
 * 3、等待文件描述符发生变化，epoll_wait,返回发生变化的文件描述符
*/
int socket_server_epoll(int port) {
    //调用socket函数返回的文件描述符
	int server_socket, client_socket;
	//声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
	struct sockaddr_in client_addr;
    socklen_t adr_sz;

    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_ct;

	server_socket = startup_server(port);
    if (server_socket == -1) {
        return -1;
    }
    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) *EPOLL_SIZE);
    event.events = EPOLLIN;
    event.data.fd = server_socket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);
	while(1)	 
	{	 
		printf("socket change");	 
        event_ct = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_ct == -1) {
            puts("epoll wait error");
            break;
        }
        for(i = 0; i < event_ct; i++) {
            if (ep_events[i].data.fd == server_socket) {
                adr_sz = sizeof(client_addr);
                client_socket = accept(server_socket,  (struct sockaddr*)&client_addr, (socklen_t*)&adr_sz);
                event.data.fd = client_socket;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event);
                printf("connected client: %d\n", client_socket);
            } else {
                // 处理请求
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if (str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client: %d\n", ep_events[i].data.fd);
                } else {
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }      
	close(server_socket);
    close(epfd);
	return 0;
}


void* thread_func(void* args) {
    int i = 0;
    int cnt = *((int*)args);
    char * msg = (char *) malloc(sizeof(char) * 50);
    strcpy(msg, "This is thread return");

    for (i=0; i < cnt; i++) {
        sleep(1);
        puts("runing thread");
    }
    return (void *) msg;
}

/**
 * 线程测试
 * 1、利用thread_join主线程等待子线程结束
*/
int test_thread() {
    pthread_t t_id;
    int thread_param = 5;
    void * prd_res;
    if (pthread_create(&t_id, NULL, thread_func, (void*)&thread_param) != 0) {
        puts("create thread error");
        return -1;
    }
    // sleep(5);
    if (pthread_join(t_id, &prd_res) != 0) {
        puts("thread join error");
        return -1;
    }
    printf("Thread return message: %s\n", (char *)prd_res);
    free(prd_res);
    puts("end main thread");
    return 0;
}

long long num = 0;

void * thread_inc(void * args) {
    int i;
    for (i = 0; i < 20000000; i++) {
        num += 1;
    }
    return NULL;
}

void * thread_des(void * args) {
    int i;
    for (i = 0; i < 20000000; i++) {
        num -= 1;
    }
    return NULL;
}

int test_unsafe_thread() {
    clock_t start_t, end_t;
    start_t = clock();
    pthread_t t_ids[THREAD_NUM];
    int i;
    printf("size of long long:%ld\n", sizeof(long long));
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&t_ids[i], NULL, thread_inc, NULL) != 0) {
                puts("create thread error");
                return -1;
            }
        } else {
            if (pthread_create(&t_ids[i], NULL, thread_des, NULL) != 0) {
                puts("create thread error");
                return -1;
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(t_ids[i], NULL) != 0) {
            puts("thread join error");
            return -1;
        }
    }   
    printf("end main thread, num is %lld\n", num);
    end_t = clock();
    printf("Time used = %.5fs\n", (double)(end_t - start_t)/CLOCKS_PER_SEC);
    return 0;
}

pthread_mutex_t mutex_t;

void * thread_safe_inc(void * args) {
    int i;
    pthread_mutex_lock(&mutex_t);
    for (i = 0; i < 20000000; i++) {
        num += 1;
    }
    pthread_mutex_unlock(&mutex_t);
    return NULL;
}

void * thread_safe_des(void * args) {
    int i;
    for (i = 0; i < 20000000; i++) {
        pthread_mutex_lock(&mutex_t);
        num -= 1;
        pthread_mutex_unlock(&mutex_t);
    }
    return NULL;
}

int test_safe_thread() {
    clock_t start_t, end_t;
    start_t = clock();
    pthread_mutex_init(&mutex_t, NULL);

    pthread_t t_ids[THREAD_NUM];
    int i;
    printf("size of long long:%ld\n", sizeof(long long));
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&t_ids[i], NULL, thread_safe_inc, NULL) != 0) {
                puts("create thread error");
                return -1;
            }
        } else {
            if (pthread_create(&t_ids[i], NULL, thread_safe_des, NULL) != 0) {
                puts("create thread error");
                return -1;
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(t_ids[i], NULL) != 0) {
            puts("thread join error");
            return -1;
        }
    }   
    printf("end main thread, num is %lld\n", num);
    pthread_mutex_destroy(&mutex_t);
    end_t = clock();
    printf("Time used = %.5fs\n", (double)(end_t - start_t)/CLOCKS_PER_SEC);
    return 0;
}


/**
 * 
*/
// int main(int argc, char * argv[])
// {
//     // getIpFromDomainName();
//     // test_fork();
//     // test_signal();
//     test_pipe();
//     if (argc < 3){
//         printf("Usage: %s base exponent \n", argv[0]);
//         return 1;
//     }
//     double base = atof(argv[1]);
//     int exponent = atoi(argv[2]);
//     double result = dpower(base, exponent);
//     printf("%ld ^ %d is %ld\n", base, exponent, result);
//     return 0;
// } 
