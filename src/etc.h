/*
 * =====================================================================================
 *
 *       Filename:  etc.h
 *
 *    Description:  Additional functions - headers
 *
 *        Version:  0.2
 *        Created:  28/01/13 22:28:24
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef ETC_H
#define ETC_H

#include "config.h"

#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Strings
std::string stoupper(std::string);
int stoupper(char *);
int strim(char *);

// Sockets
ssize_t s_readline(int, void *, size_t);
ssize_t s_writeline(int, void const *, size_t);

#endif /* ETC_H */
