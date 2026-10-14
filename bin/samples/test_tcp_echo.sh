#!/bin/sh

KLCTOOL=./klctool

${KLCTOOL} load spf ./samples/tcp_echo.o -r
${KLCTOOL} run tcp_echo start 11111

echo "Please nc 127.0.0.1 11111 to test tcp echo"
echo "Use 'klctool run tcp_echo stop' to stop the tcp echo service"

