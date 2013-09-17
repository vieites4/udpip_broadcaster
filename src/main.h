/**
 * @file main.h
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
 *
 * @section DESCRIPTION
 *
 * Header file with main dependencies for this application and all of its
 * components.
 */

#ifndef MAIN_H_
#define MAIN_H_


#define itsGnLocalGnAddr 1
#define itsGnLocalAddrConfMethod  0//Managed(1) Auto (0)
#define itsGnProtocolVersion 0
#define itsGnStationType 0 //0 vehicle 1 roadside
#define itsGnMinimunUpdateFrequencyLPV 1000//0 roadside
#define itsGnMaxSduSize 1398
#define itsGnMaxGeoNetworkingHeaderSize 88
#define itsGnLifetimeLocTE 20
#define itsGnLocationServiceMaxRetrans 10
#define itsGnLocationServiceRetransmitTimer 1000
#define itsGnLocationServicePacketBufferSize 1024
#define itsGnBeaconServiceRetransmitTimer 3000
#define itsGnBeaconServiceMaxJitter itsGnMaxPacketLifetime/4
#define itsGnDefaultHopLimit 10
#define itsGnMaxPacketLifeTime 600
#define itsGnMinPacketRepetitionInterval 100
#define itsGnGeoUnicastForwardingAlgorithm 0//unspecified 0, greedy 1, cbf 2
#define itsGnGeoBroadcastForwardingAlgorithm 0 //1 simple
#define itsGnGeoUnicastCbfMinTime 1
#define itsGnGeoUnicastCbfMaxTime 100
#define itsGnDefaultMaxCommunicationRange 1000
#define itsGnGeoAreaLineForwarding 0//1 enabled
#define itsGnUcForwardingPacketBufferSize 256
#define itsGnBcForwardingPacketBufferSize 1024
#define itsGnCbForwardingPacketBufferSize 256
#define itsGnTrafficClassRelevance 3
#define itsGnTrafficClassReliability 10
#define itsGnTrafficClassLatency 10



#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "udpev/udp_events.h"
#include "udpev/cb_udp_events.h"

#include "execution_codes.h"

/*!< Flag for activating the verbose mode. */
extern bool __verbose;

void print_version();
void print_help();

#endif /* MAIN_H_ */

