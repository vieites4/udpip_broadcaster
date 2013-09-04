/*
 * pkt_handling.h
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */

#ifndef PKT_HANDLING_H_
#define PKT_HANDLING_H_




/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */


void GeoUnicast(int option);
void TSB();

void SHB();

void GeoBroadcast();
void GeoAnycast();
void CommonHeader_processing();
void determine_nexthop();





#endif /* PKT_HANDLING_H_ */
