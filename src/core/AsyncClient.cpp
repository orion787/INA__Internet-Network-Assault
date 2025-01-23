#include <AsyncClient.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

AsyncClient::AsyncClient(const std::string& host, uint16_t port)
    : targetHost(host), targetPort(port) {}

bool AsyncClient::connectToServer() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(targetPort);
    if (inet_pton(AF_INET, targetHost.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid address");
        return false;
    }

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        return false;
    }

    return true;
}

void AsyncClient::sendData(const std::string& data) {
    if (send(sockfd, data.c_str(), data.size(), 0) < 0) {
        perror("Send failed");
    }
}

AsyncClient::~AsyncClient() {
    close(sockfd);
}
