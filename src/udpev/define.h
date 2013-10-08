/*
 * define.h
 *
 *  Created on: 26/09/2013
 *      Author: pc
 */

#ifndef DEFINE_H_
#define DEFINE_H_


#define itsGnLocalAddrConfMethod  0//Managed(1) Auto (0)
#define itsGnProtocolVersion 0
#define itsGnStationType 0 //0 vehicle 1 roadside
#define itsGnMinimunUpdateFrequencyLPV 1//0 roadside //1000 ms
#define itsGnMaxSduSize 1398
#define itsGnMaxGeoNetworkingHeaderSize 88
#define itsGnLifetimeLocTE 20
#define itsGnLocationServiceMaxRetrans 10
#define itsGnLocationServiceRetransmitTimer 1000
#define itsGnLocationServicePacketBufferSize 1024
#define itsGnBeaconServiceRetransmitTimer 3 //3000 ms
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
#define itsGnTrafficClassReliability 2//10 é binario
#define itsGnTrafficClassLatency 2 //10 é binario
#define ETH_ALEN	6
#endif /* DEFINE_H_ */


/*void GeoUnicast(int option){ //send(source),forward,reception(destination)

	switch(option)
				{
					case 0: //send
					//	{
					//{////if a GN-DATA.request arrive with a Pkt transport type=GeoUnicast and GN_ADDR=GN_ADDR in DE pv
			//		LL_ADDR_NH=determine_nexthop();
					////create GNPFU
					// set Common Header fields
					//set GeoUnicast Extended Header
			//		if (LL_ADDR_NH=0){
						//buffer pkt in UC_forwardin_pkt_buffer
						//break;

				//	}
	//			if (Repetitional interval of GN-DATA.request){ // este terá que ser variable de entrada
						//save GeoUnicast pkt
						//retransmit pkt with period Repetition interval until Maximum lifetime[LT] (at GN-DATA.request)//tamén variable de entrada
	//				}
					//				if (Communication profile of GN-DATA.request= ITS-G5A){
						//operations specified in i.2???????????
					//				}
					//enviar GN-PDU->LL, destination=LL_ADDR_NH


						//break;
				//	}
					case 1: //forward
				//	{ ////if a GN-DATA.request arrive with a Pkt transport type=GeoUnicast and GN_ADDR/=GN_ADDR in DE pv
						////itsGnGeoUnicastForwardingAlgorithm. annex C
						//------------------------x-----------------////inicio da parte común
						CommonHeader_processing(); //114 de netmanagement.c
						if( mngdaddrconf(3)){ //duplicate

						}

											//discard the packet
											break;
					//					}
						//update SO PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
							//				if (so/=se){ IS_NEIGHBOUR=FALSE}

						//								if (SO LS_pending){
												//flush packet buffers. SO LS packet buffer
												//forward the stored packets
												// SO LS_pending=false;
						//									}

									//		if(UC_forwarding_pkt_buffer!=empty){//flush the UC_forwarding_pkt_buffer
												//forward the stored packets

									//		}
					//------------------------x-----------------////fin da parte común
						//update DE PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
					////update fields of Common Header
					//HL--
					//SE PV=LPV

					// DE PV=DE PE of LocT (clause B.3)
	//				if (HL=0){
						//discard GN-PDU
						//break;
	//						}
	//				LL_ADDR_NH= determine_nexthop();
					//		if (LL_ADDR_NH=0){
					//buffer pkt in UC_forwardin_pkt_buffer
					//break;
					//	}
//					if (Communication profile of GN-DATA.request= ITS-G5A){
											//operations specified in i.2???????????
//										}
					//enviar GN-PDU->LL, destination=LL_ADDR_NH

	//					break;
				//	}

					default://reception
					{
						//------------------------x-----------------////inicio da parte común
						CommonHeader_processing(); //114 de netmanagement.c
						if( mngdaddrconf(3)){ //duplicate
						//discard the packet
						break;
						}
						//update SO PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
						//				if (so/=se){ IS_NEIGHBOUR=FALSE}

	//					if (SO LS_pending){
						//flush packet buffers. SO LS packet buffer
						//forward the stored packets
						// SO LS_pending=false;
																		}
//
						//		if(UC_forwarding_pkt_buffer!=empty){//flush the UC_forwarding_pkt_buffer
						//forward the stored packets

						//		}
						//------------------------x-----------------////fin da parte común

						//pass the payload to upper protocol with GN-DATA.indication and parameters of table 22

				//		break;
				//	}


				}



//};
}*/


/**
void LS(int option){
	switch(option)
				{
					case 1: //initial transmission of LS Request
					{
						printf("initial LS Request\n");
//						if (LS_pending){
//							pkt2buffer();
//							break;
//										}
			//			else{printf("envío o paquete LS request");
					     //start timer Tls_gnaddr= itsGnLocationServiceRetransmitTimer"
						//initialize the retransmit counter RTCls_gnaddr==0;
						//add the packet in LocTE and set LS_pending=true
				//
				//		}
						break;
					}
					case 2://LS Request re-transmission
					{
						//aquí entraríamos no caso de que o timer Tls_gnaddr expirara


//			if( RTCls_gnaddr<itsGnLocationServiceMaxRetrans){
//
			//reenviar o packete LS Request como un TSB.
			//restart Tls_gnaddr=itsGnLocationServiceRetransmitTimer
			//RTCls_gnaddr++;

//			}
//			else{
				//flush the LS packet buffer for the sought GN_ADDR and discard the stored packets;
				//remove the LocTE for the sought(desexado) GN_ADDR

//			}

			printf("LS Request re-transmission");


						break;

					}
					case(3)://receive LS Reply //sería a opcion 3
					{
						//if the source receives a LS Reply packet for the sought GN_ADDR

						//------------------------x-----------------////inicio da parte común
				//Common Header processing
				if( mngdaddrconf(3)){

					//discard the packet
				//	break;
				}else{

//update SO PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
	//				if (so/=se){ IS_NEIGHBOUR=FALSE}

				//	if (SO LS_pending){
						//flush packet buffers. SO LS packet buffer
						//forward the stored packets
						// SO LS_pending=false;
						}

			//		if(UC_forwarding_pkt_buffer!=empty){//flush the UC_forwarding_pkt_buffer
						//forward the stored packets

			//		}
					//------------------------x-----------------////fin da parte común

					//flush LS_pkt_buffer for the sought GN_ADDR & forward the stored packets
					//LS_pending=false of GN_ADDR
					//stop Tls_gnaddr
					//reset RTCls_gnaddr


					//}


					printf("LS Reply\n");

										break;



				}


					default:{ //recepción da LS Request
						//despois de comprobar na recepcion  que ten o GN_ADDR correcto

						//Common Header processing
						if( mngdaddrconf(3)){

						//discard the packet
						break;
					}else{
						//B.2
						//IF(SO/=SE)SO IS_NEIGHBOUR=FALSE
						//

					}

					break;
					}
				}



}**/
