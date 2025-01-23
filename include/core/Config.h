#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <unordered_map>

class Config {
public:
    explicit Config(const std::string& filePath);
    std::string getValue(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> configData;

    void loadConfig(const std::string& filePath);
};

#endif // CONFIG_H
