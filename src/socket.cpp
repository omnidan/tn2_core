/*
 * =====================================================================================
 *
 *       Filename:  socket.cpp
 *
 *    Description:  Socket control
 *
 *        Version:  1.1
 *        Created:  27/01/13 20:55:15
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "socket.h"

/* Socket: Constructor */
Socket::Socket(int port) {
 // Clear the file descriptor sets
 FD_ZERO(&master);
 FD_ZERO(&rfds);
 
 // Prevent SIGPIPE from killing the server
 signal(SIGPIPE, SIG_IGN);
 
 // Create socket
 if ((tdata.listener=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
  std::cout << "[ERROR] [socket      ] Couldn't create socket." << std::endl;
  close(tdata.listener);
  exit(EXIT_FAILURE);
 }
 
 // Make the socket non-blocking
 if ((fcntl(tdata.listener, F_SETFL, O_NONBLOCK)) < 0) {
  std::cout << "[ERROR] [socket      ] Couldn't make the socket non-blocking." << std::endl;
  close(tdata.listener);
  exit(EXIT_FAILURE);
 }
 
 // Get rid of address already in use errors
 int on = 1;
 if ((setsockopt(tdata.listener, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0) std::cout << "[WARN ] [socket      ] setsockopt failed, this isn't critical, continuing." << std::endl;
 
 std::cout << "[INFO ] [socket      ] Created socket at port " << port << "." << std::endl;
 
 // Initialise sockaddr
 struct sockaddr_in sockaddr;
 memset(&sockaddr, 0, sizeof(sockaddr));
 sockaddr.sin_port = htons(port); // Set port
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
 // Bind socket
 if (bind(tdata.listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
  std::cout << "[ERROR] [socket      ] Couldn't bind to socket." << std::endl;
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 std::cout << "[DEBUG] [socket      ] Bound to socket." << std::endl;
 #endif
 
 // Listen to socket
 if (listen(tdata.listener, LISTENQ) < 0) {
  std::cout << "[ERROR] [socket      ] Couldn't listen to socket." << std::endl;
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 std::cout << "[DEBUG] [socket      ] Listening to socket." << std::endl;
 #endif
 
 // Add listener to the master set
 FD_SET(tdata.listener, &master);
 fdmax = tdata.listener;
}

void *newconn(void *ptr) {
 conndata *tdata;
 tdata = (conndata *)ptr;
 
 // Deallocate resources upon return
 if ((pthread_detach(pthread_self())) < 0) std::cout << "[WARN ] [child       ] Couldn't detach thread." << std::endl;
 
 #ifdef DEBUG
 std::cout << "[DEBUG] [child       ] New connection. Forked child process." << std::endl;
 #endif
 
 if ((tdata->cip = inet_ntoa(tdata->client.sin_addr)) < 0) {
  std::cout << "[ERROR] [child       ] Failed to get peer address." << std::endl;
  pthread_exit(0);
 } 
 #ifdef DEBUG
 std::cout << "[DEBUG] [child       ] Peer address: " << tdata->cip << std::endl;
 #endif
 
 RequestHandler(tdata->connection, tdata->cip); // Initialise request handler
 if (close(tdata->connection) < 0) std::cout << "[WARN ] [child       ] Couldn't close connection." << std::endl;
 #ifdef DEBUG
 else std::cout << "[DEBUG] [child       ] Connection closed. Killing child process." << std::endl;
 #endif
 pthread_exit(0); // Kill child process
}

/* loop: Main loop for the socket */
int Socket::loop() {
 // Initialise client struct
 sockaddr_in client;
 client.sin_family = AF_INET;
 socklen_t clen = sizeof(client);
 tdata.cip = NULL;
 pthread_t thread;
 
 while (true) {
  rfds = master;
  
  // Wait for activities on the sockets, timeout NULL, so it waits forever
  if (((select(fdmax+1, &rfds, NULL, NULL, NULL)) < 0) && (errno != EINTR)) { std::cout << "[WARN ] [socket      ] Couldn't select connection. (ERRNO: " << errno << ")" << std::endl;
   exit(-1);
  }
  // Something happened on the listener socket, incoming connection
  if (FD_ISSET(tdata.listener, &rfds)) {
   // Accept connection if available
   if ((tdata.connection=accept(tdata.listener, (struct sockaddr*)&tdata.client, &clen)) < 0) {
    if (errno != EWOULDBLOCK) {
     std::cout << "[WARN ] [socket      ] Couldn't accept connection. (ERRNO: " << errno << ")" << std::endl;
     usleep(1000000);
     continue;
    }
   } else {
    pthread_create(&thread, NULL, newconn, (void *)&tdata);
    //pthread_join(thread, NULL);
    //pthread_cancel(thread);
   }
  }
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
