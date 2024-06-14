# Load Balancer, Python 
# Anna Leitner, June 2024 

import socket
import sys

HOST, PORT = '', 80

lb = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
lb.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
lb.bind((HOST, PORT)) 
lb.listen(1) 
servers = []
for i in range(len(sys.argv)): 
   if (i != 0): 
      servers.append(int(sys.argv[i]))
server_counter = 0
while True: 
   if (server_counter >= len(servers)): 
      server_counter = 0
   client_connection, client_address = lb.accept() 
   # print("client connection: " + client_connection)
   request_data = client_connection.recv(1024) 
   print("Recived a message from " +request_data.decode('utf-8'))
   data = request_data.decode('utf-8')
   http_response = (b"""\
   HTTP/1.1 200 OK 

   Message Recived: """ + request_data) 
   BEHOST, BEPORT = '', servers[server_counter]
   be = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   be.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   be.connect((BEHOST, BEPORT))
   be.sendall(request_data)
   be_data = be.recv(1024) 
   print("Recived: " + be_data.decode('utf-8') + " from be server")
   client_connection.sendall(be_data) 
   client_connection.close()
   be.close()
   server_counter+=1