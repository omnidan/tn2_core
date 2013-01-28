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
 InitReqInfo(&request); // TODO
 
 // Handle request and check request type
 if (handle() == true) {
  if (request->type == REQ_TYPE_HTTP) {
   if (request->status == 200) {
    Parse_JSON(&request); // TODO
    Output_HTTP_Headers(connection, &request); // TODO, response
    Return_Resource(connection, resource, &request); // TODO, response
   } else Return_Error_Msg(conn, &request); // TODO
  } else if (request->type == REQ_TYPE_TN) {
   Parse_JSON(request); // TODO
  } else log->warning("", "Unknown request type, killing request.");
 } else log->warning("", "Couldn't handle request, killing it.");
 
 // Free request data stored in ram
 FreeReqInfo(&request);
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
   Readline(conn, buffer, MAX_REQ_LINE - 1); // TODO
   Trim(buffer); // TODO
   
   if (buffer[0] == '{') {
    // TODO: This is a TN request, parse it as such
   } else {
    // TODO: This is an HTTP request, parse it as such
   }
   
   // HTTP stuff
   if (buffer[0] == '\0') break; // End of HTTP headers
   if (Parse_HTTP_Header(buffer, request)) break; // Parse headers, TODO
  }
 } while (request->done != true);
 return true; // Successfully processed
}
