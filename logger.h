/*
 * =====================================================================================
 *
 *       Filename:  logger.h
 *
 *    Description:  Logger class headers
 *
 *        Version:  1.0
 *        Created:  27/01/13 18:21:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <vector>
#include <string>
#include <time.h>
#include "etc.h"
#include "config.h"
#include <cstdio>
#include <sstream>

class LogTime {
private:
    time_t t;
    struct tm * now;
public:
    LogTime();
    std::string asString();
    char * asCString();
    int asSecond();
    int asMinute();
    int asHour();
    int asDay();
    int asMonth();
    int asYear();
};

struct LogMsg {
    LogTime time;
    std::string type;
    std::string title;
    std::string message;
};

class Logger {
private:
    std::string stag;
    std::vector<LogMsg> buffer;
    std::vector<LogMsg>::iterator it;
    void addToBuffer(LogMsg); // This adds a message to the buffer
    void updateFile(); // This updates the log file
    void printLog(LogMsg);
public:
    Logger(std::string);
    std::string print(LogMsg);
    void log(std::string, std::string, std::string);
    void error(std::string, std::string);
    void warning(std::string, std::string);
    void info(std::string, std::string);
    void debug(std::string, std::string);
    std::vector<LogMsg> getBuffer();
};

#endif	/* LOGGER_H */
