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

#include "config.h"

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

// Internal headers
#include "etc.h"
#include "logger.h"

enum RequestMethod { GET, HEAD, UNSUPPORTED };
enum RequestLevel { SIMPLE, FULL };
enum RequestType { HTTP, TN };

struct Request {
 enum RequestMethod method;
 enum RequestLevel level;
 enum RequestType type;
 char *referer;
 char *useragent;
 char *resource;
 int status;
};

class RequestHandler {
private:
 Request request;
 Logger *log;
 int connection;
 char *clientip;
 void InitRequest(Request *);
 void FreeRequest(Request *);
 bool outputHTTPHeader(int, Request *);
 bool parseHTTPHeader(char *, Request *);
public:
 RequestHandler(int, char*);
 bool handle();
};

#endif /* REQUESTHANDLER_H */
