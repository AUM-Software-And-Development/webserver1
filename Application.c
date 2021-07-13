// System/compiler headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKETBUILD_FAILURE EXIT_FAILURE

int main()

{

struct sockaddr_in IncomingSocketAddress;
int IncomingSocketAddressLength = sizeof(IncomingSocketAddress);
int ListeningSocket, AcceptingSocket;
const int TransportAddress = 8080;

	// Attempt to define a socket
	// (domain, type, protocol)
	if ((ListeningSocket =
	     socket
	    (
	     AF_INET, SOCK_STREAM,
	     0
	    )) < 0) {
	          perror("The socket request was not valid.");
		  exit(SOCKETBUILD_FAILURE);
		  /* The address family is set to internet
		     and then takes a stream of data.
		     With only one virtual circuit service as part of the OSI,
		     there is no variance in protocol, thus an argument of 0. */
	}

	// Attempt to name a socket (assign its transport address [aka port number])
	// This Function/Method is commonly called binding:

	memset((char*)&IncomingSocketAddress, 0, IncomingSocketAddressLength);
	IncomingSocketAddress.sin_family = AF_INET;
	IncomingSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	IncomingSocketAddress.sin_port = htons(TransportAddress);	
/* sin_addr is the mechanical object, or interface IP.
   INADDR_ANY sets this to be OS defined behavior; 0.0.0.0 
   htonl() converts a long datatype/address into a network readable byte stream
   htons() converts a short/transport address into a network readable byte stream. */
	// (socket, address, size)
	if (bind
	   (
	    ListeningSocket,
	    (struct sockaddr*)&IncomingSocketAddress,
	    IncomingSocketAddressLength
	   ) < 0) {
		perror("The socket could not be bound to the address requested.");
		exit(SOCKETBUILD_FAILURE);}

	// Attempt to set the server to catch incoming connection requests
	// (socket, connection maximum)
	if (listen
	   (
	    ListeningSocket, 3
	   ) < 0) {
		perror("Server could not set up listening environment.");
		exit(SOCKETBUILD_FAILURE);}


long ServerBuffer;
char ConnectionBuffer[30000] = {0};
const char* ReceivedMessage = "Received and accepted your request.";

do
{
	
	// Loops until flagged
	printf("\nNo new connection yet\n\n");

	// (socket, incoming/requestor address, length)
	if ((AcceptingSocket = 
	     accept
	    (
	     ListeningSocket,
	     (struct sockaddr*)&IncomingSocketAddress,
	     (socklen_t*)&IncomingSocketAddressLength)
	    ) < 0) {
	         perror("Server could not setup another socket with which to \
	         accept incoming connections.");
	         exit(SOCKETBUILD_FAILURE);}

	printf("Incoming data...\n");
	ServerBuffer = read(AcceptingSocket, ConnectionBuffer, 30000);
	printf("'%s'\n", ConnectionBuffer);
	write(AcceptingSocket, ReceivedMessage, strlen(ReceivedMessage));

	close(AcceptingSocket);
	printf("\nClosed that connection after sending '%s'.\n\n", ReceivedMessage);

} while (0);




return 0;
}
