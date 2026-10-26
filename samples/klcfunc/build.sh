clang -O2 -I ../../h -I /usr/include/aarch64-linux-gnu -target bpf -c *_sample.c
clang -O2 -I ../../h -I /usr/include/aarch64-linux-gnu -target bpf -c *_kern.c
cp *.o ../../bin/samples/
