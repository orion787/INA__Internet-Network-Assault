#ifndef ASYNCCLIENT_H
#define ASYNCCLIENT_H

#include <string>
#include <cstdint>

class AsyncClient {
private:
    std::string targetHost; // Объявление переменной
    uint16_t targetPort;    // Объявление переменной
    int sockfd;             // Дескриптор сокета

public:
    AsyncClient(const std::string& host, uint16_t port);
    bool connectToServer();
    void sendData(const std::string& data);
    ~AsyncClient();
};

#endif // ASYNCCLIENT_H
