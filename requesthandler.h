/*
 * =====================================================================================
 *
 *       Filename:  requesthandler.h
 *
 *    Description:  Request handler headers
 *
 *        Version:  1.0
 *        Created:  27/01/13 21:48:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef REQUESTHANDLER_H
#define	REQUESTHANDLER_H

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
#include "etc.h"
#include "logger.h"

struct Request {

}; // TODO

class RequestHandler {
private:
 Request *request;
 Logger *log;
public:
 RequestHandler(int);
 bool handle();
};

#endif	/* REQUESTHANDLER_H */
