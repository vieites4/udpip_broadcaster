/*
 * pkt_handling.h
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */

#ifndef PKT_HANDLING_H_
#define PKT_HANDLING_H_


//#include "udpev/udp_events.h"
#include "netmanagement.h"

itsnet_packet * TSB(void *data);

itsnet_packet * SHB(void *dato);

itsnet_packet * GeoBroadcast(void *dato);
itsnet_packet_f * TSB_f(void *data);

itsnet_packet_f * SHB_f(void *dato);

itsnet_packet_f * GeoBroadcast_f(void *dato);
void GeoAnycast();
void CommonHeader_processing(public_ev_arg_r *arg);
void determine_nexthop();
void GeoUnicast();



#endif /* PKT_HANDLING_H_ */
