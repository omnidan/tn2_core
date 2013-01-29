/*
 * =====================================================================================
 *
 *       Filename:  etc.h
 *
 *    Description:  Additional functions - headers
 *
 *        Version:  1.0
 *        Created:  28/01/13 22:28:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#include <iostream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

// For s_getip
#include <arpa/inet.h>

// Strings
std::string stoupper(std::string);
int stoupper(char *);
int sttrim(char *);
void urlize(char *);

// Sockets
ssize_t s_readline(int, void *, size_t);
ssize_t s_writeline(int, void const *, size_t);
void *s_getip(const struct sockaddr *);
