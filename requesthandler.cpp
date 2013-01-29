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
RequestHandler::RequestHandler(int iconnection, char *cip) {
 // Initialise logging system
 log = new Logger("request");

 clientip = cip;
 connection = iconnection;
 
 // Initialise request
 InitRequest(&request);
 
 // Handle request and check request type
 if (handle() == true) {
  log->debug("", "handle() returned true, continuing!");
  if (request.type == HTTP) {
   log->debug("http", "This is an HTTP request.");
   if (request.status == 200) {
    log->debug("http", "Status 200, returning result.");
    //Parse_JSON(&request); // TODO
    outputHTTPHeader(connection, &request);
    s_writeline(connection, "{}", 2);
    log->debug("http", "Answered to request with HTTP.");
    //Return_Resource(connection, resource, &request); // TODO, response
   } //else Return_Error_Msg(conn, &request); // TODO
  } else if (request.type == TN) {
   log->debug("tn", "This is a TN request.");
   //Parse_JSON(request); // TODO
   s_writeline(connection, "{}", 2);
   log->debug("tn", "Answered to request with TN.");
  } else log->warning("", "Unknown request type, killing request.");
 } else log->warning("", "Couldn't handle request, killing it.");
 
 // Free request data stored in ram
 FreeRequest(&request);
}

/* InitRequest: Initialises the request data */
void RequestHandler::InitRequest(Request *request) {
 request->status = 200;
 request->method = UNSUPPORTED;
 request->useragent = NULL;
 request->referer = NULL;
 request->resource = NULL;
 log->debug("", "Initialised request.");
}

/* FreeRequest: Clear the request data */
void RequestHandler::FreeRequest(Request *request) {
 if (request->resource) free(request->resource);
 if (request->useragent) free(request->useragent);
 if (request->referer) free(request->referer);
 log->debug("", "Free'd request.");
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
  FD_SET(connection, &fds);
  
  rval = select(connection+1, &fds, NULL, NULL, &tv); // Select from request
  
  if (rval < 0) log->warning("request_handle", "Couldn't select from request");
  else if (rval == 0) return false; // Timeout, kill request
  else {
   s_readline(connection, buffer, MAX_REQ_LINE - 1);
   
   if (buffer[0] == '{') {
    request.type = TN;
    return true;
   } else {
    request.type = HTTP;
    sttrim(buffer);
   }
   
   // HTTP stuff
   if (buffer[0] == '\0') break; // End of HTTP headers
   if (parseHTTPHeader(buffer, &request)) break; // Parse headers
  }
 } while (request.level != SIMPLE);
 return true; // Successfully processed
}

bool RequestHandler::outputHTTPHeader(int connection, Request *request) {
 char buffer[100];
 
 sprintf(buffer, "HTTP/1.1 %d OK\r\n", request->status);
 s_writeline(connection, buffer, strlen(buffer));
 s_writeline(connection, "Server: HTTPAPI v0.1\r\n", 22);
 s_writeline(connection, "Content-Type: application/json\r\n", 32);
 s_writeline(connection, "\r\n", 2);
 
 log->debug("http", "HTTP headers forged and sent.");
 
 return true;
}

bool RequestHandler::parseHTTPHeader(char *buffer, Request *request) {
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
  
  request->resource = (char *)calloc(len + 1, sizeof(char));
  strncpy(request->resource, buffer, len);
  
  if (strstr(buffer, "HTTP/")) request->level = FULL;
  else request->level = SIMPLE;
  
  first_line = 0;
  return true;
 }
 
 // More headers aside from the request line
 endptr = strchr(buffer, 58); // 58 is a : character
 if (endptr == NULL) {
  request->status = 400;
  return false;
 }
 
 temp = (char *)calloc((endptr - buffer) + 1, sizeof(char));
 strncpy(temp, buffer, (endptr - buffer));
 stoupper(temp);
 
 buffer = endptr + 1;
 while (*buffer && isspace(*buffer)) ++buffer;
 if (*buffer == '\0') return true;
 
 if (!strcmp(temp, "USER-AGENT")) {
  request->useragent = (char *)malloc(strlen(buffer) + 1);
  strcpy(request->useragent, buffer);
 } else if (!strcmp(temp, "REFERER")) {
  request->referer = (char *)malloc(strlen(buffer) + 1);
  strcpy(request->referer, buffer);
 }
 
 free(temp);
 return true;
}
