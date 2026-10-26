clang -O2 -I ../../h -I /usr/include/aarch64-linux-gnu -target bpf -c *.c
cp *.o ../../bin/samples/
