if [ "$(clang -dumpversion | cut -d. -f1)" -lt 18 ]; then
    echo "Need clang version >= 18"
    exit 1
fi

clang -O2 -g -I ../../h -target bpf -c *_kern.c
gcc -O2 -I ../../h *_user.c -lbpf -o luaxdp
cp *.o ../../bin/samples/
cp luaxdp ../../bin/samples/

