#include "sqlconnectionpool.h"
#include "sqlconnectionRAII.h"

class MysqlOpt {
public:
  static int Register(const std::string& name, const std::string& phone, const std::string& passwd);
  static int Login(const std::string&name, const std::string& passwd);
};
