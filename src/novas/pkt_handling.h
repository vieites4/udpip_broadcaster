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
/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */


//void GeoUnicast(int option);
void TSB(public_ev_arg_t *arg);

void SHB();

void GeoBroadcast();
void GeoAnycast();
void CommonHeader_processing();
void determine_nexthop();
void GeoUnicast();




#endif /* PKT_HANDLING_H_ */
