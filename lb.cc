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
   // Takes, as arguments, 1 or more servers 
   vector<int> servers; 
   if (argc < 2) {
      cout << "Must include names of available servers as HTONS convertable numbers " << endl; 
      exit(1); 
   }
   for (int i = 1; i < argc; i++) {
      string num = argv[i]; 
      int num_i = stoi(num); 
      servers.push_back(num_i); 
   }
   // Creates a TCP style socket 
   // AF_INET = IPv4 Protocal Family 
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
   sockaddr_in serverAddress; 
   serverAddress.sin_family = AF_INET; 
   // Converts the int address to a byte readable thing
   serverAddress.sin_port = htons(80); 
   // Lets it listen to all IPs 
   serverAddress.sin_addr.s_addr = INADDR_ANY; 

   // Now, we actually create the server 
   bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
   int server_counter = 0; 
   while(1) {
      if (server_counter >= servers.size()) {
         server_counter = 0; 
      }
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
      // GRAB THESE FROM THE LIST OF BE SERVERS 
      beAddress.sin_port = htons(servers[server_counter]); 

      // Now, we actually create the server 
      bind(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress)); 

      connect(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress));
      send(beSocket, buffer, strlen(buffer), 0);
      listen(beSocket, beAddress.sin_port);
      char buffer2[1024] = {0}; 
      recv(beSocket, buffer2, sizeof(buffer2), 0); 
      cout << "Message from Be: " << buffer2 << endl; 
      send(clientSocket, buffer2, sizeof(buffer2), 0); 


      close(clientSocket); 
      close(beSocket); 
      server_counter++; 
   }
   
   close(serverSocket); 
   return 0;
}


