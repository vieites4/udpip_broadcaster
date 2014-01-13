/*
 * define.h
 *
 *  Created on: 26/09/2013
 *      Author: pc
 */

#ifndef DEFINE_H_
#define DEFINE_H_
#define itsGnLocalAddrConfMethod  0//Managed(1) Auto (0)
#define itsGnLocalGnAddr 1
#define itsGnProtocolVersion 0
#define itsGnStationType 0 //0 vehicle 1 roadside
#define itsGnIsMobile 0// 0 stationary 1 mobile
#define itsIfType 0 //0 unspecified 1 its-g5
#define itsGnMinimunUpdateFrequencyLPV 1//0 roadside //1000 ms
#define itsGnPaiInterval 80
#define itsGnMaxSduSize 1398
#define itsGnMaxGeoNetworkingHeaderSize 88
#define itsGnLifetimeLocTE 20
#define itsGnSecurity 0//0 disable 1 enabled
#define itsGnAnDecapResultHandling 0//0 strict 1 non-strict
#define itsGnLocationServiceMaxRetrans 10
#define itsGnLocationServiceRetransmitTimer 1000
#define itsGnLocationServicePacketBufferSize 1024
#define itsGnBeaconServiceRetransmitTimer 3 //3000 ms

#define itsGnDefaultHopLimit 10
#define itsGnMaxPacketLifeTime 600
#define itsGnBeaconServiceMaxJitter itsGnMaxPacketLifeTime/4
#define itsGnMinPacketRepetitionInterval 100
#define itsGnMaxPacketDataRate 100
#define itsGnMaxGeoAreaSize 10
#define itsGnGeoUnicastForwardingAlgorithm 0//unspecified 0, greedy 1, cbf 2
#define itsGnGeoBroadcastForwardingAlgorithm 0 //2 simple
#define itsGnGeoUnicastCbfMinTime 1
#define itsGnGeoBroadcastCbfMaxTime 100
#define itsGnGeoBroadcastCbfMinTime 1
#define itsGnGeoUnicastCbfMaxTime 100
#define itsGnDefaultMaxCommunicationRange 1000
#define Pi 3.14159
#define itsGnBroadcastCBFDefSectorAngle Pi/6 //valor en radianes!! Pi/6=30º
#define itsGnUnicastCBFDefSectorAngle 30

#define itsGnGeoAreaLineForwarding 0//1 enabled
#define itsGnUcForwardingPacketBufferSize 256
#define itsGnBcForwardingPacketBufferSize 1024000
#define itsGnCbfPacketBufferSize 256000
#define itsGnDefaultTrafficClass 0
//#define itsGnTrafficClassRelevance 3
//#define itsGnTrafficClassReliability 2//10 é binario
//#define itsGnTrafficClassLatency 2 //10 é binario

#define ETH_ALEN	6
#define ETH_P_ALL 3
//#define ETH_FRAME_LEN	1514
#define DEBUG_PRINT_ENABLED 1  // uncomment to enable DEBUG statements
#define ITSNET_DATA_SIZE 1500

#endif /* DEFINE_H_ */

