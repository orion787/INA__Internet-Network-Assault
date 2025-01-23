#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <string>

class ReportGenerator {
public:
    static void generateReport(const std::string& filePath, const std::string& content);
};

#endif // REPORT_GENERATOR_H
