#cmake最低版本
cmake_minimum_required(VERSION 3.1.0)

#项目名
project(WEB_SERVER)

# 添加头文件路径
include_directories(include)

# 收集源文件
file(GLOB SOURCES src/*.cpp)

# 添加可执行文件
add_executable(webServer ${SOURCES})
