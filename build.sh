#!/bin/bash

ARCH=$(uname -m)

if [[ "$ARCH" == "x86_64" ]]; then
    cp bin/arch/klctool-x64 bin/klctool
elif [[ "$ARCH" == "aarch64" ]] || [[ "$ARCH" == "arm64" ]]; then
    cp bin/arch/klctool-arm64 bin/klctool
else
    echo "暂不支持CPU: $ARCH"
    exit;
fi

cd src_linux/klc/ko/
make
cp *.ko ../../../bin/
cd - > /dev/null

cd samples/hello_world/
./build.sh
cd - > /dev/null

cd samples/klcfunc/
./build.sh
cd - > /dev/null

cd samples/klua/
./build.sh
cd - > /dev/null

cd samples/luaxdp/
./build.sh
cd - > /dev/null

cd spf/kapp/http_sniffer
./build.sh
cd - > /dev/null

