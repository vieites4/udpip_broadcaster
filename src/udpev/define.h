/*
 * define.h
 *
 *  Created on: 26/09/2013
 *      Author: pc
 */

#ifndef DEFINE_H_
#define DEFINE_H_

const unsigned char tsb0[1]={0x05};
const unsigned char tsb1[1]={0x15};
const unsigned char geobroad0[1]={0x04};
const unsigned char geobroad1[1]={0x14};
const unsigned char geobroad2[1]={0x24};
const unsigned char geoanycast0[1]={0x03};
const unsigned char geoanycast1[1]={0x13};
const unsigned char geoanycast2[1]={0x23};
const unsigned char geounicast[1]={0x02};
const unsigned char beacon[1]={0x01};
const unsigned char any[1]={0x00};
const unsigned char ls0[1]={0x06};
const unsigned char ls1[1]={0x16};

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
