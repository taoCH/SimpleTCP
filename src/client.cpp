#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

using namespace std;

pthread_t thread;

unsigned short port = 8080;        		// 服务器的端口号
char *server_ip = "127.0.0.1";    	// 服务器ip地址


int TCPConnInit( int &sockfd , int port, char *server_ip )
{
	

	sockfd = socket(AF_INET, SOCK_STREAM, 0);// 创建通信端点：套接字
	
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	// 设置服务器地址结构体
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr)); // 初始化服务器地址
	server_addr.sin_family = AF_INET;	// IPv4
	server_addr.sin_port = htons(port);	// 端口
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);	// ip
	
	 // 主动连接服务器
	int err_log = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));     
	if(err_log != 0)
	{
		perror("connect");
		close(sockfd);
		return -1;
	}
}

int SendMsg( int &sockfd, string msg )
{
	cout<<"Message sent to "<<server_ip<<":"<<port<<endl;
	if ( send(sockfd, msg.c_str(), msg.size(), 0) < 0)   // 向服务器发送信息
		return -1;

	return 0;
}

void* RevMsg( void *cfd )
{
	int connfd = *((int*)cfd);
	
	char recv_buf[512] = "";
	while( recv(connfd, recv_buf, sizeof(recv_buf), 0) > 0 ) // 接收数据
	{
		printf("\nrecv data from host %c:\n", recv_buf[0]);
		printf("%s\n",recv_buf);
	}
		
	close(connfd);     //关闭已连接套接字
	printf("client closed!\n");
	return NULL;
}

int main()
{
	int sockfd; 
	string cmd, message;
	int res;

	if( TCPConnInit( sockfd, port, server_ip ) < 0 ) {
		cout << "Can't connect to server, please make sure the server is on" << endl;
		return -1;
 	}

	int *skd = &sockfd;
	res = pthread_create( &thread, NULL, RevMsg, skd );
    if(res)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",res);
     	exit(EXIT_FAILURE);
    }

	/*
	cout << "Enter a message to send : ";
	cin >> message;

	while ( !message.empty() && message != "quit" ) {
		if( SendMsg( sockfd, message ) < 0 )
			break;

		cout << "Enter a message to send : ";
		cin >> message;
	}
	*/

	while(1){
		cout << "[Command]";
		cin >> cmd;
	    
	    if ( cmd == "list") {
	    	if( SendMsg( sockfd, "GetListClient" ) < 0 )
				break;
	    } else if ( cmd == "send" ) {

	    } else if ( cmd == "help" ) {
	    	cout << "Available command:" << endl;
	    	cout << "	1.list		Get the list of clients" << endl;
	    	cout << "	2.send		send message to a client" << endl;
	    	cout << "	3.help		show usage" << endl;
	    }

	}
	

	close(sockfd);
	
	return 0;
}