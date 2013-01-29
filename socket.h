/*
 * =====================================================================================
 *
 *       Filename:  socket.h
 *
 *    Description:  Socket control headers
 *
 *        Version:  1.0
 *        Created:  27/01/13 20:56:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef SOCKET_H
#define	SOCKET_H

// Standard headers
#include <stdlib.h>
#include <string.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

// Internal headers
#include "config.h"
#include "logger.h"
#include "requesthandler.h"

class Socket {
private:
 int listener;
 int connection;
 pid_t pid;
 Logger *log;
public:
 Socket(int);
 int loop();
};

#endif	/* SOCKET_H */
