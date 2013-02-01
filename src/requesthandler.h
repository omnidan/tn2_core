/*
 * =====================================================================================
 *
 *       Filename:  requesthandler.h
 *
 *    Description:  Request handler headers
 *
 *        Version:  0.2
 *        Created:  27/01/13 21:48:26
 *       Revision:  none
 *       Compiler:  g++
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Internal headers
#include "etc.h"
#include "json/json.h"

enum RequestMethod { GET, HEAD, UNSUPPORTED };
enum RequestLevel { SIMPLE, FULL };
enum RequestType { HTTP, TN };

struct Request {
 enum RequestMethod method;
 enum RequestLevel level;
 enum RequestType type;
 char *resource;
 int status;
};

class RequestHandler {
private:
 Request request;
 Json::Reader jReader;
 Json::Value jRoot;
 char *clientip;
 void InitRequest(Request *);
 void FreeRequest(Request *);
 bool outputHTTPHeader(int, Request *);
 bool parseHTTPHeader(char *, Request *);
 bool parseJSON();
public:
 RequestHandler(int, char*);
 ~RequestHandler();
 bool handle(int);
};

#endif /* REQUESTHANDLER_H */
