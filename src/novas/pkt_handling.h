/*
 * pkt_handling.h
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */

#ifndef PKT_HANDLING_H_
#define PKT_HANDLING_H_


#include "udpev/udp_events.h"
#include "netmanagement.h"
#include "itsnet_header.h"

itsnet_packet * TSB(void *data);

itsnet_packet * SHB(void *dato);

itsnet_packet * GeoBroadcast(void *dato);
void GeoAnycast();
void CommonHeader_processing(public_ev_arg_t *arg);
void determine_nexthop();
void GeoUnicast();



#endif /* PKT_HANDLING_H_ */
