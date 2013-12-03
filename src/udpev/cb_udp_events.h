/**
 * @file cb_udp_events.h
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
#ifndef CB_UDP_EVENTS_H_
#define CB_UDP_EVENTS_H_
#include <sys/socket.h>
#include "../novas/itsnet_header.h"
#include <stdio.h>
#include <time.h>
#include "__NEC__gnbtpapi_udp_msg.h"
#include "udp_events.h"
#include "define.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// LIBEV CALLBACK FUNCTIONS
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
 * @fn cb_print_recvfrom
 * @brief Callback function that reads the given socket which has just been
 * 			put available for reading.
 * @param public_arg Public arguments for this callback function.
 *
 */
void cb_print_recvfrom(public_ev_arg_r *arg);

/**
 * @fn cb_forward_recvfrom
 * @brief Callback function that takes information arrived from link layer.
 * It will be sent to the upper layer and in some cases also resent to link layer
 *  in broadcast mode.
 * @param public_arg Public arguments for this callback function.
 */
void cb_forward_recvfrom(public_ev_arg_r *arg);

/**
 * @fn cb_broadcast_recvfrom
 * @brief Callback function that forwards an UDP message that was received from upper layer.
 *  The forwarded message is sent through broadcast to link layer.
 * @param public_arg Public arguments for this callback function.
 */
void cb_broadcast_recvfrom(public_ev_arg_r *arg);
/**
 * @fn thr_h3
 * @brief Thread we use to timer Beacon send. Beacon is how we announce our mac.
 * If we sent another frame the timer will be updated
 * @param public_arg Public arguments for this callback function.
 */

void *thr_h3(void *arg);

#endif /* CB_UDP_EVENTS_H_ */
