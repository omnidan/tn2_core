/*
 * =====================================================================================
 *
 *       Filename:  etc.cpp
 *
 *    Description:  Additional functions
 *
 *        Version:  1.0
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
   else { std::cout << "[WARN ] [s_readline] Socket error " << errno << std::endl; return 0; }
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
   else { std::cout << "[WARN ] [s_writeline] Socket error " << errno << std::endl; return 0; }
  }
  nleft -= nwritten;
  buffer += nwritten;
 }
 
 return n;
}

// Decode HTTP encodings
std::string decodeURI(const std::string & sSrc) {
 const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
 const int SRC_LEN = sSrc.length();
 const unsigned char * const SRC_END = pSrc + SRC_LEN;
 const unsigned char * const SRC_LAST_DEC = SRC_END - 2; // last decodable '%' 
 
 char * const pStart = new char[SRC_LEN];
 char * pEnd = pStart;
 
 while (pSrc < SRC_LAST_DEC) {
  if (*pSrc == '%') {
   char dec1, dec2;
   if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)]) && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
    *pEnd++ = (dec1 << 4) + dec2;
    pSrc += 3;
    continue;
   }
  }
  *pEnd++ = *pSrc++;
 }
 
 while (pSrc < SRC_END) *pEnd++ = *pSrc++; // the last 2- chars
 
 std::string sResult(pStart, pEnd);
 delete [] pStart;
 
 return sResult;
}
