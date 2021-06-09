#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger() {
        this->source = "";
    }

    Logger(std::string source) {
        this->source = source;
    }
    
    bool printDate = false;
    std::string source;

    void info(std::string msg) {
        std::cout << makeLine("INFO", msg);
    }

    void warn(std::string msg) {
        std::cout << makeLine("WARN", msg);
    }

    void error(std::string msg) {
        std::cout << makeLine("ERROR", msg);
    }

private:
    std::string makeLine(std::string prefix, std::string msg) {
        std::string line = prefix + ": " + msg + "\n";
        if (source.length() > 0) {
            line = source + ": " + line;
        }
        if (printDate) {
            time_t time = std::time(NULL);
            line = std::ctime(&time) + line;
        } 
        return line;
    }
};

#endif
