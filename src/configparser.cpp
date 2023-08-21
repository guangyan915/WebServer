#include "../include/configparser.h"
#include <fstream>

ConfigParser* ConfigParser::Instance() {
  static ConfigParser instance(DEFAULT_CONFIG_FILE_PATH);
  return &instance;
}

ConfigParser::ConfigParser(const std::string& configFile) : _config_file_path(configFile) {}

void ConfigParser::SetConfigFilePath(const std::string& path) {
  _config_file_path = path;
}

bool ConfigParser::Parse() {
    std::ifstream file(_config_file_path);
    if (!file.is_open()) {
      return false;
    }
    std::string line;
    std::string currentSection;
    while (std::getline(file, line)) {
      Trim(line); // 去除行首尾空白
      if (line.empty() || line[0] == '#') {
        // 跳过空行和注释行
        continue;
      } else if (line[0] == '[' && line.back() == ']') {
        // 配置段标头
        currentSection = line.substr(1, line.size() - 2);
      } else {
        // 键值对
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
          std::string key = Trim(line.substr(0, pos));
          std::string value = Trim(line.substr(pos + 1));
          _config_data[currentSection][key] = value;
        }
      }
    }
    return true;
}

// 从配置文件中获取整数配置项值，如果配置项不存在则返回默认值
int ConfigParser::GetInt(const std::string& section, const std::string& key, int defaultValue) const {
    auto it = _config_data.find(section);
    if (it != _config_data.end()) {
        auto valueIt = it->second.find(key);
        if (valueIt != it->second.end()) {
            return std::stoi(valueIt->second);  // 将配置项值字符串转换为整数并返回
        }
    }
    return defaultValue;  // 配置项不存在时返回默认值
}

// 从配置文件中获取字符串配置项值，如果配置项不存在则返回默认值
std::string ConfigParser::GetString(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    auto it = _config_data.find(section);
    if (it != _config_data.end()) {
        auto valueIt = it->second.find(key);
        if (valueIt != it->second.end()) {
            return valueIt->second;  // 返回配置项的值
        }
    }
    return defaultValue;  // 配置项不存在时返回默认值
}

// 去除字符串两端的空白字符（空格和制表符）
std::string ConfigParser::Trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");   
    if (start != std::string::npos && end != std::string::npos) {
        return str.substr(start, end - start + 1);  
    }
    return "";
}
