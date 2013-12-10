/**
 * @file udp_events.c
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
 * This file is part of udpip-broadcaster.
 * udpip-broadcaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * udpip-broadcaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with udpip-broadcaster.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "udp_events.h"

/* new_udp_events */
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
udp_events_t *new_udp_events()
{PRF("entrei en new_udp_events\n");
	udp_events_t *s = NULL;
	if ( ( s = (udp_events_t *)malloc(LEN__UDP_EVENTS) ) == NULL )
	{ handle_sys_error("new_udp_events: <malloc> returns NULL."); }
	if ( memset(s, 0, LEN__UDP_EVENTS) == NULL )
	{ handle_sys_error("new_udp_events: <memset> returns NULL."); }
	return(s);
}

/* init_tx_udp_events *//**
udp_events_t *init_tx_udp_events
(const char* if_name, const int port, in_addr_t addr, const ev_cb_t callback, const bool broadcast)
{
	udp_events_t *s = new_udp_events();
	if ( broadcast == true )
	{
		PRF(">>> Opening TX socket in broadcast mode.");
	s->socket_fd = open_broadcast_udp_socket(if_name, port);
	}	else	{
		PRF(">>> Opening TX socket in normal mode.");
	s->socket_fd = open_transmitter_udp_socket(port, addr);
	}
		if ( init_watcher(s, callback, EV_READ, port, if_name, addr) < 0 )	{ handle_app_error("init_tx_udp_events: <init_watcher> error.\n"); }
	ev_io_arg_t *arg = (ev_io_arg_t *)s->watcher;
	arg->public_arg.__test_number = 0;
	return(s);

}**/


/**udp_events_t *init_tx_raw_events
(const char* if_name, const int port, const ev_cb_t callback, const bool broadcast)
{
	udp_events_t *s = new_udp_events();
	if ( broadcast == true )
	{
		PRF(">>> Opening TX socket in broadcast mode.");
		s->socket_fd = open_broadcast_raw_socket(if_name, port);
	}	else	{
		PRF(">>> Opening TX socket in normal mode.");
		s->socket_fd = open_transmitter_raw_socket(port);
	}

	if ( init_watcher_raw(s, callback, EV_READ, port, if_name, (in_addr_t) NULL) < 0 )	{ handle_app_error("init_tx_udp_events: <init_watcher> error.\n"); }
	ev_io_arg_t *arg = (ev_io_arg_t *)s->watcher;
	arg->public_arg.__test_number = 0;
	return(s);

}**/

/* init_rx_udp_events */
udp_events_t *init_rx_udp_events(const int port, const char* if_name, const ev_cb_t callback)
{

	udp_events_t *s = new_udp_events();
	s->socket_fd = open_receiver_udp_socket(port);
	if ( init_watcher(s, callback, EV_READ, port, if_name,(in_addr_t) NULL) < 0 )	{ handle_app_error("init_rx_udp_events: <init_watcher> error.\n"); }
	return(s);

}

/* init_rx_udp_events */
udp_events_t *init_rx_raw_events(const int port, const char* if_name, const ev_cb_t callback)
{

	udp_events_t *s = new_udp_events();
	s->socket_fd = open_receiver_raw_socket(port);
	if ( init_watcher_raw(s, callback, EV_READ, port, if_name,(in_addr_t) NULL) < 0 )	{ handle_app_error("init_rx_udp_events: <init_watcher> error.\n"); }
	return(s);

}

/* init_net_udp_events */
udp_events_t *init_net_raw_events
(	const int net_tx_port,const int net_rx_port, const char* net_if_name,const char *app_fwd_addr, const int app_fwd_port,
		const bool nec_mode,	const ev_cb_t callback, List_locT *locT,List_lsp *lsp,List_lsp *rep,bool g)
{	udp_events_t *s = init_rx_raw_events(net_rx_port, net_if_name, callback);
	ev_io_arg_t *arg = (ev_io_arg_t *)s->watcher;
	arg->public_arg.local_addr= init_if_sockaddr_in(net_if_name, net_rx_port);
	arg->public_arg.forwarding_socket_fd= open_broadcast_udp_socket(net_if_name,app_fwd_port);
	arg->public_arg.net_socket_fd= open_broadcast_raw_socket(net_if_name, net_tx_port);
	arg->public_arg.forwarding_port =  app_fwd_port;
	arg->public_arg.forwarding_addr = init_sockaddr_in( app_fwd_port,app_fwd_addr);
	arg->public_arg.nec_mode = nec_mode;
	arg->public_arg.__test_number = 0;
	arg->public_arg.print_forwarding_message = __verbose;
	arg->cb_specfic	=callback;
	arg->public_arg.locT=locT;
	arg->public_arg.lsp=lsp;
	arg->public_arg.rep=rep;
	arg->public_arg.net_port=net_tx_port;
	arg->public_arg.gn=g;
	return(s);

}

/* init_app_udp_events */
void *init_app_udp_events
(	const int app_rx_port,in_addr_t addr,
		const char* if_name, const int net_fwd_port,
		const ev_cb_t callback, List_locT *locT,List_lsp *lsp,List_lsp *rep,bool g)
{	udp_events_t *s = init_rx_udp_events(app_rx_port, if_name, callback);
	ev_io_arg_t *arg = (ev_io_arg_t *)s->watcher;
	arg->public_arg.local_addr = init_if_sockaddr_ll(if_name, net_fwd_port);
	arg->public_arg.forwarding_socket_fd= open_broadcast_raw_socket(if_name, net_fwd_port);
	arg->public_arg.forwarding_addr= init_sockaddr_ll(net_fwd_port);
	arg->public_arg.forwarding_port = net_fwd_port;
	arg->public_arg.print_forwarding_message = __verbose;
	arg->public_arg.locT=locT;
	arg->public_arg.lsp=lsp;
	arg->public_arg.rep=rep;
	arg->public_arg.gn=g;
	//if (g) PRF("é true\n");else PRF("é false\n");
	public_ev_arg_r * argum= &arg->public_arg;

	return((void *)argum);

}

/* free_udp_events */
/**void free_udp_events(udp_events_t *m)
{
	free(m);
}**/

/* print_udp_events *//**
void print_udp_events
(const udp_events_t *m, const int rx_port, const int fwd_port)
{
	PRF("{\n");
	PRF("\t* rx_port = %d\n", rx_port);
	PRF("\t* fw_port = %d\n", fwd_port);
	PRF("\t* sock_fd = %d\n", m->socket_fd);
	PRF("}\n");
}**/

/* new_ev_io_arg_t */
ev_io_arg_t *new_ev_io_arg()
{PRF("entrei en new_ev_io_arg\n");
	ev_io_arg_t *s = NULL;
	if ( ( s = (ev_io_arg_t *)malloc(LEN__EV_IO_ARG) ) == NULL )
	{ handle_sys_error("new_ev_io_arg_t: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__EV_IO_ARG) == NULL )
	{ handle_sys_error("new_ev_io_arg_t: <memset> returns NULL.\n"); }
	return(s);
}

/* init_watcher */
int init_watcher(udp_events_t *m, const ev_cb_t callback, const int events, const int port, const char* if_name,in_addr_t addr)
{PRF("entrei en init_watcher\n");
	m->loop = EV_DEFAULT;
	ev_io_arg_t *arg = init_ev_io_arg(m, callback, port, if_name);
	arg->public_arg.local_addr = init_if_sockaddr_in(if_name, port);
	m->watcher = &arg->watcher;
	if (addr!=NULL) arg->public_arg.forwarding_addr=addr;
	ev_cb_t com_cb = NULL;
	com_cb = (ev_cb_t)&cb_common;
	ev_io_init(	m->watcher, com_cb,	m->socket_fd, EV_READ	);
	ev_io_start(m->loop, m->watcher);
	return(EX_OK);

}
int init_watcher_raw(udp_events_t *m, const ev_cb_t callback, const int events, const int port, const char* if_name,in_addr_t addr)
{PRF("entrei en init_watcher_raw\n");
	m->loop = EV_DEFAULT;
	ev_io_arg_t *arg = init_ev_io_arg(m, callback, port, if_name);
	arg->public_arg.local_addr = init_if_sockaddr_ll(if_name, port);
	m->watcher = &arg->watcher;
	if (addr!=NULL) arg->public_arg.forwarding_addr=addr;
	ev_cb_t com_cb = NULL;
	com_cb = (ev_cb_t)&cb_common;
	ev_io_init(	m->watcher, com_cb,	m->socket_fd, EV_READ	);
	ev_io_start(m->loop, m->watcher);
	return(EX_OK);

}

/* init_ev_io_arg */
ev_io_arg_t *init_ev_io_arg(const udp_events_t *m, const ev_cb_t callback, const int port, const char* if_name)
{PRF("entrei en init_ev_io_arg\n");
	ev_io_arg_t *s = new_ev_io_arg();

	// 1) private data initialization
	s->cb_specfic = callback;
	// 2) public data initialization
	if ( ( s->public_arg.data = malloc(UDP_BUFFER_LEN) ) == NULL )
	{ handle_sys_error("init_ev_io_arg: <malloc> returns NULL."); }
	//s->public_arg.local_addr = init_if_sockaddr_in(if_name, port);
	s->public_arg.len = 0;
	s->public_arg.socket_fd = m->socket_fd;
	s->public_arg.port = port;
	s->public_arg.msg_header= init_msg_header(s->public_arg.data, UDP_BUFFER_LEN);
	return(s);

}

/* cb_common */
void cb_common
(struct ev_loop *loop, struct ev_io *watcher, int revents)
{	if ( EV_ERROR & revents )
	{ log_sys_error("Invalid event"); return; }
	ev_io_arg_t *arg = (ev_io_arg_t *)watcher;
	public_ev_arg_r *public_arg = &arg->public_arg;
	public_arg->socket_fd = watcher->fd;
	if ( arg->cb_specfic == NULL )
	{
		PRF("cb_common (ev=%d,fd=%d): <cb_function> NULL!\n", revents, watcher->fd);
		return;
	}
	arg->cb_specfic(public_arg);
}
