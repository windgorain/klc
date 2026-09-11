clang -O2 -I ../../../h -target bpf -c *.c
cp *.o ../../../bin/spf/kapp/http_sniffer.o
