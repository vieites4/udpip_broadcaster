/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */



#include "pkt_handling.h"


void GeoUnicast(int option){ //send(source),forward,reception(destination)

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
}
void TSB(public_ev_arg_t *arg){

//	Create common header

	itsnet_common_header ch;

	/*char *TC=NULL;
	TC = (char *)malloc(1);
	memcpy(TC,arg->data +6,1);
	char *LEN=NULL;
	LEN = (char *)malloc(2);
	memcpy(LEN,arg->data +4,2);
	char *FLAGS=NULL;
	FLAGS = (char *)malloc(1);
	memcpy(FLAGS,arg->data +3,1);
	char *HL=NULL;
	HL = (char *)malloc(1);
	memcpy(HL,arg->data +2,1);
	char *HT=NULL;
	HT = (char *)malloc(2);
	memcpy(HT,arg->data ,2);


	memcpy(ch.protocol_info,HT,2); //te
	ch.txpower=0;
	ch.flags=atoi(FLAGS);
	ch.payload_lenght=atoi(LEN);
	ch.traffic_class=atoi(FLAGS);
	ch.hop_limit=atoi(HL);
	//ch->forwarder_position_vector=;

//create extended header
	char *TS=NULL;
	TS = (char *)malloc(4);
	memcpy(TS,arg->data +12,4);


	itsnet_tsb_t tsb_h;
	tsb_h.source_position_vector.time_stamp=TS;
	//tsb_h->source_position_vector.node_id=; SN
	//...LPV
	memcpy(tsb_h.payload,arg->data +20,ch.payload_lenght);


//NEIGHBOURS.
	if  (exist_neighbours()== false){

		//buffer in BC AND omit next executions

	}


//REPETITION INTERVAL

	char *REP=NULL;
	REP = (char *)malloc(4);
	memcpy(REP,arg->data +8,4);

	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
		 }
*/

}

void SHB(){}

void GeoBroadcast(){}
void GeoAnycast(){}
void CommonHeader_processing(){

	////this is the first think we must do after reception of a GN pkt.


	//update PV in the SE LocTE with SE PV fields of CommonHeader
	//IS_NEIGHBOUR=TRUE of SE LocTE
	////flush pkt buffers
	//if(SE LS_pending){
	//flush the SE LS_pkt_buffer
    //forward the stored pkts
	//SE LS_pending=FALSE
	//}
	//IF (SE UC_forwarding_pkt_buffer!= empty){
	//flush the UC forwarding pkt buffer
	//forward stored pkts //quere dicir que se envíen os pkts que quitamos mediante o flush??
	//}
	//if(BC_forwarding_pkt_buffer!= empty){
	//flush the BC forwarding pkt buffer
	//forward stored pkts //quere dicir que se envíen os pkts que quitamos mediante o flush??
	//}
	//if (NH field of CM=0){
	//discard the packet
	//break;
	//}

	//if (HT field of CM=0){
	//discard the packet
	//break;
	//}

}


void determine_nexthop(){
//	switch(itsGnGeoUnicastForwardingAlgorithm){

//						case(0)://unspecified
					//	case(1)://greedy
//						//{
						//gf algorithm os c2
						//break;
						//}
//						case(2)://unspecified
						//{
						//cbf algorithm os c3 //returns broadcast LL or, in case of no neighbours, 0
						//break;
						//}
//						default:break;
//						}//2º switch


//	return(LL_ADDR_NH);

}

