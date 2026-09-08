clang -O2 -I ../../h -target bpf -c *_sample.c
clang -O2 -I ../../h -target bpf -c *_kern.c
cp *.o ../../bin/samples/
