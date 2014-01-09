#ifndef FORW_ALGORITHM_H
#define FORW_ALGORITHM_H
#include "netmanagement.h"
#include <stdint.h>
#include "../udpev/define.h"

/**
 * @fn Greedy_Forwarding_UC
 * @brief This is one of the forwarding algorithm for geounicast packets
 * @param lpv, location position vector to destination
 * @result mac direction of forwarding
 */

mac_addr Greedy_Forwarding_UC(itsnet_position_vector *lpv_p);

/**
 * @fn Distance
 * @brief This is function calculates the distance between this lpvs
 * @param lpv, location position vector to destination
 * @result distance between lpvs
 */


int Distance(itsnet_position_vector *lpv_a,itsnet_position_vector *lpv_b);

//send_bcast_cbf_uc(gTimer_cbf_uc[i]);

/**
 * @fn CBF_UC
 * @brief This is one of the forwarding algorithm for geounicast packets
 * @param pkt, this is the packet we must forward
 * @param len, len of pkt
 * @param lpv_se, location position vector of source
 * @result 0,-1: (0)the packet if buffered in lsp_cbf_uc, (-1) the packet is discarded
 */

int CBF_UC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se,int any);

/**
 * @fn CBF_BC
 * @brief This is one of the forwarding algorithm for geobroadcast packets
 * @param pkt, this is the packet we must forward
 * @param len, len of pkt
 * @param lpv_se, location position vector of source
 * @result mac_addr, 0,-1: (0)the packet if buffered in lsp_cbf_uc (ZEROS), (-1) the packet is discarded (TWOS)
 */

mac_addr CBF_BC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se);

mac_addr Advanced_BC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se,mac_addr *PKT_mac,char h_source[ETH_ALEN]);
int G(itsnet_position_vector * lpv_se,itsnet_position_vector * lpv_f);
/**
 * @fn geo_limit
 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
 * @param HT, to now if it's circular(0), rectangular(1) or elliptical(2).
 * @param pkt,packet we will send to the upper layer if we are into the desired area
 * @return if it is <0 it is out of area.
 */
int geo_limit(void *HT,itsnet_packet_f *pkt,itsnet_position_vector * lpv_se);


/**
 * @fn geo_limit_ll
 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
 * @param pkt, packet we will send to link layer if we are into the desired area
 * @return if it is <0 it is out of area.
 */


int geo_limit_ll(itsnet_packet *pkt,itsnet_position_vector * lpv_se);
#endif
