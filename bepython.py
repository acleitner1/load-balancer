# BE Server for Load Balancer, Python 
# Anna Leitner, June 2024 

import socket

HOST, PORT = '', 8081

be = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
be.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
be.bind((HOST, PORT)) 
be.listen(1) 
print("listening") 

while True: 
   client_connection, client_address = be.accept() 
   request_data = client_connection.recv(1024) 
   print("Recived a message from " +request_data.decode('utf-8'))
   data = request_data.decode('utf-8')
   http_response = (b"""\
   Hello from BE Server""")
   client_connection.sendall(http_response) 
   client_connection.close()