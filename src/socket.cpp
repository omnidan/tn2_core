/*
 * =====================================================================================
 *
 *       Filename:  socket.cpp
 *
 *    Description:  Socket control
 *
 *        Version:  1.0
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
 
 while (true) {
  pthread_t thread;
  
  // Accept connection if available
  if ((tdata.connection=accept(tdata.listener, (struct sockaddr*)&tdata.client, &clen)) < 0) {
   if (errno == EWOULDBLOCK) continue; // No connection available, just continue to check.
   else {
    std::cout << "[WARN ] [socket      ] Couldn't accept connection. (ERRNO: " << errno << ")" << std::endl;
    usleep(1000000);
    continue;
   }
  }
  
  // New connection, create a new thread
  pthread_create(&thread, NULL, newconn, (void *)&tdata);
  //pthread_join(thread, NULL);
  //pthread_cancel(thread);
  //close(tdata.connection);
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
