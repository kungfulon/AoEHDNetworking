#pragma once

#include <fstream>

class CLog {
public:
    CLog();
    void Debug(const char* fmt, ...);

private:
    std::ofstream f;
};

CLog* Log();
