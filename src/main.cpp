/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Main entry for the http request api
 *
 *        Version:  0.1
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
#include <stdlib.h>
#include <string.h>

// Network headers
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

// Internal headers
#include "config.h"
#include "socket.h"

/* main: Main function */
int main(int argc, char *argv []) {
 int retval; // Needed later
 
 // Print info message
 printf("[INFO ] [main] %s booting...\n", VERSION);
 printf("[INFO ] [main]  Copyright (c) 2013 Daniel Bugl <daniel.bugl@touchlay.com>\n");
 printf("[INFO ] [main]  Copyright (c) 2013 TouchLay <support@touchlay.com>\n");
 
 // Initialise socket
 Socket *socket = new Socket(PORT);
 retval = socket->loop();
 
 // The loop stopped, exit with its return value
 return retval;
}
