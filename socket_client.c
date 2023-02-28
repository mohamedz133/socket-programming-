//include the needed headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>

//define some macros
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

int main() {
	
	 //set the following 
	 const char *server_hostname = "127.100.10.1";
	 const char *server_portnum = "8090"  ;  
	
	 //configure a remote address for connection
	 printf("Configuring remote address...\n");
	 struct addrinfo hints;
	 memset(&hints, 0, sizeof(hints));
	 hints.ai_socktype =SOCK_STREAM; 
	 struct addrinfo *peer_address;
	 if (getaddrinfo(server_hostname, server_portnum, &hints, &peer_address)) {
		 fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }
	 
	 //create clietnt socket
	 printf("Creating socket...\n");
	 SOCKET socket_peer;
	 socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
	                               
	 if (!ISVALIDSOCKET(socket_peer)) {
		 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }
	 
	 //Establish a connection to the remote server
	 printf("Connecting...\n");
	 if ( connect ( socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) { 
	                                               
		 fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }
	 freeaddrinfo(peer_address);
	 
	 printf("Connected.\n");
	 

	 //send request to server
	 printf("Sending request...\n");
	 const char *request = " Mohamed_Ahmed_Zaki \n";  //send your own full name to the server
	 int bytes_sent = send(socket_peer, request, strlen(request), 0);
	 printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(request));
	 
	 //receive response from server
	 char read[1024];
	 recv(socket_peer, read, 1024, 0);
     int bytes_received = strlen(read);
	 if (bytes_received < 1)
	 {
		 printf("Connection closed by peer.\n");
     }
	 printf("Received (%d bytes): %.*s", bytes_received, bytes_received, read);
	 
	 //close the client connection
	 printf("Closing socket...\n");
     CLOSESOCKET(socket_peer);
	 
	 printf("Finished.\n");
     return 0;
}

	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
