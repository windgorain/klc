#!/bin/bash

IF_NAME=eth0
if [ "$1" ];then
    IF_NAME=$1
fi

KLCTOOL=./klctool

${KLCTOOL} load spf -i klcfunc_test ./samples/klcfunc_sample.o -r

ip link set dev ${IF_NAME} xdp obj ./samples/klcfunc_test_kern.o sec xdp

#退出处理
quit_process() {
    echo ""
    echo "quit..."
    ip link set dev ${IF_NAME} xdp off
    exit
}

trap quit_process INT

echo "Please visit this computer by http. eg: curl --ipv4 http://target"
echo "Press Ctrl+C to quit"

cat /sys/kernel/debug/tracing/trace_pipe

