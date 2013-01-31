/*
 * =====================================================================================
 *
 *       Filename:  socket.cpp
 *
 *    Description:  Socket control
 *
 *        Version:  0.2
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
 if ((tdata.listener=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
 if (bind(tdata.listener, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
  printf("[ERROR] [socket] Couldn't bind to socket.\n");
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 printf("[DEBUG] [socket] Bound to socket.\n");
 #endif
 
 // Listen to socket
 if (listen(tdata.listener, LISTENQ) < 0) {
  printf("[ERROR] [socket] Couldn't listen to socket.\n");
  exit(EXIT_FAILURE);
 }
 #ifdef DEBUG
 printf("[DEBUG] [socket] Listening to socket.\n");
 #endif
}

#ifdef FORKING
void newconn(int connection, int listener, sockaddr_in client, char *cip) {
#else
void *newconn(void *ptr) {
conndata *tdata;
tdata = (conndata *)ptr;
int connection = tdata->connection;
int listener = tdata->listener;
sockaddr_in client = tdata->client;
char *cip = tdata->cip;
#endif
// newconn
   #ifdef DEBUG
   printf("[DEBUG] [child] New connection. Forked child process.\n");
   #endif
   if ((cip = inet_ntoa(client.sin_addr)) < 0) {
    printf("[ERROR] [child] Failed to get peer address.\n");
    exit(EXIT_FAILURE);
   } 
   #ifdef DEBUG
   printf("[DEBUG] [child] Peer address: %s\n", cip);
   #endif
   if (close(listener) < 0) printf("[WARN] [child] Couldn't close socket.\n");
   RequestHandler(connection, cip); // Initialise request handler
   if (close(connection) < 0) printf("[WARN] [child] Couldn't close connection.\n");
   #ifdef DEBUG
   printf("[DEBUG] [child] Connection closed. Killing child process.\n");
   #endif
// end newconn
#ifdef FORKING
exit(EXIT_SUCCESS); // Kill child process
#else
pthread_exit(0); // Kill child process
#endif
}

/* loop: Main loop for the socket */
int Socket::loop() {
 // Initialise client struct
 sockaddr_in client;
 client.sin_family = AF_INET;
 socklen_t clen = sizeof(client);
 tdata.cip = NULL;
 
 while (true) {
  // Accept connection if available
  if ((tdata.connection=accept(tdata.listener, (struct sockaddr*)&tdata.client, &clen)) < 0) { printf("[WARN] [socket] Couldn't accept connection.\n"); continue; }
  
  #ifdef FORKING
  // New connection, fork a new process
  // TODO: Check if a fork limit is needed here
  if ((pid=fork()) == 0) {
   newconn(tdata.connection, tdata.listener, tdata.client, tdata.cip);
  }
  waitpid(-1, NULL, WNOHANG);
  signal(SIGCHLD, SIG_IGN);
  #else
  // New connection, create a new thread
  pthread_t thread;
  pthread_create(&thread, NULL, newconn, (void *)&tdata);
  pthread_join(thread, NULL);
  #endif
  
  // Cleanup
  if (close(tdata.connection) < 0) { printf("[WARN] [socket] Couldn't close connection.\n"); continue; }
 }
 
 return EXIT_FAILURE; // Something bad happened, exit parent
}
