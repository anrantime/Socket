#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>//struct
#include <unistd.h>//close head
#include <string.h>//bzero
#define PORT 8111
#define MESSAGE_SIZE 1024
int main(int argc, char* argv[])
{
	int socket_fd;
	int accept_fd;
	int backlog = 10;
	int ret = -1;
	int flag = 1;
	struct sockaddr_in local_addr, remote_addr;
	char in_buf[MESSAGE_SIZE] = {0,};
	
	//create socket
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		std::cout << "file to create socket!" << std::endl;
		exit(-1);
	}
	//set socket options
	ret = setsockopt(socket_fd, 
					SOL_SOCKET, 
					SO_REUSEADDR, 
					&flag, 
					sizeof(flag));
	if (ret == -1)
	{
		std::cout << "failed to set socket options!" << std::endl;
	}
	//set localaddr
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = PORT;
	local_addr.sin_addr.s_addr = INADDR_ANY;//0 RENHE IP DOU LISTEN
	bzero(&(local_addr.sin_zero),8);
	//bind socket
	ret = bind(socket_fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));
	if (ret == -1)
	{
		std::cout << "failed to bind addr!" << std::endl;
		exit(-1);
	}
	//listen
	ret = listen(socket_fd, backlog);
	if (ret == -1)
	{
		std::cout << "failed to listen socket!" << std::endl;
		exit(-1);
	}
	for(;;)
	{
		socklen_t addr_len = sizeof(struct sockaddr);
		accept_fd  = accept(socket_fd, 
							(struct sockaddr *) &remote_addr, 
							&addr_len);
		for (;;)
		{
			ret = recv(accept_fd, (void *)in_buf, MESSAGE_SIZE, 0);
			if (ret == 0)
				break;
			std::cout << "receive: " << in_buf << std::endl;
		
			send(accept_fd, (void*)in_buf, MESSAGE_SIZE,0);
		}
		close(accept_fd);
	}
	close(socket_fd);
	return 0;
}
