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
#include<thread> 


using namespace std; 

// TO DO: 
// 2. Step 3

vector< vector<int> > servers; 
int hold; 
string get_http;

void health_check() { 
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
   sockaddr_in serverAddress; 
   serverAddress.sin_family = AF_INET; 
   // Converts the int address to a byte readable thing
   serverAddress.sin_port = htons(80); 
   // Lets it listen to all IPs 
   serverAddress.sin_addr.s_addr = INADDR_ANY; 
   

   // Now, we actually create the server 
   bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
   while(1) { 
      cout << "servers.size(): " << servers.size() << endl; 
      for (int i = 0; i < servers.size(); i++) {
         int beSocket = socket(AF_INET, SOCK_STREAM, 0); 
         sockaddr_in beAddress; 
         beAddress.sin_family = AF_INET; 
         // // Converts the int address to a byte readable thing
         // GRAB THESE FROM THE LIST OF BE SERVERS 
         beAddress.sin_port = htons(servers[i][0]); 
         cout << "servers: " << servers[i][0] << endl; 

         // Now, we actually create the server 
         bind(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress)); 
         connect(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress));
         char health_buffer[1024] = {0}; 
         send(beSocket, get_http.c_str(), strlen(get_http.c_str())+1, 0); 
         recv(beSocket, health_buffer, sizeof(health_buffer)+1, 0); 

         if (health_buffer != "Hello fr") {
            cout << "Server unhealthy" << endl; 
            servers[i][1] = 0; 
         }
         else {
            servers[i][1] = 1; 
         }
      }
      std::this_thread::sleep_for(std::chrono::seconds(hold));
   }
}


/*
* main - Creates socket server 
*
*/
int main(int argc, char** argv) {
   // Takes, as arguments, 1 or more servers 
   if (argc < 3) {
      cout << "Must include names of available servers as HTONS convertable numbers, a health check url, and a frequency at which to health check" << endl; 
      exit(1); 
   }
   for (int i = 1; i < argc - 2; i++) {
      string num = argv[i]; 
      int num_i = stoi(num); 
      vector<int> tuple; 
      tuple.push_back(num_i); 
      tuple.push_back(1); 
      servers.push_back(tuple); 
   }
   string health_check_url = argv[argc-2]; 
   hold = atoi(argv[argc - 1]); 
   get_http = "GET / HTTP/1.1\r\nHost: " + health_check_url + "\r\nConnection: close\r\n\r\n";
   // Creates a TCP style socket 
   // AF_INET = IPv4 Protocal Family 
   cout << "past the health" << endl; 
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
   thread t1(health_check); 
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
      beAddress.sin_port = htons(servers[server_counter][0]); 

      // Now, we actually create the server 
      bind(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress)); 
      connect(beSocket, (struct sockaddr*)&beAddress, sizeof(beAddress));

      send(beSocket, buffer, strlen(buffer), 0);
      char buffer2[1024] = {0}; 
      recv(beSocket, buffer2, sizeof(buffer2), 0); 
      cout << "Message from Be: " << buffer2 << endl; 
      send(clientSocket, buffer2, sizeof(buffer2), 0); 


      close(clientSocket); 
      close(beSocket); 
      server_counter++; 
   }
   
   close(serverSocket); 
   t1.join(); 
   return 0;
}




