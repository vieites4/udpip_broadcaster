/**
 * @file udp_socket.h
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

#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../logger.h"
#include "../execution_codes.h"
#include <stdio.h>
#include <unistd.h>
#include <linux/if_packet.h>
//#include <net/ethernet.h> /* the L2 protocols */
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <ev.h>
#include "../novas/netmanagement.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// SOCKET STRUCTURES
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define LEN__IEEE80211_FRAME sizeof(ieee80211_frame_t)
#define IEEE_80211_HLEN 		30		/*!< IEEE 802.11 header length (B). */
#define TYPE_IEEE_80211 	2	/*!< Buffer with an IEEE 802.11 frame. */


#define typo[2] =                          {0x08,0x00}


typedef struct ieee80211_header_frame_control
{

	uint8_t mac_service;				/*!< IEEE 802.11 MAC service (1 B). */
	uint8_t flags;						/*!< IEEE 802.11 flags (1 B). */

} ieee80211_header_frame_control_t;



typedef struct ifreq ifreq_t;				/**< Type for ifreq. */
#define LEN__IFREQ sizeof(ifreq_t)
typedef struct sockaddr_ll sockaddr_ll_t;
#define LEN__SOCKADDR_LL sizeof(sockaddr_ll_t)
/**
 * @fn new_ifreq
 * @brief Allocates memory for an ifreq structure.
 * @return A pointer to the newly allocated block of memory.
 */
ifreq_t *new_ifreq();

/**
 * \fn init_ifreq
 * \brief Allocates memory for an ifreq structure and initializes it with the
 * 			given interface name.
 * \return A pointer to the initialized ifreq structure.
 */
ifreq_t *init_ifreq(const char *if_name);


#define LEN__SOCKADDR sizeof(sockaddr_t)

/**
 * @fn new_sockaddr
 * @brief Allocates memory for an sockaddr structure.
 * @return A pointer to the newly allocated block of memory.
 */
//sockaddr_t *new_sockaddr();


#define LEN__SOCKADDR_IN sizeof(sockaddr_in_t)
#define LEN__SOCKADDR_LL sizeof(sockaddr_ll_t)
/**
 * @fn new_sockaddr_in
 * @brief Allocates memory for an sockaddr_in structure.
 * @return A pointer to the newly allocated block of memory.
 */
sockaddr_in_t *new_sockaddr_in();
sockaddr_ll_t *new_sockaddr_ll();

typedef struct iovec iovec_t;
#define LEN__IOVEC sizeof(iovec_t)

/**
 * @fn new_iovec
 * @brief Allocates memory for an iovec structure.
 * @return A pointer to the newly allocated block of memory.
 */
iovec_t *new_iovec();


#define LEN__MSG_HEADER sizeof(msg_header_t)

#define CONTROL_BUFFER_LEN 0x100	/**< Length of the control buffer. */

/**
 * @fn msg_header_t
 * @brief Allocates memory for an msg_header structure.
 * @return A pointer to the newly allocated block of memory.
 */
msg_header_t *new_msg_header();

/**
 * @fn init_msg_header
 * @brief Initializes a message header structure that uses the given buffer
 * 			for package reception/transmission.
 * @param buffer Buffer where to store received/transmitted bytes.
 * @param buffer_len Maximum length of the given buffer.
 * @return An initialized msg_header structure.
 */
msg_header_t *init_msg_header(void* buffer, const int buffer_len);

/**
 * @fn init_broadcast_sockaddr_in
 * @brief Initializes a sockaddr_in structure for broadcasting messages in
 * 			the given port.
 * @param port The port to use for broadcasting.
 * @result An initialized sockaddr_in structure.
 */
sockaddr_in_t *init_broadcast_sockaddr_in(const int port);
sockaddr_ll_t *init_broadcast_sockaddr_ll(const int port);
/**
 * @fn init_any_sockaddr_in
 * @brief Initializes a sockaddr_in structure for receiving messages from
 * 			any source address, which were sent to the given port.
 * @param port The port where the messages were sent to.
 * @result An initialized sockaddr_in structure.
 */
sockaddr_in_t *init_any_sockaddr_in(const int port);
sockaddr_ll_t *init_any_sockaddr_ll(const int port);

/**
 * @fn init_sockaddr_in
 * @brief Initializes a sockaddr_in structure for sending messages to the
 * 			given source address and port.
 * @param address The address where the messages will be sent to.
 * @param port The port where the messages will be sent to.
 * @result An initialized sockaddr_in structure.
 */
sockaddr_in_t *init_sockaddr_in( const int port, in_addr_t addr);
sockaddr_ll_t *init_sockaddr_ll(const int port);

/**
 * @fn init_if_sockaddr_in
 * @brief Initializes a sockaddr_in structure for sending messages through
 * 			the given interface and port.
 * @param if_name The interface through which UDP messages will be sent to.
 * @param port The port where the messages will be sent to.
 * @result An initialized sockaddr_in structure.
 */
sockaddr_in_t *init_if_sockaddr_in(const char *if_name, const int port);
sockaddr_ll_t *init_if_sockaddr_ll(const char *if_name, const int port);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// UDP SOCKET MANAGEMENT
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/**
 * @fn set_broadcast_socket
 * @brief Sets socket options for permitting this socket to send broadcast
 * 			messages.
 * @param socket_fd File descriptor of the socket.
 * @return 'EX_OK' in case everything went allright; otherwise, < 0.
 */
int set_broadcast_socket(const int socket_fd);

/**
 * @fn set_bindtodevice_socket
 * @brief Sets socket options for binding this socket to the given interface.
 * @param iface Interface where the socket is expected to be bound.
 * @param socket_fd File descriptor of the socket.
 */
int set_bindtodevice_socket(const char *if_name, const int socket_fd);

int set_msghdrs_socket(const int socket_fd);
//int set_msghdrs_socket_raw(const int socket_fd);
/**
 * @fn open_receiver_udp_socket
 * @brief Creates and binds an UDP socket that uses the given port.
 * @param port The UDP port to be used by this socket.
 * @return File descriptor of the opened UDP socket.
 */
int open_receiver_udp_socket(const int port);
int open_receiver_raw_socket(const int port);

/**
 * @fn open_transmitter_udp_socket
 * @brief Creates and DOES NOT BIND an UDP socket that uses the given port.
 * @param port The UDP port to be used by this socket.
 * @return File descriptor of the opened UDP socket.
 */
//int open_transmitter_udp_socket(const int port, in_addr_t addr);
//int open_transmitter_raw_socket(const int port);
/**
 * @fn open_broadcast_udp_socket
 * @brief Creates and binds an UDP socket that uses the given port with
 * 			broadcasting privileges.
 * @param iface Interface where the socket is expected to be bound.
 * @param port The UDP port to be used by this socket.
 * @return File descriptor of the opened UDP socket.
 */
int open_broadcast_udp_socket(const char *if_name, const int port);
int open_broadcast_raw_socket(const char *if_name, const int port);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// SOCKET DATA TRANSMISSION
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define UDP_BUFFER_LEN 5000 	/**< Size of the RX/TX buffers. */

/**
 * @fn recv_message
 * @brief Sends a message to the given destination address.
 * @param dest_addr The destination address where the message will be sent to.
 * @param socket_fd File descriptor of the socket to be used.
 * @param buffer Pointer to the buffer where the message is stored.
 * @param len Length of the message to be sent.
 */
int send_message(	const sockaddr_t* dest_addr, const int socket_fd,
					const void *buffer, const int len	);

/**
 * @fn recv_message
 * @brief Receives a message from the given socket..
 * @param socket_fd File descriptor of the socket to be used.
 * @param data Buffer for storing the message.
 * @return The number of bytes read and stored in the message. If < 0, it
 * 			indicates that an error has occurred and, therefore, data value
 * 			shall not be taken into consideration.
 */
int recv_message(const int socket_fd, void *data);

/**
 * @fn recv_msg
 * @brief Receives an UDP message from the given socket and marks whether
 * 			the message should be blocked or not because its source address
 * 			matches that of the given block_ip address.
 * @param socket_fd File descriptor of the socket to be used.
 * @param msg Structure holding both message's data and all related reception
 * 				headers.
 * @param block_ip IPv4 address whose messages are to flagged as "blocked".
 * @param blocked Block flag that indicates that a message is to be blocked.
 * @return The number of bytes read and stored in the message. If < 0, it
 * 			indicates that an error has occurred and, therefore, data and
 * 			blocked flag values shall not be taken into consideration.
 */
int recv_msg(	const int socket_fd, msg_header_t *msg,
				const in_addr_t block_ip, bool *blocked	);

/**
 * @fn get_source_address
 * Gets the source address of the given message, by iterating along all
 * headers included in the message structure.
 *
 * @param msg Structure containing the received message and all associated
 * 				headers with associated information.
 * @return Source address of the given message.
 */
in_addr_t get_source_address(msg_header_t *msg);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// COMMON SOCKET TOOLS
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define BYTES_PER_LINE 5000	/**< Number of bytes per line to be printed. */

/**
 * @fn print_hex_data
 * @brief Prints hexadecimal data.
 * @param hexadecimal data as an array.
 * @param data length in bytes
 */
int print_hex_data(const char *buffer, const int len);

/**
 * @fn sprint_hex_data
 * @brief convert in integer hexadecimal data .
 * @param hexadecimal data as an array.
 * @param data length in bytes
 */
int sprint_hex_data(const char *buffer, const int len);
int set_ll_frame
	(ll_frame_t *frame, const int frame_type, const int frame_len);

/**
 * @fn init_ieee80211_frame
 * @brief initialization of ieee80211 frame .
 * @param socket where we will sent the frame
 * @param mac address destination (broadcast?)
 * @param mac address source
 */
ieee80211_frame_t *new_ieee80211_frame();

ieee80211_frame_t *init_ieee80211_frame	(	const int ll_sap, const unsigned char *h_dest, const unsigned char *h_source	);
/**
 * @fn get_mac_address
 * @brief obtain our home mac address .
 * @param socket number
 * @param interface name for this socket
 * @param place where the mac address is returned
 */
int get_mac_address	(const int socket_fd, const char *if_name, const unsigned char *mac);
#endif /* UDP_SOCKET_H_ */
