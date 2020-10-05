//
// Created by HetingJian on 10/2/2020.
//

#include	"unp.h"

int
main(int argc, char **argv)
{
    int					sockfd;
    struct sockaddr_in	servaddr;

    if (argc != 2)
        err_quit("usage: tcpcli <IPaddress>");

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    //TCP客户用connect函数来建立与TCP服务器的连接
    //参数： sockfd是由socket函数返回的套接字描述符，第二、三参数分别是一个指向套接字地址结构的指针和结构大小
    //connect函数会导致当前套接字从CLOSED状态转移到SYN_SENT状态，若成功再转移到ESTABLISHED状态
    //另：若connect函数失败则该套接字不可再用。必须关闭
    str_cli(stdin, sockfd);		/* do it all */

    exit(0);
}