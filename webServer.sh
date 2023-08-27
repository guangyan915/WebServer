#!/bin/bash

DAEMON_PATH="./webServer"

start() {
    if [ -z "$1" ]; then
        echo "缺少配置文件路径。使用方法：$0 start <config_path>"
        exit 1
    fi
    if [ ! -f "$1" ]; then
        echo "配置文件不存在：$1"
        exit 1
    fi
    echo "正在启动守护进程..."
    $DAEMON_PATH "$1" &
    echo "守护进程启动成功。"
}

stop() {
    echo "正在停止守护进程..."
    pkill -f $DAEMON_PATH
    echo "守护进程停止成功。"
}

restart() {
    if [ -z "$1" ]; then
        echo "缺少配置文件路径。使用方法：$0 restart <config_path>"
        exit 1
    fi
    if [ ! -f "$1" ]; then
        echo "配置文件不存在：$1"
        exit 1
    fi
    stop
    sleep 1
    start "$1"
}

case "$1" in
    start)
        start "$2"
        ;;
    stop)
        stop
        ;;
    restart)
        restart "$2"
        ;;
    *)
        echo "使用方法：$0 {start <config_path>|stop|restart <config_path>}"
        exit 1
        ;;
esac

exit 0

