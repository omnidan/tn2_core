/*
 * =====================================================================================
 *
 *       Filename:  main_client.cpp
 *
 *    Description:  Main file for the client
 *
 *        Version:  1.0
 *        Created:  29/01/13 20:47:42
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

// Includes
#include <iostream>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <ctime>
#include "etc.h"
#include "config.h"

typedef unsigned long timestamp_t;

static timestamp_t timestamp() {
 struct timeval now;
 gettimeofday (&now, NULL);
 return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void test1(int connection) {
 s_writeline(connection, "GET /%7B%7D HTTP/1.1\r\n", 22); // %7B%7D is the URI encoded version of {}
 s_writeline(connection, "User-Agent: tn2_benchmark/1.0\r\n", 24);
 s_writeline(connection, "Host: 127.0.0.1\r\n", 17);
 s_writeline(connection, "Accept: */*", 11);
 s_writeline(connection, "\r\n", 2);
}

void test2(int connection) {
 s_writeline(connection, "{}\r\n", 4);
}

int establishConnection(char *serverip, int port) {
 int connection = socket(AF_INET, SOCK_STREAM, 0);
 struct sockaddr_in servAddr;
 struct hostent *server;
 
 server = gethostbyname(serverip);
 
 bzero((char *)&servAddr, sizeof(servAddr));
 servAddr.sin_family = AF_INET;
 bcopy((char *)server->h_addr, (char *)&servAddr.sin_addr.s_addr, server->h_length);
 servAddr.sin_port = htons(PORT);
 
 if (connect(connection, (struct sockaddr*)&servAddr, sizeof(struct sockaddr)) < 0) {
  std::cout << "[-] Connection to " << serverip << " failed." << std::endl;
  return -1;
 }
 return connection;
}

int main(int argc, char *argv []) {
 char *serverip;
 int connection;
 if (argc < 2) {
  std::cout << "[~] No IP address as argument provided, assuming 127.0.0.1" << std::endl;
  serverip = (char *)"127.0.0.1";
 } else serverip = (char *)argv[1];
 
 if ((connection=establishConnection(serverip, PORT)) < 0) return EXIT_FAILURE;
 
 std::cout << "[~] Connected to " << serverip << "... Starting benchmarks." << std::endl;
 
 timestamp_t benchmark0, benchmark1, benchmark2;
 
 fd_set fds;
 struct timeval tv;
 tv.tv_sec = 3; // 3 second timeout
 tv.tv_usec = 0;
 int rval;
 
 benchmark0 = timestamp();
 test1(connection);
 benchmark1 = timestamp();
 FD_ZERO(&fds);
 FD_SET(connection, &fds);
 rval = select(connection+1, &fds, NULL, NULL, &tv);
 if (rval < 0) std::cout << "[-] Test 1 (HTTP) failed." << std::endl;
 else {
  benchmark2 = timestamp();
  if ((((benchmark2-benchmark1)/1000.0L)+1) > 3000) std::cout << "[-] Test 1 (HTTP) : Timeout." << std::endl;
  else std::cout << "[+] Test 1 (HTTP) : Send: " << (double)((benchmark1-benchmark0)/1000.0L) << " milliseconds, Recv: " << (double)((benchmark2-benchmark1)/1000.0L) << " milliseconds." << std::endl;
 }
 close(connection);
 
 usleep(REQUEST_TIMEOUT_SEND*1000000);
 
 if ((connection=establishConnection(serverip, PORT)) < 0) return EXIT_FAILURE;
 
 benchmark0 = timestamp();
 test2(connection);
 benchmark1 = timestamp();
 FD_ZERO(&fds);
 FD_SET(connection, &fds);
 rval = select(connection+1, &fds, NULL, NULL, &tv);
 if (rval < 0) std::cout << "[-] Test 2 (TN)   failed." << std::endl;
 else {
  benchmark2 = timestamp();
  if ((((benchmark2-benchmark1)/1000.0L)+1) > 3000) std::cout << "[-] Test 2 (TN)   : Timeout." << std::endl;
  else std::cout << "[+] Test 2 (TN)   : Send: " << (double)((benchmark1-benchmark0)/1000.0L) << " milliseconds, Recv: " << (double)((benchmark2-benchmark1)/1000.0L) << " milliseconds." << std::endl;
 }
 close(connection);
 return 0;
}

