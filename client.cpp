#include <iostream>
#include <sys/socket.h>//1connect
#include <sys/types.h>//2connect
#include <netinet/in.h> //struct
#include <string.h>//memset
#include <stdio.h>//gets
#include <unistd.h>//close
#include <arpa/inet.h>//inet
#include <stdio.h>
#define PORT 8111
#define MESSAGE_LEN 1024
using namespace std;
int main(int argc, char* argv[])
{
	int socket_fd;
	int ret = -1;
	char sendbuf[MESSAGE_LEN] = {0,};
	char recvbuf[MESSAGE_LEN] = {0,};
	struct sockaddr_in serverAddr;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		cout << "failed to create socket" << endl;
		exit(-1);
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(socket_fd, 
				(struct sockaddr *)&serverAddr, 
				sizeof(struct sockaddr));
	if (ret < 0)
	{
		cout << "failed to connect!" << endl;
		exit(-1);
	}
	while(1)
	{
		memset(sendbuf, 0, MESSAGE_LEN);
		//gets(sendbuf);
		//cin.getline(sendbuf);
		fgets(sendbuf,MESSAGE_LEN,stdin);
		ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
		if (ret <= 0)
		{
			cout << "failed to send data!" << endl;
			break;		
		}
		//guanbi 1 kill -9 1111      2 duibi
		if (strcmp(sendbuf, "quit") == 0)
		{
			break;
		}
		ret = recv(socket_fd, recvbuf, MESSAGE_LEN, 0);
		recvbuf[ret] = '\0';
		cout << "recv:" << recvbuf << endl;
	}
	close(socket_fd);
	
	
	return 0;
}

