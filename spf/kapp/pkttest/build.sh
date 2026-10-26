clang -O2 -I ../../../h -I /usr/include/aarch64-linux-gnu -fno-builtin-memset -target bpf -c *.c
cp *.o ../../../bin/spf/kapp/pkttest.o
