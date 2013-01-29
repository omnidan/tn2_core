/*
 * =====================================================================================
 *
 *       Filename:  requesthandler.cpp
 *
 *    Description:  Handle requests
 *
 *        Version:  1.0
 *        Created:  28/01/13 21:47:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Headers
#include "requesthandler.h"

/* RequestHandler: Constructor */
RequestHandler::RequestHandler(int connection) {
 // Initialise logging system
 log = new Logger("request");
 
 // Initialise request
 InitRequest(&request);
 
 // Handle request and check request type
 if (handle() == true) {
  if (request->type == REQ_TYPE_HTTP) {
   if (request->status == 200) {
    Parse_JSON(&request); // TODO
    OutputHTTPHeader(connection, &request);
    Return_Resource(connection, resource, &request); // TODO, response
   } else Return_Error_Msg(conn, &request); // TODO
  } else if (request->type == REQ_TYPE_TN) {
   Parse_JSON(request); // TODO
  } else log->warning("", "Unknown request type, killing request.");
 } else log->warning("", "Couldn't handle request, killing it.");
 
 // Free request data stored in ram
 FreeRequest(&request);
}

/* InitRequest: Initialises the request data */
void RequestHandler::InitRequest(struct Request *request) {
 request->status = 200;
 request->method = UNSUPPORTED;
 request->useragent = NULL;
 request->referer = NULL;
 request->resource = NULL;
}

/* FreeRequest: Clear the request data */
void RequestHandler::FreeRequest(struct Request *request) {
 if (request->resource) free(request->resource);
 if (request->useragent) free(request->useragent);
 if (request->referer) free(request->referer);
}

/* handle: Handle a request */
bool RequestHandler::handle() {
 char buffer[MAX_REQ_LINE] = {0};
 int rval;
 fd_set fds;
 struct timeval tv;
 
 // Timeout: 5 seconds
 tv.tv_sec = 5;
 tv.tv_usec = 0;
 
 do {
  // Reset FD
  FD_ZERO(&fds);
  FD_SET(conn, &fds);
  
  rval = select(conn+1, &fds, NULL, NULL, &tv); // Select from request
  
  if (rval < 0) log->warning("request_handle", "Couldn't select from request");
  else if (rval == 0) return false; // Timeout, kill request
  else {
   s_readline(conn, buffer, MAX_REQ_LINE - 1);
   sttrim(buffer);
   
   if (buffer[0] == '{') {
    // TODO: This is a TN request, parse it as such
   } else {
    // TODO: This is an HTTP request, parse it as such
   }
   
   // HTTP stuff
   if (buffer[0] == '\0') break; // End of HTTP headers
   if (parseHTTPHeader(buffer, request)) break; // Parse headers
  }
 } while (request->type != SIMPLE);
 return true; // Successfully processed
}

int RequestHandler::outputHTTPHeader(int connection, struct Request *request) {
 char buffer[100];
 sprintf(buffer, "HTTP/1.1 %d OK\r\n", request->status);
 s_writeline(connection, buffer, strlen(buffer));
 s_writeline(connection, "Server: HTTPAPI v0.1\r\n", 22);
 s_writeline(connection, "Content-Type: application/json\r\n", 32);
 s_writeline(connection, "\r\n", 2);
 return 0;
}

bool RequestHandler::parseHTTPHeader(char *buffer, struct Request *request) {
 static bool first_line = true;
 char *temp;
 char *endptr;
 int len;
 if (first_line == true) {
  if (!strncmp(buffer, "GET ", 4)) {
   request->method = GET;
   buffer += 4;
  } else if (!strncmp(buffer, "HEAD ", 5)) {
   request->method = HEAD;
   buffer += 5;
  } else {
   request->method = UNSUPPORTED;
   request->status = 501;
   return false;
  }
  while (*buffer && isspace(*buffer)) buffer++; // Skip the start
  endptr = strchr(buffer, ' ');
  if (endptr == NULL) len = strlen(buffer);
  else len = endptr - buffer;
  
  if (len == 0) {
   request->status = 400;
   return false;
  }
  
  request->resource = calloc(len + 1, sizeof(char));
  strncpy(request->resource, buffer, len);
  
  if (strstr(buffer, "HTTP/")) request->level = FULL;
  else request->level = SIMPLE;
  
  first_line = 0;
  return true;
 }
 
 // More headers aside from the request line
 endptr = strchr(buffer, ":");
 if (endptr == NULL) {
  request->status = 400;
  return false;
 }
 
 temp = calloc((endptr - buffer) + 1, sizeof(char));
 strncpy(temp, buffer, (endptr - buffer));
 stoupper(temp);
 
 buffer = endptr + 1;
 while (*buffer && isspace(*buffer)) ++buffer;
 if (*buffer == '\0') return true;
 
 if (!strcmp(temp, "USER-AGENT")) {
  request->useragent = malloc(strlen(buffer) + 1);
  strcpy(request->useragent, buffer);
 } else if (!strcmp(temp, "REFERER")) {
  request->referer = malloc(strlen(buffer) + 1);
  strcpy(request->referer, buffer);
 }
 
 free(temp);
 return true;
}

bool OutputHTTPHeader(int connection, struct Request *request) {
 char buffer[100];
 
 sprintf(buffer, "HTTP/1.1 %d OK\r\n", request->status);
 s_writeline(connection, buffer, strlen(buffer));
 s_writeline(connection, "Server: HTTPAPI v0.1\r\n", 22);
 s_writeline(connection, "Content-Type: application/json\r\n", 32);
 s_writeline(connection, "\r\n", 2);
 
 return true;
}
