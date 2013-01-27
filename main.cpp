/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main entry for the http request api
 *
 *        Version:  1.0
 *        Created:  27/01/13 18:18:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  
 *
 * =====================================================================================
 */

// Standard headers
#include <stdlib.h>
#include <string.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "config.h"
#include "logger.h"

// TODO: Move to socket.cpp

/* main: Main function */
int main(int argc, char *argv []) {
 // Initialise variables
 int listener, connection;
 pid_t pid;
 struct sockaddr_in sockaddr;
 
 // Initialise logging system
 Logger *log = new Logger();
 
 // Create socket
 if ((listener=socket(AF_INET, SOCK_STREAM, 0)) < 0) log->error("main", "Couldn't create socket.");
 log->debug("main", "Created socket.");
 
 // Initialise sockaddr
 memset(&sockaddr, 0, sizeof(sockaddr));
 sockaddr.sin_port = htons(PORT); // Set port from config
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
 // Bind socket
 if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) log->error("main", "Couldn't bind to socket.");
 log->debug("main", "Bound to socket.");
 
 // Listen to socket
 if (listen(listener, LISTENQ) < 0) log->error("main", "Couldn't listen to socket.");
 log->debug("main", "Listening to socket.");
 
 // Main loop
 while (true) {
  // Accept connection if available
  if ((connection=accept(listener, NULL, NULL)) < 0) log->warning("main", "Couldn't accept connection.");
  
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
  if (close(connection) < 0) log->warning("main", "Couldn't close connection.");
  waitpid(-1, NULL, WNOHANG);
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
