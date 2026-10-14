#!/bin/bash

cd samples/hello_world
./build.sh
cd - > /dev/null

cd samples/klcfunc
./build.sh
cd - > /dev/null

cd samples/tcp_echo
./build.sh
cd - > /dev/null

cd samples/klua
./build.sh
cd - > /dev/null

cd samples/luaxdp
./build.sh
cd - > /dev/null

cd spf/kapp/sniffer
./build.sh
cd - > /dev/null

cd spf/drivers/snull
./build.sh
cd - > /dev/null

