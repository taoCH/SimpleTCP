#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>						
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sstream>
#include <algorithm>    // std::find

using namespace std;

pthread_t thread;

unsigned short port = 8080;	

static vector<int> ListClient;

enum State {
	FREE,
	BUSY
};

template <typename T>
string NumberToString ( T Number )
{
     ostringstream ss;
     ss << Number;
     return ss.str();
}

int TCPConnInit( int &sockfd )
{
	struct sockaddr_in my_addr;

	bzero(&my_addr, sizeof(my_addr));	
	my_addr.sin_family = AF_INET;	// ipv4
	my_addr.sin_port   = htons(port);	
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);   // 创建通信端点：套接字

	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}

	// 绑定
	int err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if( err_log != 0)
	{
		perror("binding");
		close(sockfd);		
		exit(-1);
	}
	
	err_log = listen(sockfd, 10); // 监听，监听套接字改为被动
	if(err_log != 0)
	{
		perror("listen");
		close(sockfd);		
		exit(-1);
	}	
	
	printf("listen client @port=%d...\n",port);

}	

void* RevMsg( void *cfd )
{
	int connfd = *((int*)cfd);
	int dest;
	State state = FREE;
	char recv_buf[512] = "";
	string MsgBack;
	while( recv(connfd, recv_buf, sizeof(recv_buf), 0) > 0 ) // 接收数据
	{
		printf("Receive data from client %d\n", connfd);

		if( state == FREE && strcmp(recv_buf, "GetListClient") == 0 ) {
			for( vector<int>::iterator i = ListClient.begin(); i != ListClient.end(); i++ ) {
				MsgBack += NumberToString(*i);
				if( *i == connfd )
					MsgBack += "(me)" ;
				if( i < ( ListClient.end() - 1) )
					MsgBack += ",\n" ;
			}
			dest = connfd;
			printf("Send list of clients to client %d\n", connfd);

			if ( send(dest, MsgBack.c_str(), MsgBack.size(), 0) < 0)   // 向服务器发送信息
				cout<<"failed to send back"<<endl;
		} else if ( state == FREE && strcmp(recv_buf, "Send") ) {
			dest = (recv_buf[4]-'0')*100 + (recv_buf[5]-'0')*10 + (recv_buf[6]-'0');
			if ( find( ListClient.begin(), ListClient.end(), dest ) != ListClient.end() ) {
				if ( send(dest, recv_buf+7, strlen(recv_buf)-7, 0) < 0)   // 向服务器发送信息
					cout<<"failed to send back"<<endl;
			}
		}

		

		/*
		int TargetClient = recv_buf[0] - '0';
		recv_buf[0] = connfd + '0';
		if ( send(TargetClient, recv_buf, strlen(recv_buf), 0) < 0)   // 向服务器发送信息
			cout<<"failed to send back"<<endl;
		*/
	}
		
	close(connfd);     //关闭已连接套接字
	printf("client closed!\n");
	return NULL;
}

int main(int argc, char *argv[])
{	
	int sockfd;
	int res;

	res = TCPConnInit( sockfd );

	while(1)
	{	
	
		struct sockaddr_in client_addr;		   
		char cli_ip[INET_ADDRSTRLEN] = "";	   
		socklen_t cliaddr_len = sizeof(client_addr);    
		
		int connfd;
		// 等待连接
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);       
		if(connfd < 0)
		{
			perror("accept");
			continue;
		}

		ListClient.push_back( connfd );
		cout<<"Insert new client : "<<connfd<<endl;
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("----------------------------------------------\n");
		printf("client ip=%s,port=%d\n", cli_ip,ntohs(client_addr.sin_port));

		int *cfd = &connfd;
		
		res = pthread_create( &thread, NULL, RevMsg, cfd );
     	if(res)
     	{
         	fprintf(stderr,"Error - pthread_create() return code: %d\n",res);
     	    exit(EXIT_FAILURE);
     	}
	}
	
	close(sockfd);         //关闭监听套接字
	
	return 0;
}