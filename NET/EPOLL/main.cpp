#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>


#define MAX_EVENTS 1000

int main() {
    //创建event事件
    struct epoll_event event, eventsList[MAX_EVENTS];

    //创建socket文件描述符
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //在创建时设置为NONBLOCK
//    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK)

    //服务器端地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8999);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//    server_addr.sin_addr.s_addr = htonl(inet_addr("127.0.0.1"));

    //绑定服务器地址和socket文件描述符
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //开始监听，第一个参数为sockfd，第二个参数为backlog队列长度
    listen(sockfd, 100);

    //允许的最大socket数量1000
    //创建数组保存所有连接的socket
    int sockNumber[1000] = {0};
    sockNumber[0] = sockfd;

    //创建一个epoll对象，注意此处使用epoll_create1()而非epoll_cerate()
    //具体区别参见 man epoll_create
    int epollFD = epoll_create1(0);
    if (epollFD == -1) {
        perror("创建文件描述符失败");
        return 0;
    }

    //将之前的监听socket绑定到event上
    //这个event将作为epollFD的根节点（epollFD挂了一棵红黑树）
    event.data.fd = sockfd;
    event.events = EPOLLIN;

    //将上一步的event挂到树上
    int clRET = epoll_ctl(epollFD, EPOLL_CTL_ADD, sockfd, &event);
    if (-1 == clRET) {
        perror("注册监听事件失败");
    }

    int max1 = 0;
    //创建缓冲区
    char buf[1024] = {0};

    //创建一个标识符，用来关闭连接
    bool shutdown = false;

    //开始循环接收数据和请求
    while (1) {

        if (shutdown) {
            break;
        }

        //返回有事件的数量
        clRET = epoll_wait(epollFD, eventsList, MAX_EVENTS, -1);
        if (clRET < -1) {
            break;
        } else if (clRET == -1) {
            continue;
        }


        //循环遍历所有事件
        for (int i = 0; i < clRET; i++) {
            std::cout << eventsList[i].data.fd << std::endl;

            //判断是否是监听事件，有新的请求
            if (sockfd == eventsList[i].data.fd && EPOLLIN == (eventsList[i].events & EPOLLIN)) {

                //创建客户端的sockaddr
                struct sockaddr_in cli_addr;
                socklen_t length = sizeof (cli_addr);

                //接收一个新的socket文件描述符，创建了新的连接
                //这里返回一个新的sockfd->newClient
                int newClient = accept(sockfd, (struct sockaddr*)&cli_addr, &length);
                /*
                 * accept4()接收非阻塞的socket
                 * int newClient = accept4(sockfd, (struct sockaddr*)&cli_addr, &length, SOCK_NONBLOCK);
                 * 或者使用fnctl更改
                 * int flag = fcntl(newClient, F_GETFL, 0);
                 * fcntl(newClient, F_SETFL, flag | O_NONBLOCK);/
                 */

                if(newClient > 0) {

                    //注意这个event，在执行epoll_ctl的时候，会判断是否要将event进行拷贝（从用户态到内核）
                    //因此这里event是可以重用重写的。
                    //根据收到的请求创建一个新的event并在之后将其添加到epollFD上
                    event.data.fd = newClient;

                    //可以设置ET或者LT，默认是LT模式
                    //event.events = EPOLLIN | EPOLLET;
                    event.events = EPOLLIN;

                    //添加到epollFD
                    //When successful, epoll_ctl() returns zero.
                    // When an error occurs, epoll_ctl() returns -1 and errno is set appropriately.
                    int ret_for_epadd = epoll_ctl(epollFD, EPOLL_CTL_ADD, newClient, &event);
                    if (ret_for_epadd == 0) {
                        printf("%s%s\n", "新客户端上线", inet_ntoa(cli_addr.sin_addr));
                    }
                }
            }
            //处理写事件
            else if ((eventsList[i].events & (EPOLLIN | EPOLLERR)) == EPOLLIN) {
                memset(buf, 0, sizeof (buf));
                int len_of_recv = recv(eventsList[i].data.fd, buf, sizeof(buf), 0);
                if (len_of_recv <= 0) {
                    printf("%s\n", "无字节可读，关闭连接");
                    int delret = epoll_ctl(epollFD, EPOLL_CTL_DEL, eventsList[i].data.fd, &eventsList[i]);
                    close(eventsList[i].data.fd);
                }
                printf("%s", buf);

                //在客户端输入“stop”以关闭所有连接
                if (!strncmp(buf, "stop\n", 5)) {
                    shutdown = true;
                }

                /*将数据再发送回客户端
                 * 对于send()/write()，一定要依靠EPOLLOUT触发调用吗？什么时候才需要注册EPOLLOUT？
                 * the answer is no。如果返回值 > 0，说明数据都已经拷贝到发送缓冲区，一切正常。
                 * 而如果其 < 0并errno == EAGAIN，说明此时发送区已满，需要把剩余的字节保存；
                 * 并注册EPOLLOUT，等待epoll_wait()返回，此时如果有EPOLLOUT，说明发送缓冲区可写；
                 * 接着发送之前的数据，直到write()/send()返回值 > 0，再将EPOLLOUT取消
                 * */
                int len_of_write = send(eventsList[i].data.fd, buf, sizeof(buf), 0);
                std::cout << "number of write : " << len_of_write << std::endl;
            }
        }
    }
    close(epollFD);
    return 0;
}
