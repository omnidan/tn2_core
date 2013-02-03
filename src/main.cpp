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

// Internal headers
#include "config.h"
#include "socket.h"

/* main: Main function */
int main(int argc, char *argv []) {
 // TODO: Parse arguments
 
 // Print info messages
 std::cout << "[INFO ] [main] " << NAME << " v" << VERSION;
 #ifdef VERSION_TAG
 std::cout << " (" << VERSION_TAG << ")";
 #endif
 std::cout << " booting..." << std::endl;
 std::cout << "[INFO ] [main]  Copyright (c) 2013 Daniel Bugl <daniel.bugl@touchlay.com>" << std::endl;
 std::cout << "[INFO ] [main]  Copyright (c) 2013 TouchLay <support@touchlay.com>" << std::endl;
 
 Socket *socket = new Socket(PORT); // Initialise socket
 return socket->loop(); // Exit with the return value from the loop
}
