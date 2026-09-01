#!/bin/bash

KLCTOOL=./klctool/klctool-x64

sudo mountpoint -q /sys/fs/bpf
if [ $? != 0 ];then
  sudo mount -t bpf bpf /sys/fs/bpf
fi

./unloadklc.sh

sudo sysctl -w net.core.rmem_max=2097152  # 设置为 2MB
sudo sysctl -w net.core.wmem_max=2097152  # 设置为 2MB

sudo insmod klc_base.ko > /dev/null 2>&1
sudo insmod klc_oscomp.ko
sudo insmod klc_impl.ko

sudo ${KLCTOOL} init
sudo ${KLCTOOL} load bare spf/bare/spf_loader.[ARCH].bare
sudo ${KLCTOOL} load spf -i kbase.spfmap spf/base/spfmap.[ARCH].spf
sudo ${KLCTOOL} load spf -i kbase.spfvm spf/base/spfvm.[ARCH].spf
sudo ${KLCTOOL} load spf -i kbase.khelper khelper_klc.spf
sudo ${KLCTOOL} load spf -i kbase.hookspf hookspf_klc.spf

