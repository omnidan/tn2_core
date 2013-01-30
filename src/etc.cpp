/*
 * =====================================================================================
 *
 *       Filename:  etc.cpp
 *
 *    Description:  Additional functions
 *
 *        Version:  0.2
 *        Created:  28/01/13 22:18:03
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#include "etc.h"

// String to upper function
std::string stoupper(std::string s) {
    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        std::ptr_fun <int, int> ( std::toupper )
    );
    return s;
}

// Overloaded function for cstrings
int stoupper(char *buffer) {
 while (*buffer) {
  *buffer = toupper(*buffer);
  ++buffer;
 }
 return 0;
}

// Remove trailing whitespace from a string
int strim(char *buffer) {
 int n = strlen(buffer)-1;
 while (!isalnum(buffer[n]) && (n >= 0)) buffer[n--] = '\0';
 return 0;
}

// Read line from socket
ssize_t s_readline(int sockd, void *vptr, size_t maxlen) {
 ssize_t n, rc;
 char c, *buffer;

 buffer = (char *)vptr;
 
 for (n=1; (unsigned)n<maxlen; n++) {
  if ((rc=read(sockd, &c, 1)) == 1) {
   *buffer++ = c;
   if (c == '\n') break;
  } else if (rc == 0) {
   if (n == 1) return 0;
   else break;
  } else {
   if (errno == EINTR) continue;
   return 0; // Some error happened, TODO: Handle this error
  }
 }
 
 *buffer = 0;
 return n;
}

// Write line to socket
ssize_t s_writeline(int sockd, void const *vptr, size_t n) {
 size_t nleft;
 ssize_t nwritten;
 const char *buffer;
 
 buffer = (const char *)vptr;
 nleft = n;
 
 while (nleft > 0) {
  if ((nwritten = write(sockd, buffer, nleft)) <= 0) {
   if (errno == EINTR) nwritten = 0;
   else return 0; // Some error happened, TODO: Handle this error
  }
  nleft -= nwritten;
  buffer += nwritten;
 }
 
 return n;
}
