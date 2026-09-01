#!/bin/sh

KLCTOOL=../klctool/klctool-arm64

${KLCTOOL} load spf -i hello_world ./hello_world.o -r
${KLCTOOL} cmdrun hello_world

sudo dmesg -T | grep "Hello world"

