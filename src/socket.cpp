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
 // Create socket
 if ((listener=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
  printf("[ERROR] [socket] Couldn't create socket.\n");
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 printf("[DEBUG] [socket] Created socket.\n");
 #endif
 
 // Initialise sockaddr
 struct sockaddr_in sockaddr;
 memset(&sockaddr, 0, sizeof(sockaddr));
 sockaddr.sin_port = htons(port); // Set port
 sockaddr.sin_family = AF_INET;
 sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
 // Bind socket
 if (bind(listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
  printf("[ERROR] [socket] Couldn't bind to socket.\n");
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 printf("[DEBUG] [socket] Bound to socket.\n");
 #endif
 
 // Listen to socket
 if (listen(listener, LISTENQ) < 0) {
  printf("[ERROR] [socket] Couldn't listen to socket.\n");
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 printf("[DEBUG] [socket] Listening to socket.\n");
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
  if ((connection=accept(listener, (struct sockaddr*)&client, &clen)) < 0) printf("[WARN] [socket] Couldn't accept connection.\n");
  
  // New connection, fork a new process
  if ((pid=fork()) == 0) {
   #ifdef DEBUG
   printf("[DEBUG] [child] New connection. Forked child process.\n");
   #endif
   char *cip;
   if ((cip = inet_ntoa(client.sin_addr)) < 0) {
    printf("[ERROR] [child] Failed to get peer address.\n");
    exit(EXIT_FAILURE);              
   } 
   #ifdef DEBUG
   printf("[DEBUG] [child] Peer address: %s\n", cip);
   #endif
   if (close(listener) < 0) printf("[WARN] [child] Couldn't close socket.\n");
   new RequestHandler(connection, cip); // Initialise request handler
   if (close(connection) < 0) printf("[WARN] [child] Couldn't close connection.\n");
   #ifdef DEBUG
   printf("[DEBUG] [child] Connection closed. Killing child process.\n");
   #endif
   exit(EXIT_SUCCESS); // Kill child process
  }
  
  // Cleanup
  if (close(connection) < 0) printf("[WARN] [socket] Couldn't close connection.\n");
  waitpid(-1, NULL, WNOHANG);//waitpid(-1, NULL, WNOHANG);
  signal(SIGCHLD, SIG_IGN);
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
