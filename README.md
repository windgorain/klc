# 说明
KLC (Kernel Level Context) is a Linux kernel SPF runtime.  
SPF file: It's a universal file format that's system-independent. As long as the runtime exists on the system, it can run. It can work in kernel mode, user mode, Windows, Linux, MacOS, embedded systems, uboot, and any other environment that supports the SPF runtime.  
You just need to compile and load KLC to run SPF files in the Linux kernel.  

# Features
1. You can use klcfunc to implement custom eBPF helpers, making it easy to extend eBPF functionality. 
2. The app only needs to be compiled once and can run on various kernel versions without compiling again (Run Anywhere). 
3. The app doesn't need to release source code—just the compiled files are enough, and users can load and use the app without compiling. 
4. One app can be loaded as multiple instances, and different versions of multiple apps can be loaded simultaneously, making seamless upgrades easy. 
5. Good compatibility: some apps can run in user space or kernel space, which is convenient for debugging before rolling out to the kernel. 

# Compile
./build.sh

# Load
```
cd bin
sudo ./loadklc.sh
cd -
```

# Test
```
cd bin

#hello world
sudo ./samples/test_hello_world.sh

#Calling a custom function in an XDP program
sudo ./samples/test_klcfunc.sh eth0

#Print LUA information in the kernel
sudo ./app/lua/test_lua.sh

#Using LUA in XDP programs
sudo ./samples/test_luaxdp.sh eth0

# Monitor HTTP domain requests on this machine, and print out the process name/PID/domain
sudo ./spf/kapp/http_sniffer_load.sh

#Monitor SSL domains requested by the local machine, and print out the process name/PID/domain
sudo ./spf/kapp/sni_sniffer_load.sh

#Driver Example
sudo ./klctool load spf drivers/snull.o
sudo ./klctool load spf drivers/snull.o -i snull2
ip link
sudo ./klctool unload instance snull
sudo ./klctool unload instance snull2

cd -
```

# Example Program Description
| Name | Description |
| --- | --- |
| samples/hello_world | KLC Hello World Program |
| samples/klcfunc | Use EBPF to extend EBPF Helper functions |
| samples/klua | Print LUA info in the kernel |
| samples/luaxdp | Use LUA in XDP programs |
| spf/kapp/http_sniffer | Monitor HTTP domains requested by the local machine, and print the process name/ID/domain |
| spf/drivers/snull | Simple network device driver |

