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
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <ctime>
#include "etc.h"
#include "config.h"

#define TESTMSG "{}" // TODO: This could be improved later, when the JSON parser is implemented

typedef unsigned long timestamp_t;

static timestamp_t timestamp() {
 struct timeval now;
 gettimeofday (&now, NULL);
 return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void test1(int connection) {
 s_writeline(connection, "HTTP/1.1 200 OK\r\n", 17);
 s_writeline(connection, "Content-Type: application/json\r\n", 32);
 s_writeline(connection, "\r\n", 2);
 s_writeline(connection, TESTMSG, 2);
}

void test2(int connection) {
 s_writeline(connection, TESTMSG, 2);
}

int main(int argc, char *argv []) {
 int connection = socket(AF_INET, SOCK_STREAM, 0);
 sockaddr_in servAddr;
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = PORT;
 //if (argc < 2) {
 // printf("[~] No IP address as argument provided, assuming 127.0.0.1");
 // servAddr.sin_addr.s_addr = inet_addr(argv[1]);
 //} else 
 servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
 connect(connection, (struct sockaddr*)&servAddr, sizeof(struct sockaddr));
 
 printf("[~] Connected... Starting benchmarks.");
 
 timestamp_t benchmark0, benchmark1;
 double secs;
 
 benchmark0 = timestamp();
 test1(connection);
 benchmark1 = timestamp();
 secs = (benchmark1 - benchmark0) / 1000000.0L;
 printf("[+] Test 1 (HTTP) took %0.5f seconds.", secs);
 
 benchmark0 = timestamp();
 test2(connection);
 benchmark1 = timestamp();
 secs = (benchmark1 - benchmark0) / 1000000.0L;
 printf("[+] Test 2 (TN)   took %0.5f seconds.", secs);
 
 return 0;
}

