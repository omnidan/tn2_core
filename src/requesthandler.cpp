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
RequestHandler::RequestHandler(int connection, char *cip) {
 clientip = cip;
 
 // Initialise request
 InitRequest(&request);
 
 // Handle request and check request type
 if (handle(connection) == true) {
  #ifdef DEBUG
  printf("[DEBUG] [request] handle() returned true, continuing!\n");
  #endif
  if (request.type == HTTP) {
   #ifdef DEBUG
   printf("[DEBUG] [request_http] This is an HTTP request.\n");
   #endif
   if (request.status == 200) {
    #ifdef DEBUG
    printf("[DEBUG] [request_http] Status 200, returning result.\n");
    #endif
    //Parse_JSON(&request); // TODO
    outputHTTPHeader(connection, &request);
    s_writeline(connection, "{}", 2);
    #ifdef DEBUG
    printf("[DEBUG] [request_http] Answered to request with HTTP.\n");
    #endif
    //Return_Resource(connection, resource, &request); // TODO, response
   } //else Return_Error_Msg(conn, &request); // TODO
  } else if (request.type == TN) {
   #ifdef DEBUG
   printf("[DEBUG] [request_tn] This is a TN request.\n");
   #endif
   //Parse_JSON(request); // TODO
   s_writeline(connection, "{}", 2);
   #ifdef DEBUG
   printf("[DEBUG] [request_tn] Answered to request with TN.\n");
   #endif
  } else printf("[WARN] [request] Unknown request type, killing request.\n");
 } else printf("[WARN] [request] Couldn't handle request, killing it.\n");
 
 // Free request data stored in ram
 FreeRequest(&request);
}

/* InitRequest: Initialises the request data */
void RequestHandler::InitRequest(Request *request) {
 request->status = 200;
 request->method = UNSUPPORTED;
 request->resource = NULL;
 #ifdef DEBUG
 printf("[DEBUG] [request] Initialised request.\n");
 #endif
}

/* FreeRequest: Clear the request data */
void RequestHandler::FreeRequest(Request *request) {
 if (request->resource) free(request->resource);
 #ifdef DEBUG
 printf("[DEBUG] [request] Free'd request.\n");
 #endif
}

/* handle: Handle a request */
bool RequestHandler::handle(int connection) {
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
  
  if (rval < 0) printf("[WARN ] [request_handle] Couldn't select from request.\n");
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
 
 #ifdef DEBUG
 printf("[DEBUG] [request_http] HTTP headers forged and sent.\n");
 #endif
 
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
 
 free(temp);
 return true;
}
