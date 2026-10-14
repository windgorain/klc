/******************************************************************************
* Copyright (C), Xingang.Li
* Author:      Xingang.Li  Version: 1.0
* Description:
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "utl/ulc_user.h"
#include "spf/spf_string.h"
#include "spf/core/spf_module.h"
#include "spf/klc/klc_sym.h"

typedef struct {
    U32 valid: 1;
    int fd;
}KTCPECHO_S;

static U16 g_tcp_echo_port = 9377; 
static int g_tcp_echo_running = 0;
static int g_tcp_echo_stopping = 0;
static int g_tcp_echo_service_fd = -1;
static KTCPECHO_S g_tcp_echo_workers[128];


static int _tcp_echo_add_worker(int fd)
{
    for (int i=0; i<ARRAY_SIZE(g_tcp_echo_workers); i++) {
        if (! g_tcp_echo_workers[i].valid) {
            g_tcp_echo_workers[i].fd = fd;
            g_tcp_echo_workers[i].valid = 1;
            return i;
        }
    }

    return -1;
}

static void _tcp_echo_del_worker(U32 worker_id)
{
    if (worker_id < ARRAY_SIZE(g_tcp_echo_workers)) {
        g_tcp_echo_workers[worker_id].valid = 0;
        g_tcp_echo_workers[worker_id].fd = -1;
    }
}

static BOOL_T _tcp_echo_is_worker_exist(void)
{
    for (int i=0; i<ARRAY_SIZE(g_tcp_echo_workers); i++) {
        if (g_tcp_echo_workers[i].valid) {
            return TRUE;
        }
    }

    return FALSE;
}

static void * _tcp_echo_task_worker(void *arg)
{
    int worker_id = (long)arg;
    int fd = g_tcp_echo_workers[worker_id].fd;
    char buf[256];

    while (1) {
        int len = recv(fd, buf, sizeof(buf), 0);
        if (len <= 0) {
            break;
        }
        send(fd, buf, len, 0);
    }

    _tcp_echo_del_worker(worker_id);
    close(fd);

    return NULL;
}

static void _tcp_echo_start_worker(int fd)
{
    unsigned long tid;
    int worker_id;

    worker_id = _tcp_echo_add_worker(fd);
    if (worker_id < 0) {
        close(fd);
        return;
    }

    int ret = ulc_pthread_create(&tid, NULL, _tcp_echo_task_worker, (void*)(long)worker_id);
    if (ret < 0) {
        _tcp_echo_del_worker(worker_id);
        close(fd);
        return;
    }

    ulc_pthread_detach(tid); 
}

static void * _tcp_echo_task_main(void *arg)
{
    while (g_tcp_echo_stopping == 0) {
        struct sockaddr server_addr;
        int len = sizeof(server_addr);
        int fd = accept(g_tcp_echo_service_fd, &server_addr, &len);
        if (fd >= 0) {
            _tcp_echo_start_worker(fd);
        } else {
            ulc_sys_usleep(10000);
        }
    }

    close(g_tcp_echo_service_fd);
    g_tcp_echo_service_fd = -1;
    g_tcp_echo_running = 0;

    return NULL;
}

static void _tcp_echo_cmd_help(char *buf, int buf_size)
{
    snprintf(buf, buf_size, "Usage:\n"
            " start [port]\n"
            " stop\n");
}

static int _tcp_echo_start(int argc, char **argv, OUT char *buf, int buf_size)
{
    int ret;
    int fd;
    U16 port = 9377;
    unsigned long tid;

    if (g_tcp_echo_running) {
        snprintf(buf, buf_size, "Should stop service first\n");
        return -1;
    }

    if (argc >= 3) {
        port = atoi(argv[2]);
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        snprintf(buf, buf_size, "Can't create tcp socket\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        snprintf(buf, buf_size, "Can't bind tcp socket\n");
        close(fd);
        return -1;
    }

    listen(fd, 5);

    g_tcp_echo_port = port;
    g_tcp_echo_service_fd = fd;
    g_tcp_echo_running = 1;

    int flags = ulc_sys_fcntl(fd, F_GETFL, 0); 
    ulc_sys_fcntl(fd, F_SETFL, flags | O_NONBLOCK); 

    ret = ulc_pthread_create(&tid, NULL, _tcp_echo_task_main, NULL);
    if (ret < 0) {
        g_tcp_echo_service_fd = -1;
        g_tcp_echo_running = 0;
        close(fd);
        snprintf(buf, buf_size, "Create main thread failed\n");
        return -1;
    }

    ulc_pthread_set_name(tid, "tcpecho-main");
    ulc_pthread_detach(tid); 

    return 0;
}

static int _tcp_echo_stop(void)
{
    if (g_tcp_echo_running) {
        g_tcp_echo_stopping = 1;
    }

    while (g_tcp_echo_running) {
        ulc_sys_usleep(10000);
    }

    g_tcp_echo_stopping = 0;

    return 0;
}

static int _tcp_echo_cmd(int argc, char **argv, char *output_buf, int buf_size)
{
    if (argc < 2) {
        _tcp_echo_cmd_help(output_buf, buf_size);
        return -1;
    }

    if (strcmp(argv[1], "start")== 0) {
        _tcp_echo_start(argc, argv, output_buf, buf_size);
    } else if (strcmp(argv[1], "stop")== 0) {
        _tcp_echo_stop();
    } else {
        _tcp_echo_cmd_help(output_buf, buf_size);
        return -1;
    }

    return 0;
}


static int _tcp_echo_event_init(void)
{
    return 0;
}

static int _tcp_echo_event_fini1(void)
{
    _tcp_echo_stop();

    for (int i=0; i<ARRAY_SIZE(g_tcp_echo_workers); i++) {
        int fd = g_tcp_echo_workers[i].fd;
        if ((g_tcp_echo_workers[i].valid) && (fd >= 0)) {
            shutdown(fd, SHUT_RDWR);
        }
    }

    while (_tcp_echo_is_worker_exist()) {
        ulc_sys_usleep(10000);
    }

    return 0;
}

SEC(".spf.cmd/main")
int _tcp_echo_main(int argc, char **argv, char *output_buf, int buf_size)
{
    return _tcp_echo_cmd(argc, argv, output_buf, buf_size);
}

SEC(SPF_SEC_EVENT)
int event(U32 event)
{
    switch (event) {
        case SPF_EVENT_INIT:
            return _tcp_echo_event_init();
        case SPF_EVENT_FIN1:
            return _tcp_echo_event_fini1();
    }
    return 0;
}

SPF_MOD_COMMON_DEF(1)

