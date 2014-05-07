#pragma once
#include <fstream>

class Logger{

public:
    static void log(const std::string &string);

private:
    static std::ofstream mLogFile;
};