/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main entry for the http request api
 *
 *        Version:  1.0
 *        Created:  27/01/13 18:18:34
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay 
 *
 * =====================================================================================
 */

// Standard headers
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Internal headers
#include "config.h"
#include "socket.h"

/* versioninfo: Show information about this build */
void versioninfo(bool booting) {
 std::cout << "[INFO ] [main] " << NAME << " v" << VERSION;
 #ifdef VERSION_TAG
 std::cout << " (" << VERSION_TAG << ")";
 #endif
 if (booting) std::cout << " booting..." << std::endl;
 else std::cout << " built at " << __DATE__ << ", " << __TIME__ << "." << std::endl;
 std::cout << "[INFO ] [main]  Copyright (c) 2013 Daniel Bugl <daniel.bugl@touchlay.com>" << std::endl;
 std::cout << "[INFO ] [main]  Copyright (c) 2013 TouchLay <support@touchlay.com>" << std::endl;
 if (!booting) {
  std::cout << "[INFO ] [main] Compilation configuration:" << std::endl;
  std::cout << "[INFO ] [main]  PORT=" << PORT << std::endl;
  std::cout << "[INFO ] [main]  LISTENQ=" << LISTENQ << std::endl;
  std::cout << "[INFO ] [main]  MAX_REQ_LINE=" << MAX_REQ_LINE << std::endl;
  std::cout << "[INFO ] [main]  REQUEST_TIMEOUT_RECV=" << REQUEST_TIMEOUT_RECV << std::endl;
  std::cout << "[INFO ] [main]  REQUEST_TIMEOUT_SEND=" << REQUEST_TIMEOUT_SEND << std::endl;
  #ifdef DEBUG
  std::cout << "[INFO ] [main]  DEBUG=true" << std::endl;
  #else
  std::cout << "[INFO ] [main]  DEBUG=false" << std::endl;
  #endif
 }
}

/* main: Main function */
int main(int argc, char *argv []) {
 int port = PORT;
 
 char c;
 opterr = 0;
 while ((c=getopt(argc, argv, "vp:")) != -1) {
  switch (c) {
   case 'v':
    versioninfo(false);
    return EXIT_SUCCESS;
    break;
   case 'p':
    port = atoi(optarg);
    if (port <= 0) port = PORT;
    break;
   case '?':
    if ((char)optopt == 'p') std::cout << "[ERROR] [main] Option -" << (char)optopt << " requires an argument." << std::endl;
    else if (isprint((char)optopt)) std::cout << "[ERROR] [main] Unknown option: -" << (char)optopt << "." << std::endl;
    else std::cout << "[ERROR] [main] Unknown option character: " << (char)optopt << "." << std::endl;
    return EXIT_FAILURE;
   default:
    abort();
  }
 }
 
 // Print info messages
 versioninfo(true);
 
 Socket *socket = new Socket(port); // Initialise socket
 return socket->loop(); // Exit with the return value from the loop
}
