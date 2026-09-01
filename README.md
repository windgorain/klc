# 说明
KLC(Kernel Level Context)是一款Linux内核SPF运行时

# 1. 编译
./build.sh

# 2. 运行
cd app
# x64系统上: 
./loadklc.x64.sh
# arm64系统上: 
./loadklc.arm64.sh

# 3. 测试
cd samples
# x64系统上: 
./test_samples.x64.sh
# arm64系统上: 
./test_samples.arm64.sh


