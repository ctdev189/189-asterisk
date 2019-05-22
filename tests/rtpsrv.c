#include <arpa/inet.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#include "rtp.h"

#define MAXDATASIZE 1500 //数据长度

int main(int argc, char **argv)
{
    struct sockaddr_in local_sockaddr, remote_sockaddr; //声明服务器和客户端的socket存储结构
    int local_port;
    ssize_t recvbytes;
    socklen_t sin_size;
    int sockfd;            //socket描述符
    char buf[MAXDATASIZE]; //传输的数据

    if (argc < 2)
    { //需要用户指定链接的地址
        fprintf(stderr, "Please enter the rtp server port");
        exit(1);
    }
    local_port = atoi(argv[1]);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    { //建立socket链接
        perror("Socket");
        exit(1);
    }

    printf("Socket success!, sockfd=%d, port=%d\n", sockfd, local_port);

    //以sockaddt_in结构体填充socket信息
    memset(&local_sockaddr, 0, sizeof(local_sockaddr));
    local_sockaddr.sin_family = AF_INET;         //IPv4
    local_sockaddr.sin_port = htons(local_port); //端口
    local_sockaddr.sin_addr.s_addr = INADDR_ANY; //本主机的任意IP都可以使用

    if ((bind(sockfd, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr))) == -1)
    { //bind函数绑定
        perror("Bind");
        exit(-1);
    }

    printf("Bind success!\n");

    struct pollfd fds[1];
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    while (1)
    {
        int resfd = 0;
        resfd = poll(fds, 1, 1000);
        printf("Waiting package...\n");
        if (resfd < 0)
        {
            if (errno == EINTR)
            {
                perror("poll");
                continue;
            }
            break;
        }
        else if (resfd == 0)
        {
            /* No data, keep going */
            continue;
        }
        if (fds[0].revents & (POLLERR | POLLHUP))
        {
        }
        else if (fds[0].revents & POLLIN)
        {
            recvbytes = recvfrom(sockfd, &buf, MAXDATASIZE, 0, (struct sockaddr *)&remote_sockaddr, &sin_size);

            dev189_rtp_header *header = (dev189_rtp_header *)buf;

            unsigned long timestamp = ntohl(header->timestamp);
            unsigned short seq = ntohs(header->seq_number);

            printf("Received a package: %u\n", seq);
        }
    }

    //char *ip = inet_ntoa(remote_sockaddr.sin_addr);
    // printf("Received from : %s\n", ip);

    close(sockfd);

    return 0;
}