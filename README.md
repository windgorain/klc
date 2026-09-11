# 说明
KLC(Kernel Level Context)是一款Linux内核SPF运行时.  
SPF文件: 是一种和系统无关的通用文件格式,只要系统上存在运行时,即可运行. 可以运行在内核态、用户态、windows、Linux、MacOS、嵌入式、uboot等各种支持SPF运行时的环境。 
只需要编译加载KLC, 即可在Linux内核运行SPF文件.  
可以通过klcfunc实现自定义ebpf helper.  
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
#运行lua测试
cd bin
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

