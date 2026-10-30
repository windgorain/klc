#!/bin/bash

if [ "$(clang -dumpversion | cut -d. -f1)" -lt 19 ]; then
    echo "Need clang version >= 19"
    exit 1
fi

cd samples/hello_world
make
cd - > /dev/null

cd samples/klcfunc
make
cd - > /dev/null

cd samples/tcp_echo
make
cd - > /dev/null

cd samples/klua
make
cd - > /dev/null

cd samples/luaxdp
make
cd - > /dev/null

cd spf/kapp/sniffer
make
cd - > /dev/null

cd spf/kapp/pktgen
make
cd - > /dev/null

cd spf/kapp/dnsblock
make
cd - > /dev/null

cd spf/kapp/dnsdoctor
make
cd - > /dev/null

cd spf/drivers/snull
make
cd - > /dev/null

