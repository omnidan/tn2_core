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
 *       Compiler:  gcc
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
 // Initialise logging system
 log = new Logger("socket");
 
 // Create socket
 if ((listener=socket(AF_INET, SOCK_STREAM, 0)) < 0) log->error("", "Couldn't create socket.");
 #ifdef DEBUG
 log->debug("", "Created socket.");
 #endif
 
 // Initialise sockaddr
 struct sockaddr_in sockaddr;
 memset(&sockaddr, 0, sizeof(sockaddr));
 sockaddr.sin_port = htons(port); // Set port
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
 // Bind socket
 if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) log->error("", "Couldn't bind to socket.");
 #ifdef DEBUG
 log->debug("", "Bound to socket.");
 #endif
 
 // Listen to socket
 if (listen(listener, LISTENQ) < 0) log->error("", "Couldn't listen to socket.");
 #ifdef DEBUG
 log->debug("", "Listening to socket.");
 #endif
}

/* loop: Main loop for the socket */
int Socket::loop() {
 while (true) {
  // Initialise client struct
  sockaddr_in client;
  client.sin_family = AF_INET;
  socklen_t clen = sizeof(client);
  
  // Accept connection if available
  if ((connection=accept(listener, (struct sockaddr*)&client, &clen)) < 0) log->warning("", "Couldn't accept connection.");
  
  // New connection, fork a new process
  if ((pid=fork()) == 0) {
   #ifdef DEBUG
   log->debug("child", "New connection. Forked child process.");
   #endif
   char *cip;
   if ((cip = inet_ntoa(client.sin_addr)) < 0) log->error("child", "Failed to get peer address.");
   #ifdef DEBUG
   char buffer[64];
   sprintf(buffer, "Peer address: %s", cip);
   log->debug("child", buffer);
   #endif
   if (close(listener) < 0) log->warning("child", "Couldn't close socket.");
   new RequestHandler(connection, cip); // Initialise request handler
   if (close(connection) < 0) log->warning("child", "Couldn't close connection.");
   #ifdef DEBUG
   log->debug("child", "Connection closed. Killing child process.");
   #endif
   exit(EXIT_SUCCESS); // Kill child process
  }
  
  // Cleanup
  if (close(connection) < 0) log->warning("", "Couldn't close connection.");
  waitpid(-1, NULL, WNOHANG);
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
