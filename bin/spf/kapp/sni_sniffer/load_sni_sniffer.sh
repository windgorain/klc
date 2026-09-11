#!/bin/bash

KLCTOOL=./klctool

#加载sni sniffer
${KLCTOOL} load spf spf/kapp/sni_sniffer/sni_sniffer.spf -r

#开启trace
sh -c 'echo 1 > /sys/kernel/debug/tracing/events/bpf_trace/bpf_trace_printk/enable'

#退出处理
quit_process() {
    echo ""
    echo "quit..."
    ${KLCTOOL} unload instance sni_sniffer
    exit
}

trap quit_process INT

echo "按 Ctrl+C 退出"

cat /sys/kernel/debug/tracing/trace_pipe

