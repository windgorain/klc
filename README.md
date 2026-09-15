# 说明
KLC(Kernel Level Context)是一款Linux内核SPF运行时.  
SPF文件: 是一种和系统无关的通用文件格式,只要系统上存在运行时,即可运行. 可以运行在内核态、用户态、windows、Linux、MacOS、嵌入式、uboot等各种支持SPF运行时的环境。 
只需要编译加载KLC, 即可在Linux内核运行SPF文件.  

# 特点
1. 可以通过klcfunc实现自定义ebpf helper, 方便扩展ebpf功能. 
2. APP只需要编译一次, 即可在各种不同版本内核上运行, 无需再次编译(Run Anywhere). 
3. APP无需发布源代码, 只需要发布编译后的文件, 用户无需编译即可加载使用APP. 
4. 一个APP可以加载为多个实例, 也可同时加载多个APP的不同版本, 方便不断流升级.  
5. 兼容性良好, 部分APP即可在用户态运行, 又可在内核态运行, 方便调试稳定后下发到内核.

# 编译
./build.sh

# 加载环境到内核
```
cd bin
sudo ./loadklc.sh
cd -
```

# 测试
```
cd bin

#运行hello world
sudo ./samples/test_hello_world.sh

#在XDP程序中调用自定义函数
sudo ./samples/test_klcfunc.sh eth0

#在内核中打印LUA信息
sudo ./app/lua/test_lua.sh

#在XDP程序中使用LUA
sudo ./samples/test_luaxdp.sh eth0

#监控本机请求的HTTP域名, 并将进程名/进程号/域名打印出来 
sudo ./spf/kapp/http_sniffer_load.sh

#监控本机请求的SSL域名, 并将进程名/进程号/域名打印出来 
sudo ./spf/kapp/sni_sniffer_load.sh

#驱动示例
sudo ./klctool load spf drivers/snull.o
sudo ./klctool load spf drivers/snull.o -i snull2
ip link
sudo ./klctool unload instance snull
sudo ./klctool unload instance snull2

cd -
```

# 示例程序说明
| 名称 | 说明 |
| --- | --- |
| samples/hello_world| KLC Hello World 程序 |
| samples/klcfunc | 使用EBPF扩展EBPF Helper功能 |
| samples/klua | 在内核中打印LUA信息 |
| samples/luaxdp | 在XDP程序中使用LUA |
| spf/kapp/http_sniffer | 监控本机请求的HTTP域名, 并将进程名/进程号/域名打印出来 |
| spf/drivers/snull | 简单的网络设备驱动示例 |

