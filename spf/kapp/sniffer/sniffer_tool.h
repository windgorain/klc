/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-13
* Description: 
******************************************************************************/
#ifndef _SNIFFER_TOOL_H_
#define _SNIFFER_TOOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DIR_IN  0
#define DIR_OUT 1

typedef struct {
    U32 port_bitmap[65536/32];
}SNIFFER_PORTS_S;

static inline void _sniffer_print(LSTR_S *str, int dir)
{
    char info[128] = "";

    int copy_len = MIN(sizeof(info)-1, str->uiLen);

    _memcpy(info, str->pcData, copy_len);

    char *dir_str = dir == DIR_IN ? "Input" : "Output";
    BPF_Print("%s: %s", dir_str, info);
}

static void _sniffer_show_ports(OUT char *buf, int buf_size, SNIFFER_PORTS_S *ports)
{
    int i;
    int len;
    int count = 0;

    len = snprintf(buf, buf_size, "Sniffer ports:\n");

    for (i=1; i<65536; i++) {
        if (buf_size < len + 50) {
            break;
        }

        if (ArrayBit_Test(ports->port_bitmap, htons(i))) {
            count ++;
            len += snprintf(buf + len, buf_size - len, " %u", i);
            if (count % 8 == 0) {
                len += snprintf(buf + len, buf_size - len, "\n");
            }
        }
    }

    snprintf(buf + len, buf_size - len, "\n");
}

static void _sniffer_add_ports(U16 min_port, U16 max_port, SNIFFER_PORTS_S *ports)
{
    for (int i=min_port; i<=max_port; i++) {
        ArrayBit_Set(ports->port_bitmap, htons(i));
    }
}

static void _sniffer_set_ports(U16 min_port, U16 max_port, SNIFFER_PORTS_S *ports)
{
    memset(ports, 0, sizeof(SNIFFER_PORTS_S));
    _sniffer_add_ports(min_port, max_port, ports);
}

static void _sniffer_del_ports(U16 min_port, U16 max_port, SNIFFER_PORTS_S *ports)
{
    for (int i=min_port; i<=max_port; i++) {
        ArrayBit_Clr(ports->port_bitmap, htons(i));
    }
}

static void _sniffer_cmd_help(char *buf, int buf_size)
{
    snprintf(buf, buf_size, "Usage: \n"
            "show port\n"
            "add port min max\n"
            "del port min max\n"
            "set port min max\n");
}

static void _sniffer_set_port(int argc, char **argv, char *output_buf, int buf_size, SNIFFER_PORTS_S *ports)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _sniffer_set_ports(min_port, max_port, ports);
}

static void _sniffer_add_port(int argc, char **argv, char *output_buf, int buf_size, SNIFFER_PORTS_S *ports)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _sniffer_add_ports(min_port, max_port, ports);
}

static void _sniffer_del_port(int argc, char **argv, char *output_buf, int buf_size, SNIFFER_PORTS_S *ports)
{
    char *min_port_str = (argc > 3) ? argv[3] : "0";
    char *max_port_str = (argc > 4) ? argv[4] : "0";
    U16 min_port = atoi(min_port_str);
    U16 max_port = atoi(max_port_str);

    if (max_port < min_port) {
        U16 tmp = min_port;
        min_port = max_port;
        max_port = tmp;
    }

    _sniffer_del_ports(min_port, max_port, ports);
}

static int sniffer_cmd_main(int argc, char **argv, char *output_buf, int buf_size, SNIFFER_PORTS_S *ports)
{
    if (argc < 3) {
        _sniffer_cmd_help(output_buf, buf_size);
        return -1;
    }

    if ((strcmp(argv[1], "show")== 0) && (strcmp(argv[2], "port") == 0)) {
        _sniffer_show_ports(output_buf, buf_size, ports);
    } else if ((strcmp(argv[1], "set")== 0) && (strcmp(argv[2], "port") == 0)) {
        _sniffer_set_port(argc, argv, output_buf, buf_size, ports);
    } else if ((strcmp(argv[1], "add")== 0) && (strcmp(argv[2], "port") == 0)) {
        _sniffer_add_port(argc, argv, output_buf, buf_size, ports);
    } else if ((strcmp(argv[1], "del")== 0) && (strcmp(argv[2], "port") == 0)) {
        _sniffer_del_port(argc, argv, output_buf, buf_size, ports);
    } else {
        _sniffer_cmd_help(output_buf, buf_size);
        return -1;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif
