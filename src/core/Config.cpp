#include <Config.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm> // Для trim
#include <cctype>

// Функция для обрезки пробелов (trim)
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return ""; // Пустая строка
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            key = trim(key); // Удаляем пробелы вокруг ключа
            value = trim(value); // Удаляем пробелы вокруг значения
            configData[key] = value;
        }
    }
}

std::string Config::getValue(const std::string& key) const {
    auto it = configData.find(key);
    if (it != configData.end()) {
        return it->second;
    }
    throw std::runtime_error("Key not found: " + key);
}
