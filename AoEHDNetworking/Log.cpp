#include "Log.h"

#include <cstdarg>

CLog* Log() {
    static CLog* instance = nullptr;
    if (instance == nullptr) {
        instance = new CLog();
    }
    return instance;
}

CLog::CLog() : f("log.txt") {

}

void CLog::Debug(const char* fmt, ...) {
    static char buffer[4096];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    f << "[DEBUG] " << buffer << std::endl;
}
