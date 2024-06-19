# Load Balancer, Python 
# Anna Leitner, June 2024 

import socket
import sys
import requests 
import threading
import time

HOST, PORT = '', 80
servers = []
lock = threading.Lock()

def health_check(interval, url): 
   while(True): 
      lock.acquire()
      for i in range(len(servers)): 
         health = requests.get(url + ":" + str(servers[i][0])) 
         if (str(health) != "<Response [200]>"): 
            print("health is: ")
            print(health)
            servers[i] = (servers[i][0], 0)
         else: 
            print("healthy")
            servers[i] = (servers[i][0], 1)
      lock.release()      
      time.sleep(interval)  

lock.acquire()
lb = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
lb.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
lb.bind((HOST, PORT)) 
lb.listen(1) 
for i in range(len(sys.argv) - 2): 
   if (i != 0): 
      servers.append((int(sys.argv[i]), 1))
interval = int(sys.argv[len(sys.argv) - 2])
healthcode_url = sys.argv[len(sys.argv) - 1] 
health_t = threading.Thread(target=health_check, args=(interval, healthcode_url), daemon = True)
server_counter = 0
lock.release()
health_t.start()
while True: 
   lock.acquire()
   if (server_counter >= len(servers)): 
      server_counter = 0
   while (servers[server_counter][1] == 0): 
      server_counter+=1 
      if (server_counter > len(servers)): 
         server_counter = 0    
         lock.release()
   lock.release()
   lock.acquire()
   client_connection, client_address = lb.accept() 
   # print("client connection: " + client_connection)
   request_data = client_connection.recv(1024) 
   print("Recived a message from " +request_data.decode('utf-8'))
   data = request_data.decode('utf-8')
   http_response = (b"""\
   HTTP/1.1 200 OK 

   Message Recived: """ + request_data) 
   lock.release()
   lock.acquire()
   BEHOST, BEPORT = '', servers[server_counter][0]
   be = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   be.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   be.connect((BEHOST, BEPORT))
   print("sending1")
   be.sendall(request_data)
   be_data = be.recv(1024) 
   print("Recived: " + be_data.decode('utf-8') + "from be server: ")
   print("sending2")
   client_connection.sendall(be_data) 
   print("sent")
   client_connection.close()
   print("here")
   be.close()
   print("no here")
   server_counter+=1
   lock.release()
   print("what")

 
      
