/*
 * =====================================================================================
 *
 *       Filename:  socket.h
 *
 *    Description:  Socket control headers
 *
 *        Version:  0.2
 *        Created:  27/01/13 20:56:23
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef SOCKET_H
#define	SOCKET_H

#include "config.h"

// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef FORKING
#include <pthread.h>
typedef struct {
 int connection;
 int listener;
 sockaddr_in client;
 char *cip;
} conndata;
#endif

// Internal headers
#include "requesthandler.h"

#ifdef FORKING
void newconn(int, sockaddr_in, char *);
#else
void *newconn(void *);
#endif

class Socket {
private:
 int listener;
 int connection;
 pid_t pid;
public:
 Socket(int);
 int loop();
};

#endif /* SOCKET_H */
