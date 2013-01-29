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
 *       Compiler:  gcc
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
#include "logger.h"
#include "socket.h"

/* main: Main function */
int main(int argc, char *argv []) {
 int retval; // Needed later
 
 // Initialise logging system
 Logger *log = new Logger("main");
 log->info("", "HTTPAPI v0.1 booting...");
 
 // Initialise socket
 Socket *socket = new Socket(PORT); // TODO: Add requesthandler as an argument here
 retval = socket->loop();
 
 // The loop stopped, exit with its return value
 return retval;
}
