#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>
#include<unistd.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc,char *argv[]) 
{

	int nk_socket,portno,n,i;

	if(argc<3)
	{

		fprintf(stderr,"usage %s hostname port\n",argv[0]);

		exit(1);

	}

	portno = atoi(argv[2]);
	
	char buffer[255];

	nk_socket = socket(AF_INET,SOCK_STREAM,0);

	if(nk_socket < 0){error("Error n opening socket");}
 
	struct sockaddr_in server_address;

	struct hostent *server;

	server = gethostbyname(argv[1]);

	if(server == NULL){fprintf(stderr,"Error no such host");exit(1);}

	bzero((char*)&server_address,sizeof(server_address));

	server_address.sin_family =AF_INET;
	
	server_address.sin_port = htons(portno);

	bcopy((char *)server->h_addr,(char *)&server_address.sin_addr.s_addr,server->h_length);

	int status = connect(nk_socket,(struct sockaddr * ) &server_address,sizeof(server_address));

	if (status <0 )
	
	{

		error("Connection failed");
	}

	while(1)
	{
		bzero(buffer,255);

		fgets(buffer,255,stdin);

		if(n = write(nk_socket,buffer,strlen(buffer))<0){error("Error in writing");} 

		bzero(buffer,255);

		if(n = read(nk_socket,buffer,255) <0){error("Error in reading");}

		printf("Server:%s\n",buffer);
		
		i = strncmp("bye",buffer,3);
		
		if(i==0){break;}

	}	

	close(nk_socket);

return 0;
}

