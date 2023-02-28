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

//define some macros (just for program portability so that it works on windows, but we're only considered with Linux in this Assignment)
#define ISVALIDSOCKET(s) ((s) >= 0)   
#define CLOSESOCKET(s) close(s)  
#define SOCKET int    //A socket descriptor is represented by a standard file descriptor of type int
#define GETSOCKETERRNO() (errno)  //When a socket function has an error,the error number gets stored in the thread-global errno variable.

int main() {

	
	
	 //figure out the local address that our web server should bind to
	 printf("Configuring local address...\n");
	 struct addrinfo hints;
	 memset(&hints, 0, sizeof(hints));
	 hints.ai_family = AF_INET;
	 hints.ai_socktype = SOCK_STREAM; 
	 hints.ai_flags = AI_PASSIVE;
	 struct addrinfo *bind_address;
	 getaddrinfo(0, "8090", &hints, &bind_address); //port number is 8090, if it's busy on your device change it 
                                                    //you will know whether it's busy or not when you run the code	 
	 

	 //create the socket
	 printf("Creating socket...\n");
	 SOCKET socket_listen;
	 socket_listen = socket (bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
	                           
	 
	 //check that the call to socket() was successful
	 if (!ISVALIDSOCKET(socket_listen)) 
	 {
		 fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }
	 //call bind() to associate it with our address from getaddrinfo():
	 printf("Binding socket to local address...\n");
	 if (bind(socket_listen ,bind_address->ai_addr, bind_address->ai_addrlen)) 
	 {
		 fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }
	 freeaddrinfo(bind_address);
	 
	 //start listening for connections
	 printf("Listening...\n");
	 if (listen(socket_listen, 10) < 0) 
	 {
		 fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }

     //accept any incoming connection
     printf("Waiting for connection...\n");
	 struct sockaddr_storage client_address;
	 socklen_t client_len = sizeof(client_address);
	 SOCKET socket_client = accept(socket_listen,(struct sockaddr*) &client_address, &client_len);
	                                                          
	 if (!ISVALIDSOCKET(socket_client)) 
	 {
		 fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
		 return 1;
	 }

     //We read this request using the recv() function
     printf("Reading request...\n");
	 char request[1024];
	 recv(socket_client, request, 1024, 0);
	 int bytes_received = strlen(request);
	 printf("Received %d bytes.\n", bytes_received);
	 
	 //print the client's request to the console
	 printf("%.*s", bytes_received, request);   
	 
	 //send our response back
	 printf("Sending response...\n");
	 char response[] = "Hello   ";
	 strcat(response,request);
	 int bytes_sent =  send(socket_client, response, strlen(response), 0);
	 printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
	 
	 
	 //close the client connection
	 printf("Closing connection...\n");
	 CLOSESOCKET(socket_client);
	 
	 //close the listening socket too
	 printf("Closing listening socket...\n");
     CLOSESOCKET(socket_listen);
			 
	 printf("Finished.\n");
     return 0;
}
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 