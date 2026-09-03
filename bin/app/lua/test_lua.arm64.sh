#!/bin/sh

KLCTOOL=./klctool/klctool-arm64

${KLCTOOL} load spf app/lua/lua5.1.spf -r
${KLCTOOL} run lua5.1 -v
${KLCTOOL} run lua5.1 -e "print('hello world by Lua')"
dmesg | grep "Lua"

${KLCTOOL} load spf app/lua/klua_test.o -r
${KLCTOOL} run klua_test
dmesg | grep "klua"
