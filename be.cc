// Anna Leitner 
// Back End Server 


#include <fstream>
#include<cstring> 
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h> 
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>

using namespace std; 

/*
* main - Creates socket server 
*
*/
int main(int argc, char** argv) {
   // Creates a TCP style socket 
   // AF_INET = IPv4 Protocal Family 
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
   sockaddr_in serverAddress; 
   serverAddress.sin_family = AF_INET; 
   // Converts the int address to a byte readable thing
   serverAddress.sin_port = htons(8080); 

   //Only listens to the load balancer 

   serverAddress.sin_addr.s_addr = INADDR_ANY;  

   // Now, we actually create the server 
   bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
   // This makes it listen. 
   cout << "be socket: " << serverSocket << endl; 
   cout << "listening " << endl; 
   cout << serverAddress.sin_addr.s_addr << endl; 

   listen(serverSocket, serverSocket);
   // Accept the request 
   int clientSocket = accept(serverSocket, nullptr, nullptr); 

   char buffer[1024] = {0}; 
   recv(clientSocket, buffer, sizeof(buffer), 0); 
   send(clientSocket, "Hello", sizeof("Hello"), 0);
   cout << "Replied with a hello message" << endl; 
   close(serverSocket); 
   return 0;
}