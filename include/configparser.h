#pragma once

#include <string>
#include <map>
#include <iostream>

#define DEFAULT_CONFIG_FILE_PATH "../webserver.conf"

class ConfigParser {
public:
    static ConfigParser* Instance();
    
    
    void SetConfigFilePath(const std::string& path);
    // 解析配置文件
    bool Parse();
    // 获取整数配置项
    int GetInt(const std::string& section, const std::string& key, int defaultValue = 0) const;
    // 获取字符串配置项
    std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;
    
    /*
    void print() {
      for(auto it = _config_data.begin(); it != _config_data.end(); it++) {
        std::cout << "[" << it->first << "]" << std::endl;
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
          std::cout << it2->first << ":" << it2->second << std::endl;
        }
      }
    }
    */

private:
    ConfigParser() = delete;
    ConfigParser(const std::string& configFile);
    std::string _config_file_path;
    std::map<std::string, std::map<std::string, std::string>> _config_data;
    //去除字符串首尾空白
    static std::string Trim(const std::string& str);
};

