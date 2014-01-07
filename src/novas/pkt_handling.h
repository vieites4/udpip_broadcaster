/*
 * pkt_handling.h
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */

#ifndef PKT_HANDLING_H_
#define PKT_HANDLING_H_
#include "netmanagement.h"
/**
 * @fn TSB
 * @brief This function create the tsb packet to send through link layer.
 * @param data, we use  to make the packet
 * @param list where we save it if we don't have neighbours
 * @param list where we save it in case of repetition
 * @return the packet. if it is null the packet was discarded or saved in the buffer
 */
itsnet_packet * TSB(void *data,List_lsp *lsp,List_lsp *rep);
/**
 * @fn SHB
 * @brief This function create the shb packet to send through link layer.
 * @param data, we use  to make the packet
 * @param list where we save it if we don't have neighbours
 * @param list where we save it in case of repetition
 * @return the packet. if it is null the packet was discarded or saved in the buffer
 */

itsnet_packet * SHB(void *dato,List_lsp *lsp,List_lsp *rep);
/**
 *  @fn GeoBroadcast
 * @brief This function create the geobroadcast packet to send through link layer.
 * @param data, we use  to make the packet
 * @param list where we save it if we don't have neighbours
 * @param list where we save it in case of repetition
 * @return the packet. if it is null the packet was discarded or saved in the buffer
 */

itsnet_packet * GeoBroadcast(void *dato,List_lsp *lsp,List_lsp *rep,public_ev_arg_r *arg);

/**
 *  @fn TSB_f
 * @brief This function create the tsb packet to send to upper layer.
 * @param data, we use  to make the packet
 * @return the packet.
 */


/**
 *  @fn GeoUnicast
 * @brief This function create the geounicast packet to send through link layer.
 * @param data, we use  to make the packet
 * @param list where we save it if we don't have neighbours
 * @param list where we save it in case of repetition
 * @return the packet. if it is null the packet was discarded or saved in the buffer
 */

itsnet_packet * GeoUnicast(void *dato,List_lsp *lsp,List_lsp *rep,public_ev_arg_r *arg);


itsnet_packet_f * TSB_f(void *data);
/**
 * @fn SHB_f
 * @brief This function create the shb packet to send to upper layer.
 * @param data, we use  to make the packet
 * @param g, say if we have to adapt to v1.1.1 format convention
 * @return the packet.
 */
itsnet_packet_f * SHB_f(void *dato);
/**
 *  @fn GeoBroadcast_f
 * @brief This function create the geobroadcast packet to send to upper layer.
 * @param data, we use  to make the packet
 * @param g, say if we have to adapt to v1.1.1 format convention
 * @return the packet.
 */
itsnet_packet_f * GeoBroadcast_f(void *dato);
/**
 *  @fn GeoUnicast_f
 * @brief This function create the GeoUnicast packet to send to upper layer.
 * @param data, we use  to make the packet
 * @return the packet.
 */
itsnet_packet_f * GeoUnicast_f(void *dato);
/**
 *  @fn GeoAnycast_f
 * @brief This function create the GeoAnycast packet to send to upper layer.
 * @param data, we use  to make the packet
 * @return the packet.
 */

itsnet_packet_f * GeoAnycast_f(void *dato);
//itsnet_packet_f * GEOUN_f(void *dato);


itsnet_packet * LS_req_f(void *buffer);

/**
 *  @fn BasicHeader_processing
 * @brief This function makes the basic header processing of packets arrived from link layer.
 * @param arg
 * @return 0 if everything is correct.
 */

int BasicHeader_processing(public_ev_arg_r *arg);

/**
 *  @fn CommonHeader_processing
 * @brief This function makes the common header processing of packets arrived from link layer.
 * @param arg
 * @return 0 if everything is correct.
 */

int CommonHeader_processing(public_ev_arg_r *arg);

/**
 * @fn geo_limit
 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
 * @param HT, to now if it's circular(0), rectangular(1) or elliptical(2).
 * @param pkt,packet we will send to the upper layer if we are into the desired area
 * @return if it is <0 it is out of area.
 */
int geo_limit(void *HT,itsnet_packet_f *pkt);


/**
 * @fn geo_limit_ll
 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
 * @param pkt, packet we will send to link layer if we are into the desired area
 * @return if it is <0 it is out of area.
 */


int geo_limit_ll(itsnet_packet *pkt);

/**
 * @fn GeoAnycast
 * @brief GeoAnycast no implemented yet
 */
itsnet_packet * GeoAnycast(void *data,List_lsp *lsp,List_lsp *rep);
/**
 * @fn determine_nexthop
 * @brief determine_nexthop no implemented yet
 */
void determine_nexthop();
/**
 * @fn GeoUnicast
 * @brief GeoUnicast no implemented yet
 */


#endif /* PKT_HANDLING_H_ */
