#include <unistd.h>
#include "../include/webserver.h"

int main(int argc, char* argv[], char* envp[]) {
    /* 守护进程 后台运行 */
    //daemon(1, 0); 

    //WebServer server(
    //    9321, 3, 60000, false,             /* 端口 ET模式 timeoutMs 优雅退出  */
    //    3306, "root", "Always915321.", "users", /* Mysql配置 */
    //    12, 6, true, 0, 1024);             /* 连接池数量 线程池数量 日志开关 日志等级 日志异步队列容量 */
    

    if(argc <= 1) {
      std::cout << "配置文件使用默认路径" << std::endl;
    }
    else ConfigParser::Instance()->SetConfigFilePath(argv[1]);
    WebServer server;
    server.Start();
} 
  
