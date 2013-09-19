/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */



#include "pkt_handling.h"

extern int SN_g;
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
 * @brief convert a hexidecimal string to a signed long
 * will not produce or process negative numbers except
 * to signal error.
 *
 * @param hex without decoration, case insensative.
 *
 * @return -1 on error, or result (max sizeof(long)-1 bits)
 */




itsnet_packet * TSB(void *dato){

//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
//void *data=NULL;
//data=(void *)malloc(20);

printf("xa estou dentro de tsb \n");

//print_hex_data(dato,20);
//memcpy(data,dato,20);

	char *TC=NULL;
	TC = (char *)malloc(1);
	memcpy((void *) TC,dato +6,1);
	char LEN[2] ; //colle perfectamente os valores sen facer a reserva de memoria
	memcpy(LEN,dato +4,2);
	char *FLAGS=NULL;
	FLAGS = (char *)malloc(1);
	memcpy(FLAGS,dato +3,1);
	char *HL=NULL;
	HL = (char *)malloc(1);
	memcpy(HL,dato +2,1);
	char *HT=NULL;
	HT = (char *)malloc(2);
	memcpy(HT,dato ,2);
	memcpy(ch.payload_lenght,LEN,2);
	int lon_int=sprint_hex_data( LEN, 2);
memcpy(ch.version_nh,dato +7,1);
	memcpy(ch.HT_HST,HT,1);
//	ch.txpower=0;
	memcpy(ch.flags,FLAGS,1);
	//char LENPROBA[5]="03e8";
	memcpy(ch.traffic_class,TC,1);
	memcpy(ch.hop_limit,HL,1);

	printf("TC mide %d ,PV mide %d\n", sizeof(TC), sizeof(itsnet_position_vector));

	//ch->forwarder_position_vector=;

//create extended header
	char *TS=NULL;
	TS = (char *)malloc(4);
	memcpy(TS,dato +12,4);
	//char *SN=NULL;
	//SN= (char *)malloc(2);
	//memcpy(SN,(char *)SN_g,2); //podía ser o que devolve a funcion.

	char *SO_pv=NULL;
	SO_pv= (char *)malloc(28);
	itsnet_tsb_t tsb_h;

	tsb_h.sequencenumber=SN_g;
	printf("SN %d\n",tsb_h.sequencenumber);
	char TS_default[1]={0xf2}; //habería que ver como facer a conversión de segundos a o sistema dun so byte que está formado polo multiplier e a base.

	memcpy(tsb_h.lt,TS_default,1);
			SN_g++;

	//tsb_h.source_position_vector=;
	//tsb_h->source_position_vector.node_id=; SN
	//...LPV
	memcpy(tsb_h.payload.payload,dato +20,lon_int);

	//printf("METINO AQUI\n");
		//	print_hex_data(tsb_h.payload.payload, lon_int);


	pkt->payload.itsnet_tsb=tsb_h;
	pkt->common_header=ch;

//hoxe teño que deixar o paquete construido


//NEIGHBOURS.
//	if  (exist_neighbours()== false){

	itsnet_packet * pkt1 = NULL;
	//	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		// *pkt1=NULL;
	//int i =add_end_lsp(lsp_bc_g, pkt);
//return(pkt1);
		//buffer in BC AND omit next executions

	//}


//REPETITION INTERVAL

	char *REP=NULL;
	REP = (char *)malloc(4);
	memcpy(REP,dato +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
		 }
	printf("saio de tsb\n");
return(pkt);
}

void SHB(){}

void GeoBroadcast(){}
void GeoUnicast(){}
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

