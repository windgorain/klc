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

# 测试示例
```
cd bin

#运行lua测试
sudo ./app/lua/test_lua.sh

#运行hello world
sudo ./samples/test_hello_world.sh

#在XDP程序中调用自定义函数
sudo ./samples/test_klcfunc.sh eth0

cd -
```

# 应用代码示例
```
# klc/samples 下存放了应用代码示例

# 以klcfunc自定义函数为例
cd samples/klcfunc
# 可以修改klcfunc的实现, 定义自己的namefunc和idfunc
./build.sh
```

# 常用应用
## HTTP Sniffer
用于监控本机请求的HTTP域名, 并将进程名/进程号/域名打印出来 
```
cd spf/kapp/http_sniffer
./build.sh
cd -

cd bin
sudo ./spf/kapp/http_sniffer_load.sh
cd -
```

## SNI Sniffer
用于监控本机请求的SSL域名, 并将进程名/进程号/域名打印出来 
```
cd bin
sudo ./spf/kapp/sni_sniffer_load.sh
cd -
```

