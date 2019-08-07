#include<stdio.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
struct sockaddr_in server_address,client_address;
void error(const char *msg)
{
	perror(msg);
	
	exit(1);
}
void *ftcp(void* fd)
{
	int n ,network_socket = (*(int*)fd);
	char buffer[256];
	while(1)

	{
		bzero(buffer,256);

		if(n = read(network_socket,buffer,256)<0){error("Error in reading");}
		
		printf("Client(%s) : %s ",inet_ntoa(client_address.sin_addr),buffer);


		bzero(buffer,256);

		fgets(buffer,256,stdin);

		if(n=write(network_socket,buffer,strlen(buffer))<0){error("Error in writing");}

		int i = strncmp("bye",buffer,3);

		if(i==0){break;}

	}

	close(network_socket);
	pthread_exit(NULL);
}
int main(int argc ,char *argv[])
{
	if(argc<2){fprintf(stderr,"portno. not given");exit(1);}
	
	socklen_t cli;	
	
	int server_socket,portno,n,network_socket;

	server_socket = socket(AF_INET,SOCK_STREAM,0);
	
	if(server_socket<0){error("Error in creating Socket");exit(1);}

	bzero((char *) &server_address, sizeof(server_address));

	portno = atoi(argv[1]);

	server_address.sin_family = AF_INET;

	server_address.sin_port = htons(portno);
	
	server_address.sin_addr.s_addr = INADDR_ANY;

	if(bind(server_socket,(struct sockaddr*) &server_address,sizeof(server_address))<0){error("Binding failed");}
	listen(server_socket,5);

	while(1)
	{

	cli = sizeof(client_address);

	network_socket = accept(server_socket,(struct sockaddr*)&client_address,&cli);

	if(network_socket <0){error("Error in Accept");}

	pthread_t tid;
	
	pthread_attr_t t;

	pthread_attr_init(&t);

	pthread_attr_setdetachstate(&t,PTHREAD_CREATE_DETACHED);
	
	pthread_create(&tid,&t,ftcp,(void*)&network_socket);
	

	}
	close(server_socket); 
	
return 0;
}


















