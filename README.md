# 说明
KLC(Kernel Level Context)是一款Linux内核SPF运行时

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
