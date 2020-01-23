#imports the socket Module
from socket import *

#creates the Server Socket
serverSocket = socket(AF_INET, SOCK_STREAM)
#Assigns a port number
port = 6987

#Binds the port to the socket so it can send to the socket
serverSocket.bind(("", port))
#allows for only one connection at a time
serverSocket.listen(1)

while True:
    print('Ready to serve...')
    #creates the web address and the Connection Socket
    connectionSocket, addr = serverSocket.accept()
    try:
        #Gets and prints the Request Message from the client
        message = connectionSocket.recv(1024)
        print('Message is: ', message)

        #Gets and prints the filename from the message and splits the message in two bc filename is in the back half
        filename = message.split()[1]
        print('File name is: ', filename)

        #puts the contents of the file into F
        f = open(filename[1:])

        #F is then put into a buffer
        output = f.read()

        # Send the HTTP response header line to the connection socket
        connectionSocket.send("HTTP/1.1 200 OK\r\n\r\n".encode())

        #sends content of file to the Connection Socket
        for i in range(0, len(output)):
            connectionSocket.send(output[i].encode())

        connectionSocket.send("\r\n".encode())
        #closes the Connection Socket
        connectionSocket.close()
        
    #if the file is not found, it will come to here
    except IOError:
        # Send HTTP response message for file not found
        connectionSocket.send("HTTP/1.1 404 Not Found\r\n\r\n".encode())

        connectionSocket.send("<html><head></head><body><h1>404 Not Found</h1></body></html>\r\n".encode())

        # Close the client connection socket
        connectionSocket.close()

#closes the Server Socket
serverSocket.close()
