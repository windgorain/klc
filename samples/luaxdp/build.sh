clang -O2 -g -I ../../h -target bpf -c *_kern.c
gcc -O2 -I ../../h *_user.c -lbpf -o luaxdp
cp *.o ../../bin/samples/
cp luaxdp ../../bin/samples/

