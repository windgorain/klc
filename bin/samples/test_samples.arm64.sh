#!/bin/sh

KLCTOOL=./klctool/klctool-arm64

${KLCTOOL} load spf -i hello_world ./samples/hello_world.o -r
${KLCTOOL} run hello_world

sudo dmesg -T | grep "Hello world"

