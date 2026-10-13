/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-10-12
* Description: 
******************************************************************************/
#ifndef _KLC_SYM_SOCK_H_
#define _KLC_SYM_SOCK_H_

#ifdef __cplusplus
extern "C" {
#endif


#define ITER_SOURCE	1	
#define ITER_DEST	0	

struct socket;
struct sock;
struct sockaddr;
struct msghdr;
struct iov_iter;


static inline struct sock * klc_get_sock_by_socket(struct socket *res)
{
    return (void*)(long)ulc_call_sym(0, klc_get_sock_by_socket, res);
}


static inline void sk_net_refcnt_upgrade(struct sock *sk)
{
    ulc_call_sym(0, sk_net_refcnt_upgrade, sk);
}

static inline int kernel_sock_shutdown(struct socket *sock, int how)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)how) << 32);
        return ulc_call_sym(-1, kernel_sock_shutdown, p1);
    } else {
        return ulc_call_sym(-1, kernel_sock_shutdown, sock, how);
    }
}

static inline void sock_release(struct socket *sock)
{
    ulc_call_sym(0, sock_release, sock);
}


static inline int sock_create_kern(struct net *net, int family, int type, int protocol, struct socket **res)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)net; p1 |= (((U64)(U32)family) << 32);
        U64 p2 = type; p2 |= (((U64)(U32)protocol) << 32);
        return ulc_call_sym(0, sock_create_kern, p1, p2, res);
    } else {
        return ulc_call_sym(0, sock_create_kern, net, family, type, protocol, res);
    }
}

static inline int sock_create(int family, int type, int protocol, struct socket **accept_sock)
{
    if (klc_is_sys32()) {
        U64 p1 = (U32)family; p1 |= (((U64)(U32)type) << 32);
        U64 p2 = (U32)protocol; p2 |= (((U64)(U32)(long)accept_sock) << 32);
        return ulc_call_sym(-1, sock_create, p1, p2);
    } else {
        return ulc_call_sym(-1, sock_create, family, type, protocol, accept_sock);
    }
}


static inline void * kernel_socket(int af, int type, int protocol)
{
    struct socket *sock;

    struct net * netns = klc_get_current_netns();
    if (! netns) {
        return (void*)-1;
    }

    if (sock_create_kern(netns, af, type, protocol, &sock) < 0) {
        return (void*)-1;
    }

    struct sock *s = klc_get_sock_by_socket(sock);
    if (! s) {
        kernel_sock_shutdown(sock, 2);
        sock_release(sock);
        return (void*)-1;
    }

    sk_net_refcnt_upgrade(s);

    return sock;
}

static inline int kernel_bind(struct socket *sock, struct sockaddr *addr, int addrlen)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((long)addr) << 32);
        return ulc_call_sym(-1, kernel_bind, p1, addrlen);
    } else {
        return ulc_call_sym(-1, kernel_bind, sock, addr, addrlen);
    }
}

static inline int kernel_connect(struct socket *sock, struct sockaddr *addr, int addrlen, int flags)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((long)addr) << 32);
        U64 p2 = (U32)addrlen; p2 |= (((U64)(U32)flags) << 32);
        return ulc_call_sym(-1, kernel_connect, p1, p2);
    } else {
        return ulc_call_sym(-1, kernel_connect, sock, addr, addrlen, flags);
    }
}

static inline int kernel_listen(struct socket *sock, int backlog)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)backlog) << 32);
        return ulc_call_sym(-1, kernel_listen, p1);
    } else {
        return ulc_call_sym(-1, kernel_listen, sock, backlog);
    }
}


static inline int kernel_accept(struct socket *sock, struct socket **newsock, int flags)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(long)newsock) << 32);
        return ulc_call_sym(-1, kernel_accept, p1, flags);
    } else {
        return ulc_call_sym(-1, kernel_accept, sock, newsock, flags);
    }
}

static inline int sock_recvmsg(struct socket *sock, struct msghdr *msg, int flags)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)(long)msg) << 32);
        return ulc_call_sym(-1, sock_recvmsg, p1, flags);
    } else {
        return ulc_call_sym(-1, sock_recvmsg, sock, msg, flags);
    }
}

static inline int klc_socket_recv(struct socket *sock, void *buf, int buf_len, int flags)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)(long)buf) << 32);
        U64 p2 = (U32)buf_len; p2 |= (((U64)(U32)flags) << 32);
        return ulc_call_sym(-1, klc_socket_recv, p1, p2);
    } else {
        return ulc_call_sym(-1, klc_socket_recv, sock, buf, buf_len ,flags);
    }
}

static inline int klc_socket_send(struct socket *sock, void *buf, int buf_len, int flags)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)(long)buf) << 32);
        U64 p2 = (U32)buf_len; p2 |= (((U64)(U32)flags) << 32);
        return ulc_call_sym(-1, klc_socket_send, p1, p2);
    } else {
        return ulc_call_sym(-1, klc_socket_send, sock, buf, buf_len, flags);
    }
}

static inline int klc_sock_setsockopt(struct socket *sock, int level, int opt, void *optval, int optlen)
{
    if (klc_is_sys32()) {
        U64 p1 = (long)sock; p1 |= (((U64)(U32)level) << 32);
        U64 p2 = (U32)opt; p2 |= (((U64)(U32)(long)optval) << 32);
        return ulc_call_sym(-1, klc_sock_setsockopt, p1, p2, optlen);
    } else {
        return ulc_call_sym(-1, klc_sock_setsockopt, sock, level, opt, optval, optlen);
    }
}

#ifdef __cplusplus
}
#endif
#endif 
