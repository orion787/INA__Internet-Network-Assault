#ifndef UDPFLOOD_H
#define UDPFLOOD_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdint>

class UdpFlood {
private:
    std::string targetHost;
    uint16_t targetPort;
    size_t threadCount;
    size_t packetCount;
    std::mutex coutMutex; // Для безопасного вывода в консоль из разных потоков.

    void performFlood(); // Основная функция атаки.

public:
    UdpFlood(const std::string& host, uint16_t port, size_t threads, size_t packets);
    void start(); // Запуск атаки.
};

#endif // UDPFLOOD_H
