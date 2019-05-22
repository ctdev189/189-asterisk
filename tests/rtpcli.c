#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAXDATASIZE 1500

int main(int argc, char *argv[])
{
    int sockfd, sendbytes;
    int remote_upd_port;
    char buf[MAXDATASIZE];
    struct hostent *host;
    struct sockaddr_in serv_addr;

    if (argc < 3)
    { //需要用户指定链接的地址
        fprintf(stderr, "Please enter the server's hostname and port\n");
        exit(1);
    }

    if ((host = gethostbyname(argv[1])) == NULL)
    { //转换为hostent
        perror("gethostbyname");
        exit(1);
    }

    remote_upd_port = atoi(argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    { //创建socket
        perror("socket");
        exit(1);
    }

    printf("Socket success!, sockfd=%d, hostname=%s, port=%d\n", sockfd, host->h_name, remote_upd_port);

    //填充数据
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(remote_upd_port);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    memset(serv_addr.sin_zero, 0, 8);

    // if ((connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr))) == -1)
    // { //发起对服务器的链接
    //     perror("connect");
    //     exit(1);
    // }

    strcpy(buf, "hello");
    sendto(sockfd, &buf, MAXDATASIZE, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    close(sockfd);
}