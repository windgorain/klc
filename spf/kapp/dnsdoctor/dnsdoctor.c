/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-20
* Description: dnsdoctor: 对于指定域名, 将其解析出的IP地址替换成指定IP
******************************************************************************/
#include "utl/ulc_user.h"
#include "spf/klc/klc_sym.h"
#include "spf/spf_inet.h"
#include "bpf/udp_bpf.h"
#include "utl/dns_func.h"

#define DNSBLOCK_DOMAIN_SIZE 256
#define DNSBLOCK_RULE_MAX 1024

typedef struct {
    U64 match_count;
    U8 domain_len;
    char domain[DNSBLOCK_DOMAIN_SIZE];
}DNSBLOCK_RULE_S;

static char g_dnsdoctor_nf_ops[128];
static DNSBLOCK_RULE_S g_dnsdoctor_rules[DNSBLOCK_RULE_MAX];
static U32 g_dnsdoctor_rule_count = 0;
static U32 g_dnsdockor_target_ip = 0;

static int _dnsdoctor_find_rule(char *domain)
{
    char tmp[DNSBLOCK_DOMAIN_SIZE];

    int dns_len = DNS_DomainName2DnsName(domain, tmp, sizeof(tmp));
    if (dns_len < 0) {
        return -1;
    }

    for (int i=0; i<g_dnsdoctor_rule_count; i++) {
        if (strcmp(g_dnsdoctor_rules[i].domain, tmp) == 0) {
            return i;
        }
    }

    return -1;
}


static int _dnsdoctor_dns_name_len(void *dns_name, void *data_end)
{
    int data_len = data_end - dns_name;
    void *dns_name_end = memchr(dns_name, '\0', data_len);
    if (! dns_name_end) {
        return -1;
    }
    return dns_name_end - dns_name;
}

static BOOL_T _dnsdoctor_match_rule(void *dns_name, int dns_name_len)
{
    for (int i=0; i<g_dnsdoctor_rule_count; i++) {
        DNSBLOCK_RULE_S *r = &g_dnsdoctor_rules[i];
        if ((dns_name_len >= r->domain_len) && (strstr(dns_name, r->domain))) {
            r->match_count ++;
            return TRUE;
        }
    }

    return FALSE;
}

static void _dnsdoctor_nf_ipv4_pkt(void *skb)
{
    void *data = klc_skb_data(skb);
    int head_len = klc_skb_head_len(skb);
    void *data_end = data + head_len;

    IP_HEAD_S *iphdr = IPBPF_GetIPHeader(data, data_end);
    if (! iphdr) {
        return;
    }

    if (iphdr->ucProto != IP_PROTO_UDP) {
        return;
    }

    UDP_HEAD_S *udphdr = UDPBPF_GetUdpHeaderFromIP(iphdr, data_end);
    if (! udphdr) {
        return;
    }

    if (udphdr->usSrcPort != htons(53)) {
        return;
    }

    DNS_HEADER_S *dns_hdr = (void*)(udphdr + 1);
    void *dns_name = (dns_hdr + 1);
    if (dns_name >= data_end) {
        return;
    }

    int dns_name_len = _dnsdoctor_dns_name_len(dns_name, data_end);
    if (dns_name_len <= 0) {
        return;
    }

    if (! _dnsdoctor_match_rule(dns_name, dns_name_len)) {
        return;
    }

    
    void *dns_name_end = dns_name + dns_name_len + 1; 
    void *answers = dns_name_end + DNS_QST_LEN;

    U16 answers_count = ntohs(dns_hdr->usAnCount);

    
    for (int i=0; i<answers_count; i++) {
        DNS_RR_S *rr = answers + DNS_RR_NAME_LEN;
        U8 *address = (void*)(rr + 1);
        void *rr_end = address + ntohs(rr->usRDLen);
        if (rr_end > data_end) {
            break;
        }
        if (rr->usType == htons(DNS_TYPE_A)) {
            U8 *target_ip = (void*)&g_dnsdockor_target_ip;
            address[0] = target_ip[0]; address[1] = target_ip[1]; address[2] = target_ip[2]; address[3] = target_ip[3];
        }
        answers = rr_end;
    }

    return;
}

static unsigned int _dnsdoctor_nf_event(void *priv, void *skb, void *state)
{
    _dnsdoctor_nf_ipv4_pkt(skb);
    return NF_ACCEPT;
}

static int _dnsdoctor_init(void)
{
    int ret;
    KLC_PARAM_S p;
    void *inet_net;

    inet_net = get_init_net();
    if (! inet_net) {
        return -1;
    }

    p.count = 5;
    p.param[0] = (long)(void*)g_dnsdoctor_nf_ops;
    p.param[1] = (long)(void*)_dnsdoctor_nf_event;
    p.param[2] = NFPROTO_IPV4;
    p.param[3] = NF_INET_PRE_ROUTING;
    p.param[4] = -10L;

    ret = klc_init_nf_hook_ops(&p);
    if (ret < 0) {
        return ret;
    }

    return nf_register_net_hooks(inet_net, (void*)g_dnsdoctor_nf_ops, 1);
}

static int _dnsdoctor_fini(void)
{
    void *inet_net = get_init_net();
    if (inet_net) {
        nf_unregister_net_hooks(inet_net, (void*)g_dnsdoctor_nf_ops, 1);
    }

    return 0;
}

static void _dnsdoctor_cmd_help(char *buf, int buf_size)
{
    snprintf(buf, buf_size, "Usage:\n"
            " show rule\n"
            " show ip\n"
            " ip IP\n"
            " add rule DOMAIN\n"
            " del rule DOMAIN\n");
}

#define DNS_BLOCK_MORE_STR "MORE...\n"

static void _dnsdoctor_show_rule(OUT char *buf, int buf_size)
{
    int i;
    int len = 0;
    int size = buf_size - (sizeof(DNS_BLOCK_MORE_STR) - 1); 
    char tmp[DNSBLOCK_DOMAIN_SIZE];

    for (i=0; i<g_dnsdoctor_rule_count; i++) {
        DNSBLOCK_RULE_S *r = &g_dnsdoctor_rules[i];
        if (r->domain[0] == '\0') {
            break;
        }

        DNS_DnsName2DomainName(r->domain, tmp, sizeof(tmp));

        if (size <= (len + strlen(tmp) + 22)) { 
            snprintf(buf + len, buf_size - len, DNS_BLOCK_MORE_STR);
            break;
        }

        len += snprintf(buf + len, size - len, "%s:%llu\n", tmp, r->match_count);
    }
}


static int _dnsdoctor_target_ip(int argc, char **argv, char *buf, int buf_size)
{
    char *ip_str = argv[2];
    g_dnsdockor_target_ip = inet_addr(ip_str);
    return 0;
}

static void _dnsdoctor_show_target_ip(char *buf, int buf_size)
{
    char tmp[32];
    inet_ntop(AF_INET, &g_dnsdockor_target_ip, tmp, sizeof(tmp));
    snprintf(buf, buf_size, "%s\n", tmp);
}

static int _dnsdoctor_add_rule(int argc, char **argv, char *buf, int buf_size)
{
    char *domain = argv[3];
    char tmp[DNSBLOCK_DOMAIN_SIZE];

    if (g_dnsdoctor_rule_count >= ARRAY_SIZE(g_dnsdoctor_rules)) {
        snprintf(buf, buf_size, "Rule full\n");
        return -1;
    }

    int dns_len = DNS_DomainName2DnsName(domain, tmp, sizeof(tmp));
    if (dns_len < 0) {
        snprintf(buf, buf_size, "Add rule failed\n");
        return -1;
    }

    DNSBLOCK_RULE_S *r = &g_dnsdoctor_rules[g_dnsdoctor_rule_count];

    memcpy(r->domain, tmp, dns_len + 1);
    r->match_count = 0;
    r->domain_len = dns_len;

    g_dnsdoctor_rule_count ++;

    return 0;
}

static int _dnsdoctor_del_rule(int argc, char **argv, char *buf, int buf_size)
{
    char *domain = argv[3];
    int index = _dnsdoctor_find_rule(domain);

    if (index < 0) {
        snprintf(buf, buf_size, "the rule does not exist\n");
        return -1;
    }

    if (index + 1 < g_dnsdoctor_rule_count) {
        DNSBLOCK_RULE_S *r = &g_dnsdoctor_rules[index];
        DNSBLOCK_RULE_S *last = &g_dnsdoctor_rules[g_dnsdoctor_rule_count - 1];
        *r = *last;
    }

    g_dnsdoctor_rule_count --;

    return 0;
}

SEC(".spf.cmd/cmd_main")
static int _dnsdoctor_cmd_main(int argc, char **argv, char *output_buf, int buf_size)
{
    int ret = 0;

    if (argc < 3) {
        _dnsdoctor_cmd_help(output_buf, buf_size);
        return -1;
    }

    if ((strcmp(argv[1], "show")== 0) && (strcmp(argv[2], "rule") == 0)) {
        _dnsdoctor_show_rule(output_buf, buf_size);
    } else if ((strcmp(argv[1], "show")== 0) && (strcmp(argv[2], "ip") == 0)) {
        _dnsdoctor_show_target_ip(output_buf, buf_size);
    } else if (strcmp(argv[1], "ip")== 0) {
        ret = _dnsdoctor_target_ip(argc, argv, output_buf, buf_size);
    } else if ((strcmp(argv[1], "add")== 0) && (strcmp(argv[2], "rule") == 0) && (argc >= 4)) {
        ret = _dnsdoctor_add_rule(argc, argv, output_buf, buf_size);
    } else if ((strcmp(argv[1], "del")== 0) && (strcmp(argv[2], "rule") == 0) && (argc >= 4)) {
        ret = _dnsdoctor_del_rule(argc, argv, output_buf, buf_size);
    } else {
        _dnsdoctor_cmd_help(output_buf, buf_size);
        return -1;
    }

    return ret;
}

SEC(SPF_SEC_EVENT)
int _nf_event(U32 event)
{
    switch (event) {
        case SPF_EVENT_INIT: return _dnsdoctor_init();
        case SPF_EVENT_FIN1: return _dnsdoctor_fini();
    }

    return 0;
}

