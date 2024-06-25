# Load Balancer, Python 
# Anna Leitner, June 2024 

import socket
import sys
import requests 
import threading
import time
# TO DO: 
# Wrap everything in a function 
# Each time called, create a new thread that runs the code. 
# This will probably work better 
HOST, PORT = '', 80
servers = []
server_lock = threading.Lock()
sending_lock = threading.Lock()
server_counter = 0

def health_check(interval, url): 
   while(True): 
      print("here")
      server_lock.acquire()
      for i in range(len(servers)): 
         health = requests.get(url + ":" + str(servers[i][0])) 
         if (str(health) != "<Response [200]>"): 
            print("health is: ")
            print(health)
            servers[i] = (servers[i][0], 0)
         else: 
            print("healthy")
            servers[i] = (servers[i][0], 1)
      server_lock.release()      
      time.sleep(interval)  

def make(server_counter, lb): 
   server_lock.acquire()
   if (server_counter >= len(servers)): 
      server_counter = 0
   server_lock.release()   
   while (servers[server_counter][1] == 0): 
      server_counter+=1 
      if (server_counter > len(servers)): 
         server_counter = 0    
   sending_lock.acquire()
   client_connection, client_address = lb.accept() 
   print("client connection: " + client_address[0])
   request_data = client_connection.recv(1024) 
   print("Recived a message from " +request_data.decode('utf-8'))
   data = request_data.decode('utf-8')
   http_response = (b"""\
   HTTP/1.1 200 OK Message Recived: """ + request_data) 
   BEHOST, BEPORT = '', servers[server_counter][0]
   be = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   be.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   be.connect((BEHOST, BEPORT))
   be.sendall(request_data)
   be_data = be.recv(1024) 
   print("Recived: " + be_data.decode('utf-8') + "from be server: " + str(BEPORT))
   client_connection.sendall(be_data) 
   client_connection.close()
   be.close()
   server_counter+=1
   sending_lock.release()

def handle(server_counter): 
   lb = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
   lb.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   lb.bind((HOST, PORT)) 
   lb.listen(1) 


   while True: 
      # should create threads for each client 
      handler = threading.Thread(target = make, args=(server_counter, lb))
      handler.start() 
      handler.join()
      
server_lock.acquire()
for i in range(len(sys.argv) - 2): 
   if (i != 0): 
      servers.append((int(sys.argv[i]), 1))    
interval = int(sys.argv[len(sys.argv) - 2])
healthcode_url = sys.argv[len(sys.argv) - 1] 
# lb = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
# lb.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# lb.bind((HOST, PORT)) 
# lb.listen(1) 
health_t = threading.Thread(target=health_check, args=(interval, healthcode_url), daemon = True)
health_t.start()
server_lock.release()  

handle(server_counter)




 
      
