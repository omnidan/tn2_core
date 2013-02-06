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
 *       Compiler:  g++
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
 // TODO: Create a blacklist for IPs
 clientip = cip;
 
 // Initialise request
 InitRequest(&request);
 
 // Handle request and check request type
 if (handle(connection) == true) {
  #ifdef DEBUG
  std::cout << "[DEBUG] [request     ] handle() returned true, continuing!" << std::endl;
  #endif
  if (request.type == HTTP) {
   #ifdef DEBUG
   std::cout << "[DEBUG] [request_http] This is an HTTP request." << std::endl;
   #endif
   if (request.status == 200) {
    #ifdef DEBUG
    std::cout << "[DEBUG] [request_http] Status 200, returning result." << std::endl;
    #endif
    if (parseJSON()) {
     API api = API(jRoot);
     #ifdef DEBUG
     std::cout << "[DEBUG] [request_api ] Got API result(" << strlen(api.result.c_str()) << "): " << api.result << std::endl;
     #endif
     outputHTTP(connection, &request, api.result.c_str());
    } else outputHTTP(connection, &request, "{\"type\": \"error\", \"msg\": \"Invalid JSON.\"}");
    #ifdef DEBUG
    std::cout << "[DEBUG] [request_http] Answered to request with HTTP." << std::endl;
    #endif
   } else {
    std::stringstream sbuffer;
    sbuffer << "{\"type\": \"error\", \"msg\": \"HTTP Error " << request.status << "\"}";
    outputHTTP(connection, &request, sbuffer.str());
   }
   usleep(REQUEST_TIMEOUT_SEND*1000000);
  } else if (request.type == TN) {
   #ifdef DEBUG
   std::cout << "[DEBUG] [request_tn  ] This is a TN request." << std::endl;
   #endif
   if (parseJSON()) {
    API api = API(jRoot);
    #ifdef DEBUG
    std::cout << "[DEBUG] [request_api ] Got API result(" << strlen(api.result.c_str()) << "): " << api.result << std::endl; 
    #endif
    s_writeline(connection, api.result.c_str(), strlen(api.result.c_str()));
   } else s_writeline(connection, "{\"type\": \"error\", \"msg\": \"Invalid JSON.\"}", 41);
   #ifdef DEBUG
   std::cout << "[DEBUG] [request_tn  ] Answered to request with TN." << std::endl;
   usleep(REQUEST_TIMEOUT_SEND*1000000);
   #endif
  } else std::cout << "[WARN ] [request     ] Unknown request type, killing request." << std::endl;
 } else std::cout << "[WARN ] [request     ] Couldn't handle request, killing it." << std::endl;
}

/* parseJSON: JSON parser */
bool RequestHandler::parseJSON() {
 std::string data("");
 if (request.resource != 0) data = request.resource;
 if ((request.type == HTTP) && (data != "")) data.erase(data.begin()); // Remove / prefix from the GET request
 data = decodeURI(data);
 #ifdef DEBUG
 std::cout << "[DEBUG] [request_json] Got data: " << data << "\n";
 #endif
 if (!jReader.parse(data, jRoot, false)) {
  #ifdef DEBUG
  std::cout << "[DEBUG] [request_json] Invalid JSON (" << data << ")." << std::endl;
  #endif
  return false;
 }
 else return true;
}

/* ~RequestHandler: Destructor */
RequestHandler::~RequestHandler() {
 #ifdef DEBUG
 std::cout << "[DEBUG] [request     ] Destructed RequestHandler." << std::endl;
 #endif
}

/* InitRequest: Initialises the request data */
void RequestHandler::InitRequest(Request *request) {
 request->status = 200;
 request->method = UNSUPPORTED;
 request->resource = NULL;
 #ifdef DEBUG
 std::cout << "[DEBUG] [request     ] Initialised request." << std::endl;
 #endif
}

/* FreeRequest: Clear the request data */
void RequestHandler::FreeRequest(Request *request) {
 if (request->resource) delete request->resource;
 #ifdef DEBUG
 std::cout << "[DEBUG] [request     ] Free'd request." << std::endl;
 #endif
}

/* handle: Handle a request */
bool RequestHandler::handle(int connection) {
 char buffer[MAX_REQ_LINE] = {0};
 int rval;
 fd_set fds;
 struct timeval tv;
 
 // Timeout
 tv.tv_sec = REQUEST_TIMEOUT_RECV;
 tv.tv_usec = 0;
 
 do {
  // Reset FD
  FD_ZERO(&fds);
  FD_SET(connection, &fds);
  
  rval = select(connection+1, &fds, NULL, NULL, &tv); // Select from request
  
  if (rval < 0) std::cout << "[WARN ] [request     ] Couldn't select from request." << std::endl;
  else if (rval == 0) return false; // Timeout, kill request
  else {
   s_readline(connection, buffer, MAX_REQ_LINE - 1);
   
   #ifdef DEBUG
   std::cout << "[DEBUG] [request     ] Received buffer: " << buffer << std::endl;
   #endif
   
   if (buffer[0] == '{') {
    request.type = TN;
    request.resource = buffer;
    request.level = SIMPLE;
    return true;
   } else {
    request.type = HTTP;
    strim(buffer);
    char *buf = buffer;
    char *endptr = NULL;
    int len;
    if (!strncmp(buffer, "GET ", 4)) {
     request.method = GET;
     buf += 4;
    } else if (!strncmp(buffer, "HEAD ", 5)) {
     request.method = HEAD;
     buf += 5;
    } else {
     request.method = UNSUPPORTED;
     request.status = 501;
     return true;
    }
    
    while (*buf && isspace(*buf)) buf++; // Skip the start
    
    endptr = strchr(buf, ' ');
    if (endptr == NULL) len = strlen(buf);
    else len = endptr - buf;
    
    if (len == 0) {
     request.status = 400;
     return true;
    }
    
    request.resource = (char *)calloc(len + 1, sizeof(char));
    strncpy(request.resource, buf, len);
    
    request.level = SIMPLE;
    
    return true;
   }
  }
 } while (request.level != SIMPLE);
 return true; // Successfully processed
}

bool RequestHandler::outputHTTP(int connection, Request *request, std::string content) {
 std::stringstream sbuffer;
 
 if (request->status == 200) sbuffer << "HTTP/1.1 " << request->status << " OK\r\n";
 else if (request->status == 400) sbuffer << "HTTP/1.1 " << request->status << " Bad Request\r\n";
 else sbuffer << "HTTP/1.1 501 Not Implemented\r\n";
 s_writeline(connection, sbuffer.str().c_str(), strlen(sbuffer.str().c_str()));
 sbuffer.str(std::string());
 sbuffer << "Server: " << NAME << "/" << VERSION << "\r\n";
 s_writeline(connection, sbuffer.str().c_str(), strlen(sbuffer.str().c_str()));
 sbuffer.str(std::string());
 s_writeline(connection, "Content-Type: application/json\r\n", 32);
 sbuffer << "Content-Length: " << strlen(content.c_str()) << "\r\n";
 s_writeline(connection, sbuffer.str().c_str(), strlen(sbuffer.str().c_str()));
 sbuffer.str(std::string());
 s_writeline(connection, "\r\n", 2);
 s_writeline(connection, content.c_str(), strlen(content.c_str()));
 
 return true;
}
