clang -O2 -I ../../../h -fno-builtin-memset -target bpf -c *.c
cp *.o ../../../bin/spf/kapp/http_sniffer.o
