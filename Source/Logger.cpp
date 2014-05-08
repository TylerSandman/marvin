#include <cstdlib>
#include <fstream>
#include "Logger.h"

void Logger::log(const std::string &string){
    mLogFile << string.c_str();
}

std::ofstream Logger::mLogFile("log.txt");