/*
 * =====================================================================================
 *
 *       Filename:  Logger.cpp
 *
 *    Description:  Logger class
 *
 *        Version:  1.0
 *        Created:  27/01/13 18:20:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Bugl <Daniel.Bugl@touchlay.com>
 *   Organization:  TouchLay
 *
 * =====================================================================================
 */

#include "logger.h"

LogTime::LogTime() {
    t = time(0);
    now = localtime( & t );
}

int LogTime::asSecond() {
    return now->tm_sec;
}

int LogTime::asMinute() {
    return now->tm_min;
}

int LogTime::asHour() {
    return now->tm_hour;
}

int LogTime::asDay() {
    return now->tm_mday;
}

int LogTime::asMonth() {
    return (now->tm_mon + 1);
}

int LogTime::asYear() {
    return (now->tm_year + 1900);
}

std::string LogTime::asString() {
    std::stringstream sstr;
    char buffer[8];
    sprintf(buffer, "%02d", asDay());
    sstr << buffer;
    sstr << ".";
    sprintf(buffer, "%02d", asMonth());
    sstr << buffer;
    sstr << ".";
    sstr << asYear();
    sstr << "_";
    sprintf(buffer, "%02d", asHour());
    sstr << buffer;
    sstr << ":";
    sprintf(buffer, "%02d", asMinute());
    sstr << buffer;
    sstr << ":";
    sprintf(buffer, "%02d", asSecond());
    sstr << buffer;
    return sstr.str();
}

char * LogTime::asCString() {
    return (char *)asString().c_str();
}

// Logger
Logger::Logger(std::string tag) {
 stag = tag;
 #ifdef DEBUG
 debug("logger", "Logger initialised!");
 #endif
}

void Logger::log(std::string type, std::string title, std::string message) {
    LogMsg lm;
    LogTime lt;
    lm.time = lt;
    lm.type = type;
    std::stringstream ss;
    if (title == "") ss << stag;
    else {
     ss << stag;
     ss << "_";
     ss << title;
    }
    lm.title = ss.str();
    lm.message = message;
    addToBuffer(lm);
}

void Logger::error(std::string title, std::string message) {
 log("ERROR", title, message);
 exit(EXIT_FAILURE);
}

void Logger::warning(std::string title, std::string message) {
 log("WARN ", title, message);
}

void Logger::info(std::string title, std::string message) {
 log("INFO ", title, message);
}

void Logger::debug(std::string title, std::string message) {
 #ifdef DEBUG
  log("DEBUG", title, message);
 #endif
}

void Logger::addToBuffer(LogMsg message) {
    it = buffer.end();
    it = buffer.insert(it, message);
    printLog(message);
}

std::string Logger::print(LogMsg msg) {
    std::stringstream sstr;
    sstr << "[";
    sstr << msg.time.asCString();
    sstr << "] [";
    sstr << stoupper(msg.type);
    sstr << "] [";
    sstr << msg.title;
    sstr << "] ";
    sstr << msg.message;
    return sstr.str();
}

void Logger::printLog(LogMsg msg) {
    std::string sBuffer = print(msg);
    printf("%s\n", sBuffer.c_str());
    // TODO: print to file too.
}
