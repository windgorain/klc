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
cd ../../../

