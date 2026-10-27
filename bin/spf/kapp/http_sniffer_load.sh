#!/bin/bash

KLCTOOL=./klctool

#加载http sniffer
${KLCTOOL} load spf spf/kapp/http_sniffer.o -r

#开启trace
sh -c 'echo 1 > /sys/kernel/debug/tracing/events/bpf_trace/bpf_trace_printk/enable'

#退出处理
quit_process() {
    echo ""
    echo "quit..."
    ${KLCTOOL} unload instance http_sniffer
    exit
}

trap quit_process INT

echo "test: curl --ipv4 http://www.baidu.com"
echo "Press Ctrl+C to quit"

cat /sys/kernel/debug/tracing/trace_pipe

