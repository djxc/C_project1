#include <stdio.h>
#include "hello.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
 
/**
 * 从域名获取ip信息
 */
int getIpFromDomainName() {
    struct hostent *hos; 
    hos = gethostbyname("www.baidu.com");
    if (hos == NULL) {
        printf("gethostbyname error for host: %s: %s\n", 'dj', hstrerror(h_errno));
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

#define BUF_SIZE  20
/**
 * 通过管道实现进程间通信
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
 * 
*/
int main(int argc, char * argv[])
{
    // getIpFromDomainName();
    // test_fork();
    // test_signal();
    test_pipe();
    if (argc < 3){
        printf("Usage: %s base exponent \n", argv[0]);
        return 1;
    }
    double base = atof(argv[1]);
    int exponent = atoi(argv[2]);
    double result = dpower(base, exponent);
    printf("%ld ^ %d is %ld\n", base, exponent, result);
    return 0;
} 
