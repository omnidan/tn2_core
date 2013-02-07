/*
 * =====================================================================================
 *
 *       Filename:  socket.h
 *
 *    Description:  Socket control headers
 *
 *        Version:  1.1
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
#include <errno.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

// Internal headers
#include "requesthandler.h"

typedef struct {
 int connection;
 int listener;
 sockaddr_in client;
 char *cip;
} conndata;

class Socket {
private:
 conndata tdata;
 pid_t pid;
 fd_set master;
 fd_set rfds;
 int fdmax;
public:
 Socket(int);
 int loop();
};

#endif /* SOCKET_H */
