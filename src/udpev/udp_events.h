/**
 * @file udp_events.h
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

#ifndef UDPEV_MANAGER_H_
#define UDPEV_MANAGER_H_
#include <sys/socket.h>
#include <errno.h>
#include <ev.h>
#include <unistd.h>
//#include "cb_udp_events.h"
#include "../configuration.h"
//#include "../execution_codes.h"
#include "../novas/netmanagement.h"

#include "udp_socket.h"
#include "cb_udp_events.h"
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// DATA STRUCTURES
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define LEN__EV_IO 		sizeof(struct ev_io)
typedef void (*ev_cb_t)(public_ev_arg_r *);
/**
 * @struct public_ev_arg
 * @brief Structure for holding public arguments to be passed to callback
 * 			functions.
 */
//typedef struct public_ev_arg public_ev_arg_r;
/*!< Callback function. */

#define LEN__PUBLIC_EV_ARG sizeof(public_ev_arg_t)


typedef void (*ev_cb_tr)(public_ev_arg_r *);

/**
 * @struct ev_io_arg
 * @brief Structure for passing some arguments to libev's io callback.
 */
typedef struct ev_io_arg
{

	struct ev_io watcher;					/**< Event watcher. */
	ev_cb_tr cb_specfic;						/**< Callback function. */
	void (*cb_function) (public_ev_arg_r *arg);	/**< Watcher callback. */
	public_ev_arg_r public_arg;		/*!< Data for external callbacks. */

} ev_io_arg_t;

#define LEN__EV_IO_ARG sizeof(ev_io_arg_t)

/**
 * @struct udp_events
 * @brief Structure that holds the configuration of the UDP events manager.
 */
typedef struct udp_events
{

	int socket_fd;			/**< FD of the UDP socket. */

	ev_cb_tr cb_event;		/**< Callback frame rx function. */

	struct ev_loop *loop;	/**< Default event loop. */
	struct ev_io *watcher;	/**< Event watcher. */

} udp_events_t;

#define LEN__UDP_EVENTS sizeof(udp_events_t)

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// UDP EVENTS SOCKET
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/**
 * @brief Creates a new structure for handling events with this manager.
 * @return The structure that holds the current state of the manager.
 */

udp_events_t *new_udp_events();

/**
 * @brief Initializes a new structure for handling libev's transmissio
 * 			events for an UDP socket to transmit broadcast messages using the
 * 			given port.
 * @param port Port to which this socket will be bound to.
 * @param tx Flag that indicates whether this socket is for data transmission
 * 				('true') or for data reception ('false').
 * @param broadcast Flag that indicates whether this socket if to transmit
 * 					UDP messages to all hosts in the network or not.
 * @return Manager's structure configured.
 */
udp_events_t *init_tx_udp_events
(const char* if_name, const int port, in_addr_t addr
		, const ev_cb_tr callback, const bool broadcast);
udp_events_t *init_tx_raw_events
(const char* if_name, const int port
		, const ev_cb_tr callback, const bool broadcast);

/**
 * @brief Initializes a new structure for handling libev's transmission
 * 			events for an UDP socket to transmit broadcast messages using the
 * 			given port.
 * @param port Port to which this socket will be bound to.
 * @param tx Flag that indicates whether this socket is for data transmission
 * 				('true') or for data reception ('false').
 * @param callback Callback function that will process the data message
 * 					reception events.
 * @return Manager's structure configured.
 */
udp_events_t *init_rx_udp_events(const int port, const char* if_name
		, const ev_cb_tr callback);
udp_events_t *init_rx_raw_events(const int port, const char* if_name
		, const ev_cb_tr callback);
/**
 * @brief Initializes a new structure for handling libev's reception events
 * 			for an UDP socket which will trigger the immediate forwarding
 * 			of those messages to the app_tx_port.
 * @param net_rx_port UDP port where messages are to be received from the
 * 						network.
 * @param app_fwd_addr Network address of the application.
 * @param app_fwd_port UDP port where messages received from the network
 * 						are forwarded to applications.
 * @return Structure for management just configured.
 */
udp_events_t *init_net_raw_events
(	const int net_tx_port,const int net_rx_port, const char* net_if_name,const char *app_fwd_addr, const int app_fwd_port,
		const bool nec_mode,	const ev_cb_tr callback, List_locT *locT, List_lsp *lsp, List_lsp *rep)	;

void *init_app_udp_events
(	const int app_rx_port,in_addr_t addr,
		const char* if_name, const int net_fwd_port,
		const ev_cb_tr callback, List_locT *locT, List_lsp *lsp, List_lsp *rep);

/**
 * @brief Releases all resources that previously were allocated for this
 * 			manager.
 * @param m Pointer to the manager structure.
 */
void free_udp_events(udp_events_t *m);

/**
 * @brief Prints the data of the udp_events structure.
 * @param m The structure whose data is going to be printed out.
 */
void print_udp_events
(const udp_events_t *m, const int rx_port, const int fwd_port);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// LIBEV EVENTS MANAGEMENT
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/**
 * @brief Allocates memory for an ev_io_arg structure.
 * @return A pointer to the newly allocated block of memory.
 */
ev_io_arg_t *new_ev_io_arg();

/**
 * @brief Initializes an ev_io_arg structure for transmission events.
 * @param m Structure with data about UDP events initialization.
 * @param callback Callback function for the registered events.
 * @param port Port for the UDP connection.
 * @param if_name Name of the interface.
 * @return A pointer to the initialized structure.
 */
ev_io_arg_t *init_ev_io_arg(const udp_events_t *m
		, const ev_cb_tr callback
		, const int port
		, const char* if_name);

/**
 * @brief Initializes the callback functions for the events given as the last
 * 			parameter.
 * @param m Structure with the information of the socket.
 * @param callback Callback function for the RX events.
 * @param events Flags with the events that will be processed by the given
 * 					callback function.
 * @param port Port for the UDP connection.
 * @param if_name Name of the interface.
 */
int init_watcher(udp_events_t *m
		, const ev_cb_tr callback, const int events
		, const int port, const char* if_name, in_addr_t addr);
int init_watcher_raw(udp_events_t *m
		, const ev_cb_tr callback, const int events
		, const int port, const char* if_name, in_addr_t addr);
/**
 * @brief Callback function for common events processing, <libev>. All events
 * 			that are generated by <libev> call first to this function and,
 * 			afterwards, to the specific callback function given during the
 * 			event's processor registering process.
 */
void cb_common
(struct ev_loop *loop, struct ev_io *watcher, int revents);

#endif /* UDPEV_MANAGER_H_ */
