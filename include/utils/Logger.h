#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    static void log(const std::string& message);

private:
    static std::ofstream logFile;
    static std::mutex logMutex;
    static void init();
};

#endif // LOGGER_H
