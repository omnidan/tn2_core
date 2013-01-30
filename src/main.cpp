/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main entry for the http request api
 *
 *        Version:  0.2
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
#include <stdio.h>

// Internal headers
#include "config.h"
#include "socket.h"

/* main: Main function */
int main(int argc, char *argv []) {
 // Print info message
 printf("[INFO ] [main] %s booting...\n", VERSION);
 printf("[INFO ] [main]  Copyright (c) 2013 Daniel Bugl <daniel.bugl@touchlay.com>\n");
 printf("[INFO ] [main]  Copyright (c) 2013 TouchLay <support@touchlay.com>\n");
 
 Socket *socket = new Socket(PORT); // Initialise socket
 return socket->loop(); // Exit with the return value from the loop
}
