#ifndef HTTPFLOOD_H
#define HTTPFLOOD_H

#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>

class HttpFlood {
private:
    std::string targetHost;
    uint16_t targetPort;
    size_t threadCount;
    size_t requestCount;
    std::mutex coutMutex; // Для безопасного вывода в консоль из разных потоков.

    void performFlood(); // Основная функция атаки.

public:
    HttpFlood(const std::string& host, uint16_t port, size_t threads, size_t requests);
    void start(); // Запуск атаки.
};

#endif // HTTPFLOOD_H
