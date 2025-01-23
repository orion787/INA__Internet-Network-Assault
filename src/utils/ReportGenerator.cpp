#include "ReportGenerator.h"
#include <fstream>

void ReportGenerator::generateReport(const std::string& filepath, const std::string& content) {
    std::ofstream reportFile(filepath);
    if (!reportFile.is_open()) {
        throw std::runtime_error("Failed to create report file");
    }

    reportFile << content;
    reportFile.close();
}
