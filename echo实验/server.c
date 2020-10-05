//
// Created by HetingJian on 10/2/2020.
//
#include	"unp.h"

int
main(int argc, char **argv)
{
    int					listenfd, connfd;
    pid_t				childpid;
    socklen_t			clilen;
    struct sockaddr_in	cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    //socket function specify the type of communication protocol desired
    //其中family参数指明协议族，如常值AF_INET（指IPv4协议）
    //type参数指明套接字类型，如常值SOCK_STREAM（指字节流套接字）
    //protocol参数应设为某个协议常值（TCP、UDP），或设为0让系统选择给定family和type组合的系统默认值

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(SERV_PORT);
    //function h to n l&s 用于转换主机字节顺序和网络字节顺序
    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    //bind函数把一个本地协议地址赋予一个套接字，协议地址是32位的IPv4地址和16位的TCP端口号的组合
    //返回：若成功则返回0，否则返回-1
    Listen(listenfd, LISTENQ);
    //当socket函数创建一个套接字时，他被假设为一个主动套接字，也就是说它是一个将调用connect发起连接的客户套接字。
    // listen函数把一个未连接的套接字转换为一个被动套接字，指示内核应接受指向该套接字的连接请求。
    // 调用listen函数将导致套接字从CLOSED状态转换到LISTEN状态。
    // parameter: backlog  "the maximum length the queue of pending connections may grow to"
    //这个参数规定了内核应该为相应套接字排队的最大连接个数
    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
        //从已完成连接队列队头返回下一个已完成连接。如果对被client的地址不感兴趣可以把第二第三个参数设为空指针
        if ( (childpid = Fork()) == 0) {	/* child process */
            //进入这个if语句就是child process 先关闭listenfd,进行服务,然后退出
            //当一个连接建立时,accept返回，服务器接着调用fork，然后由子进程服务客户(通过已连接套接字connfd)
            //父进程则等待另一个连接(通过监听套接字listenfd)。既然新的客户服务由子进程提供服务，父进程就关闭已连接套接字。
            Close(listenfd);	/* close listening socket */
            str_echo(connfd);	/* process the request */
            exit(0);
        }
        Close(connfd);			/* parent closes connected socket */
    }
}

