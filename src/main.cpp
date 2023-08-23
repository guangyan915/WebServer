#include <unistd.h>
#include <signal.h>
#include "../include/webserver.h"
#include <iostream>
#include <fstream>

void WebDaemon(void)
{
	int fd0;
	pid_t pid;
	struct sigaction sa;
 
	//1.调用umask将文件模式创建屏蔽字设置为0
	umask(0);
 
	//2.调用fork，父进程退出（exit）
	if ((pid = fork()) < 0)
	{
		perror("fork");
	}
	else if (pid>0)
	{
		exit(0);
	}
 
	//3.调用setsid创建一个新的会话
	setsid();
 
	//4.忽略SIGCHLLD信号
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	//注册子进程退出，忽略信号
	if (sigaction(SIGCHLD, &sa, NULL) < 0)
	{
		return;
	}
	//再次fork，终止父进程，保持子进程不是会话首进程，从而保证后续不在和其他终端关联
	//这部分不是必须的
	if ((pid = fork()) < 0)
	{
		printf("fork error!\n");
		return;
	}
	else if (pid != 0)
	{
		exit(0);
	}
	
	//5.将当前工作目录更改为根目录
	//if (chdir("/") < 0)
	//{
	//	printf("child dir error\n");
	//	return;
	//}
 
	//6.关闭不在需要的文件描述符，或者重定向到 /dev/null
	close(0);
  // /dev/null:linux下的黑洞，写入的所有数据会直接丢弃。
	fd0 = open("/dev/null", O_RDWR);
	dup2(fd0, 1);
	dup2(fd0, 2);
}


int main(int argc, char* argv[], char* envp[]) {
    
    if(argc < 2) {
        std::cout << "缺少配置文件路径！" << std::endl;
        std::cout << "格式:./webServer /path/..." << std::endl;
        return -1;
    }
    
    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << argv[1] << "不存在" << std::endl;
        return -2;
    }
    
    // 守护进程
    WebDaemon();

    ConfigParser::Instance()->SetConfigFilePath(argv[1]);
    WebServer server;
    server.Start();
} 
  
