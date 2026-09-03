# 说明
KLC(Kernel Level Context)是一款Linux内核SPF运行时.  
SPF文件: 是一种和系统无关的通用文件格式,只要系统上存在运行时,即可运行. 可以运行在内核态、用户态、windows、Linux、MacOS、嵌入式、uboot等各种支持SPF运行时的环境。 
只需要编译加载KLC, 即可在Linux内核运行SPF文件.  
# 编译
./build.sh

# 加载到内核
```
cd bin
# x64系统上: 
sudo ./loadklc.x64.sh
# arm64系统上: 
sudo ./loadklc.arm64.sh
```

# 测试
```
# x64系统上: 
sudo ./samples/test_samples.x64.sh
# arm64系统上: 
sudo ./samples/test_samples.arm64.sh
```

# 测试内核Lua
```
# x64系统上: 
sudo ./app/lua/test_lua.x64.sh
# arm64系统上: 
sudo ./app/lua/test_lua.arm64.sh
```

# 应用代码示例
```
# klc/samples 下存放了应用代码示例

# 以hello_world为例
cd klc/samples/hello_world
./build.sh
# 会生成hello_world.o文件，这个文件可被加载到KLC环境执行
```

