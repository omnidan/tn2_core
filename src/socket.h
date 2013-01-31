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

typedef struct {
 int connection;
 int listener;
 sockaddr_in client;
 char *cip;
} conndata;

// Internal headers
#include "requesthandler.h"

class Socket {
private:
 conndata tdata;
 pid_t pid;
 void newconn();
public:
 Socket(int);
 int loop();
};

#endif /* SOCKET_H */
