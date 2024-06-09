// balancer.cc - Load Balancer
// Anna Leitner, June 2024

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
   serverAddress.sin_port = htons(8000); 
   // Lets it listen to all IPs 
   serverAddress.sin_addr.s_addr = INADDR_ANY; 

   // Now, we actually create the server 
   bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
   // This makes it listen. 
   listen(serverSocket, 5);
     
   // Accept the request 
   int clientSocket = accept(serverSocket, nullptr, nullptr); 

   char buffer[1024] = {0}; 
   recv(clientSocket, buffer, sizeof(buffer), 0); 
   cout << "Recieved Message from Client: " << clientSocket << endl; 
   cout << "Message from client: " << buffer << endl; 

   int beSocket = socket(AF_INET, SOCK_STREAM, 0); 
   sockaddr_in beAddress; 
   beAddress.sin_family = AF_INET; 
   // // Converts the int address to a byte readable thing
   beAddress.sin_port = htons(8080); 

   // //Only listens to the load balancer 
  // beAddress.sin_addr.s_addr = INADDR_ANY;  
   // cout << "listening : " << beAddress.sin_addr.s_addr << endl; 

   // // // Now, we actually create the server 
   bind(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress)); 

   connect(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress));
   // cout << "Sending message to be: " << beSocket << endl; 
   send(beSocket, buffer, strlen(buffer), 0);
   listen(beSocket, beAddress.sin_port);
   char buffer2[1024] = {0}; 
   recv(beSocket, buffer2, sizeof(buffer2), 0); 
   clientSocket = accept(beSocket, nullptr, nullptr); 
   cout << "Message from Be: " << buffer2 << endl; 


   //close(serverSocket); 
   //close(beSocket); 
   close(clientSocket); 
   return 0;
}


