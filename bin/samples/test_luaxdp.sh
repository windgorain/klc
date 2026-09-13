#!/bin/bash

IF_NAME=eth0
if [ "$1" ];then
    IF_NAME=$1
fi

./klctool load spf app/lua/lua5.1.spf
ip link set dev ${IF_NAME} xdp obj ./samples/luaxdp_kern.o sec xdp
./samples/luaxdp ./samples/luaxdp_test.lua

#退出处理
quit_process() {
    echo ""
    echo "quit..."
    ip link set dev ${IF_NAME} xdp off
    exit
}

trap quit_process INT

echo "Please visit this computer by http. eg: curl http://target"
echo "Press Ctrl+C to quit"

cat /sys/kernel/debug/tracing/trace_pipe

