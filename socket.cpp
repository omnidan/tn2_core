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
 log->debug("", "Created socket.");
 
 // Initialise sockaddr
 struct sockaddr_in sockaddr;
 memset(&sockaddr, 0, sizeof(sockaddr));
 sockaddr.sin_port = htons(port); // Set port
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
 // Bind socket
 if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) log->error("", "Couldn't bind to socket.");
 log->debug("", "Bound to socket.");
 
 // Listen to socket
 if (listen(listener, LISTENQ) < 0) log->error("", "Couldn't listen to socket.");
 log->debug("", "Listening to socket.");
}

/* loop: Main loop for the socket */
int Socket::loop() {
 while (true) {
  // Accept connection if available
  if ((connection=accept(listener, NULL, NULL)) < 0) log->warning("", "Couldn't accept connection.");
  
  // New connection, fork a new process
  if ((pid=fork()) == 0) {
   log->debug("child", "New connection. Forked child process.");
   if (close(listener) < 0) log->warning("child", "Couldn't close socket.");
   // TODO: Call request handler here
   if (close(connection) < 0) log->warning("child", "Couldn't close connection.");
   log->debug("child", "Connection closed. Killing child process.");
   exit(EXIT_SUCCESS); // Kill child process
  }
  
  // Cleanup
  if (close(connection) < 0) log->warning("", "Couldn't close connection.");
  waitpid(-1, NULL, WNOHANG);
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
