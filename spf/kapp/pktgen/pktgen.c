/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-20
* Description: pkt test: 发送测试报文
******************************************************************************/
#include "utl/ulc_user.h"
#include "bpf/tcp_bpf.h"
#include "bpf/udp_bpf.h"
#include "bpf/ipcsum_bpf.h"
#include "spf/core/spf_module.h"
#include "spf/klc/klc_sym.h"
#include "utl/array_bit_opt.h"
#include "utl/eth_utl.h"

#define PKTGEN_BURST_DFT 16

typedef struct {
    U32 woker_id;
    void *task;
    void *skb;
    void *dev;
}PKTGEN_S;

static char g_pktgen_dev_name[IFNAMSIZ];
static U8 g_pktgen_dst_mac[MAC_ADDR_LEN];
static U32 g_pktgen_burst = PKTGEN_BURST_DFT;
static int g_pktgen_worker_num;
static PKTGEN_S g_pktgen_workers[8];
static KLC_NETDEV_INFO_S g_pktgen_devinfo;

static void _pktgen_cmd_help(char *buf, int buf_size)
{
    snprintf(buf, buf_size, "Usage:\n"
            " show\n"
            " start DEV [WorkerNum]\n"
            " stop\n"
            " burst COUNT\n"
            " dmac MAC\n");
}

static PKTGEN_S * _pktgen_get_worker(U32 index)
{
    if (index > ARRAY_SIZE(g_pktgen_workers)) {
        return NULL;
    }

    return &g_pktgen_workers[index];
}

static struct sk_buff * _pktgen_alloc_skb(void)
{
    return klc_create_skb(1024, KLC_GFP_KERNEL);
}

static struct sk_buff * _pktgen_fill_packet_ipv4(struct net_device *dev, KLC_NETDEV_INFO_S *dev_info)
{
    int skb_len = 0;
    int ip_len;
    int udp_len;
    int data_len = 18; 

    struct sk_buff *skb = _pktgen_alloc_skb();
    if (! skb) {
        return NULL;
    }

	ETH_HEADER_S *eth = skb_push(skb, 14); 
    skb_len += 14;
	skb_reset_mac_header(skb);  

    memcpy(eth->stSMac.aucMac, dev_info->dev_addr, MAC_ADDR_LEN);
    memcpy(eth->stDMac.aucMac, g_pktgen_dst_mac, MAC_ADDR_LEN);
    eth->usProto = htons(ETH_P_IP);

    skb_set_network_header(skb, 14); 
	IP_HEAD_S *iph = skb_put(skb, sizeof(IP_HEAD_S));
    skb_len += sizeof(IP_HEAD_S);

    ip_len = sizeof(IP_HEAD_S) + sizeof(UDP_HEAD_S) + data_len;
    IPBPF_BuildHeader(iph, IP_PROTO_UDP, 0x0202a8c0, 0x0102a8c0, ip_len, htons(1));
    IPBPF_Checksum(iph);

	skb_set_transport_header(skb, skb_len);
	UDP_HEAD_S *udph = skb_put(skb, sizeof(UDP_HEAD_S));
    skb_len += sizeof(UDP_HEAD_S);

    udph->usSrcPort = htons(1234);
    udph->usDstPort = htons(1234);
    udph->usCrc = 0;
    udp_len = data_len + sizeof(UDP_HEAD_S);
    udph->usDataLength = htons(udp_len);

	skb_put(skb, data_len);

    klc_skb_set_protocol(skb, htons(ETH_P_IP));
    klc_skb_set_dev(skb, dev);
    klc_skb_set_pkt_type(skb, PACKET_HOST);
    klc_skb_set_ip_summed(skb, CHECKSUM_NONE);
    skb_set_queue_mapping(skb, 0);

    return skb;
}

static void _pktgen_xmit(PKTGEN_S *w, KLC_NETDEV_INFO_S *dev_info)
{
    struct sk_buff *skb = w->skb;

    w->skb = NULL;

    if (! skb) {
        skb = _pktgen_fill_packet_ipv4(w->dev, dev_info);
        if (! skb) { 
            return;
        }
    }

    int que_index = w->woker_id % dev_info->real_num_tx_queues;

    struct netdev_queue *txq = netdev_get_tx_queue(w->dev, que_index);

    int burst = g_pktgen_burst;
    klc_skb_add_users(skb, burst);

    local_bh_disable();
    klc_hard_tx_lock(w->dev, txq);

    int busy = 0;

    while ((burst > 0) && (! netif_xmit_frozen_or_drv_stopped(txq))) {
        int ret = netdev_start_xmit(skb, w->dev, txq, --burst > 0);
        if (! dev_xmit_complete(ret)) {
            klc_skb_add_users(skb, -1);
            busy = 1;
            break;
        }
		if (need_resched()) {
            break;
        }
    }

    if (unlikely(burst)) {
        klc_skb_add_users(skb, -burst);
    }

    klc_hard_tx_unlock(w->dev, txq);
    local_bh_enable();

    if (busy) {
        w->skb = skb;

    } else {
        kfree_skb(skb);
    }
}

static int _pktgen_worker(void *data)
{
    PKTGEN_S *w = data;

    while (! kthread_should_stop()) {
        _pktgen_xmit(w, &g_pktgen_devinfo);
		if (need_resched()) {
            schedule();
        }

		try_to_freeze();
    }

    w->task = NULL;

    return 0;
}

static int _pktgen_parse_worker_num(int argc, char **argv)
{
    if (argc < 4) {
        return 1;
    }

    int worker_num = atoi(argv[3]);
    if (worker_num <= 0) {
        return 1;
    }

    if (worker_num > ARRAY_SIZE(g_pktgen_workers)) {
        return ARRAY_SIZE(g_pktgen_workers);
    }

    return worker_num;
}

static int _pktgen_start(int argc, char **argv, char *output_buf, int buf_size)
{
    char *dev_name = argv[2];
    int worker_num = _pktgen_parse_worker_num(argc, argv);

    struct net *inet = get_init_net();
    if (! inet) {
        snprintf(output_buf, buf_size, "Can't get init_net\n");
        return -1;
    }

    struct net_device * dev = dev_get_by_name(inet, dev_name);
    if (! dev) {
        snprintf(output_buf, buf_size, "Can't get dev\n");
        return -1;
    }

    klc_netdev_get_info(dev, &g_pktgen_devinfo);

    strlcpy(g_pktgen_dev_name, dev_name, sizeof(g_pktgen_dev_name));

    g_pktgen_worker_num = 0;

    for (int i=0; i<worker_num; i++) {
        PKTGEN_S *w = _pktgen_get_worker(i);
        char worker_name[12];
        snprintf(worker_name, sizeof(worker_name), "pktgen%u", i+1);
        w->dev = dev;
        w->woker_id = i;
        w->task = kthread_create(_pktgen_worker, w, worker_name);
        if (! w->task) {
            snprintf(output_buf, buf_size, "Can't create worker %s\n", worker_name);
            break;
        }
        g_pktgen_worker_num ++;
        wake_up_process(w->task);
    }

    return 0;
}

static int _pktgen_stop(void)
{
    int i;

    for (i=0; i<ARRAY_SIZE(g_pktgen_workers); i++) {
        PKTGEN_S *w = _pktgen_get_worker(i);
        void *task = w->task;
        if (task) {
            kthread_stop(task); 
        }
    }

    return 0;
}

static int _pktgen_set_dst_mac(int argc, char **argv, char *output_buf, int buf_size)
{
    char *dst_mac_str = argv[2];
    U8 mac[MAC_ADDR_LEN];

    if (ETH_String2Mac(dst_mac_str, mac) < 0) {
        snprintf(output_buf, buf_size, "Can't parse mac address\n");
        return -1;
    }

    memcpy(g_pktgen_dst_mac, mac, MAC_ADDR_LEN);

    return 0;
}


static int _pktgen_set_burst(int argc, char **argv, char *output_buf, int buf_size)
{
    char *brust_str = argv[2];
    int burst;

    burst = atoi(brust_str);
    if (! burst) {
        burst = PKTGEN_BURST_DFT;
    }

    g_pktgen_burst = burst;

    return 0;
}

static void _pktgen_show(OUT char *buf, int buf_size)
{
    char mac_string[ETH_MAC_ADDR_STRING_LEN + 1];

    snprintf(buf, buf_size,
            " Worker: %u\n"
            " Dev: %s\n"
            " DMac: %s\n"
            " Burst: %u\n",
            g_pktgen_worker_num,
            g_pktgen_dev_name,
            ETH_Mac2String(g_pktgen_dst_mac, ':', mac_string),
            g_pktgen_burst
            );
}

static int _pktgen_event_init(void)
{
    return 0;
}

static int _pktgen_event_fin1(void)
{
    _pktgen_stop();
    return 0;
}

SEC(".spf.cmd/cmd_main")
static int _pktgen_cmd_main(int argc, char **argv, char *output_buf, int buf_size)
{
    int ret = 0;

    if (argc < 2) {
        _pktgen_cmd_help(output_buf, buf_size);
        return -1;
    }

    if (strcmp(argv[1], "show")== 0) {
        _pktgen_show(output_buf, buf_size);
    } else if ((strcmp(argv[1], "start")== 0) && (argc >= 3)) {
        ret = _pktgen_start(argc, argv, output_buf, buf_size);
    } else if (strcmp(argv[1], "stop")== 0) {
        _pktgen_stop();
    } else if ((strcmp(argv[1], "dmac")== 0) && (argc >= 3)) {
        ret = _pktgen_set_dst_mac(argc, argv, output_buf, buf_size);
    } else if ((strcmp(argv[1], "burst")== 0) && (argc >= 3)) {
        ret = _pktgen_set_burst(argc, argv, output_buf, buf_size);
    } else {
        _pktgen_cmd_help(output_buf, buf_size);
        return -1;
    }

    return ret;
}

SEC(SPF_SEC_EVENT)
int event(U32 event)
{
    switch (event) {
        case SPF_EVENT_INIT: return _pktgen_event_init();
        case SPF_EVENT_FIN1: return _pktgen_event_fin1();
    }
    return 0;
}

SPF_MOD_COMMON_DEF(1)

