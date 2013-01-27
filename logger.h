/**
 * This is the header file for the log system.
 * 
 * This file is part of LOSK, the LayOS Kernel.
 * 
 * @author Daniel Bugl <Daniel@touchlay.com>
 * @copyright Copyright (c) 2012 by TouchLay (http://touchlay.com)
 */

#ifndef LOGSYSTEM_H
#define	LOGSYSTEM_H

#include <vector>
#include <string>
#include <time.h>

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
    std::vector<LogMsg> buffer;
    std::vector<LogMsg>::iterator it;
    void addToBuffer(LogMsg); // This adds a message to the buffer
    void updateFile(); // This updates the log file
    void printLog(LogMsg);
public:
    Logger();
    std::string print(LogMsg);
    void log(std::string, std::string, std::string);
    void error(std::string, std::string);
    void warning(std::string, std::string);
    void info(std::string, std::string);
    void debug(std::string, std::string);
    std::vector<LogMsg> getBuffer();
};

#endif	/* LOGSYSTEM_H */

