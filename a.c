#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


#define MAX_LISTEN_BACKLOG 1
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]){	
	char buf[BUFFER_SIZE];
	int sfd,n,s;
	struct addrinfo hints;
	struct addrinfo *result;
	struct addrinfo *rp;
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len;
	ssize_t nread;
	int addr_info_error;

	
	memset(&hints,0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	s = getaddrinfo(NULL, argv[1], &hints, &result);
	if (s != 0){
		printf("got error getaddrinfo");
	}

	for( rp = result; rp !=NULL; rp = rp->ai_next){
	  sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
	  if (sfd == -1) perror("could not create socket");
	  if (bind(sfd,rp->ai_addr,rp->ai_addrlen) == 0 ) break;
	  close(sfd);
	}

	if (rp == NULL){
		perror("could not bind");
	}
	freeaddrinfo(result);
	int nsfd;
	listen(sfd,100);


		peer_addr_len = sizeof(struct sockaddr_storage);
		nsfd = accept(sfd,(struct sockaddr*) &peer_addr_len,&peer_addr_len);
	for(;;){

		nread = recv(nsfd,buf,BUFFER_SIZE,0);
		if(nread == -1)
			continue;
		printf("got ...%d\n",nread);
		char host[NI_MAXHOST], service[NI_MAXSERV];
		s = getnameinfo((struct sockaddr*) &peer_addr,
				peer_addr_len,host,NI_MAXHOST,
				service,NI_MAXSERV,NI_NUMERICSERV);

		if (s == 0) printf("recieved %s bytes from host:%s port:%s\n",nread,host,service); 
		if(send(nsfd,buf,nread,0) != nread)
			perror("error sending response");
	}
return 0;
}
