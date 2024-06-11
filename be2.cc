// Anna Leitner 
// Back End Server 
// Identical to all other BE servers except for htons num


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
   serverAddress.sin_port = htons(8082); 
   // Lets it listen to all IPs 
   serverAddress.sin_addr.s_addr = INADDR_ANY; 

   // Now, we actually create the server 
   bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
   // This makes it listen. 
   while (1) {
      listen(serverSocket, 5);
     
      // Accept the request 
      int clientSocket = accept(serverSocket, nullptr, nullptr); 
      char* hello = "Hello from Backend Server"; 
      char buffer[1024] = {0}; 
      recv(clientSocket, buffer, sizeof(buffer), 0); 
      cout << "Recieved Message from Client: " << clientSocket << endl; 
      cout << "Message from client: " << buffer << endl; 

      send(clientSocket, hello, sizeof(hello), 0);
      cout << "Replied with a hello message" << endl; 

      close(clientSocket);
   }
   close(serverSocket); 
   return 0;
}