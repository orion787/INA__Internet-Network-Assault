#ifndef TCPSYNFLOOD_H
#define TCPSYNFLOOD_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdint>

class TcpSynFlood {
private:
    std::string targetHost;
    uint16_t targetPort;
    size_t threadCount;
    size_t packetCount;
    std::mutex coutMutex; // Для безопасного вывода в консоль из разных потоков.

    void performFlood(); // Основная функция атаки.
    uint16_t checksum(void* data, int len); // Прототип функции checksum

public:
    TcpSynFlood(const std::string& host, uint16_t port, size_t threads, size_t packets);
    void start(); // Запуск атаки.
};

#endif // TCPSYNFLOOD_H

