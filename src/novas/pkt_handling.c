/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */



#include "pkt_handling.h"

const unsigned char tipoa[1]={0x01};
const unsigned char tipob[1]={0x02};
extern itsnet_position_vector * LPV;

extern List_locT * locT;
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

char *portoD=NULL;
	char *portoO=NULL;
	portoD = (char *)malloc(2);
	portoO = (char *)malloc(2);
memcpy(portoO,dato + 16,2);
	memcpy(portoD,dato + 18,2);


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
	ch.forwarder_position_vector=* LPV;
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

	tsb_h.source_position_vector=* LPV;
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

	if (memcmp(dato +7,tipoa,1)==0)
		{printf("é btp tipo a\n");
		//print_hex_data(portoD,2);
		//printf("\n");
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,portoD,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,portoO,2);

		}
		else{
			memcpy(portoD,dato + 18,2);
			char *info_dest=NULL;
			info_dest = (char *)malloc(2);
			memset(info_dest,0,2);
			memcpy(pkt->payload.itsnet_tsb.payload.btp1,portoD,2);
			memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);

		}
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

itsnet_packet * SHB(void *dato){

	char *portoD=NULL;
		char *portoO=NULL;
		portoD = (char *)malloc(2);
		portoO = (char *)malloc(2);
	memcpy(portoO,dato + 16,2);
		memcpy(portoD,dato + 18,2);
	//	Create common header
		itsnet_packet * pkt = NULL;
		pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		itsnet_common_header ch;

	printf("xa estou dentro de shb \n");

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
		char LENPROBA[3]="ff";
		memcpy(ch.traffic_class,TC,1);
		memcpy(ch.hop_limit,HL,1);
ch.forwarder_position_vector=* LPV;
//print_hex_data((char *) LPV,28);

		//ch->forwarder_position_vector=;

	//create extended header

		itsnet_shb_t shb_h;

				SN_g++;

		//tsb_h.source_position_vector=;
		//tsb_h->source_position_vector.node_id=; SN
		//...LPV
		memcpy(shb_h.payload.payload,dato +20,lon_int);

		//printf("METINO AQUI\n");
			//	print_hex_data(tsb_h.payload.payload, lon_int);
		pkt->common_header=ch;
//memcpy(pkt->proba,LENPROBA,1);
		pkt->payload.itsnet_shb=shb_h;

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


		if (memcmp(dato +7,tipoa,1)==0)
		{//printf("é btp tipo a\n");
		//print_hex_data(portoD,2);
		//printf("\n");
		memcpy(pkt->payload.itsnet_shb.payload.btp1,portoD,2);
		memcpy(pkt->payload.itsnet_shb.payload.btp2,portoO,2);

		}
		else{
			memcpy(portoD,dato + 18,2);
			char *info_dest=NULL;
			info_dest = (char *)malloc(2);
			memset(info_dest,0,2);
			memcpy(pkt->payload.itsnet_shb.payload.btp1,portoD,2);
			memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);

		}
	//REPETITION INTERVAL

		char *REP=NULL;
		REP = (char *)malloc(4);
		memcpy(REP,dato +8,4);
		if(atoi(REP)==0){
			//GARDAR O PAQUETE
			//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
			 }
		printf("saio de shb\n");
	return(pkt);


}

itsnet_packet * GeoBroadcast(void *dato){
	char *portoD=NULL;
		char *portoO=NULL;
		portoD = (char *)malloc(2);
		portoO = (char *)malloc(2);
		memcpy(portoO,dato + 32,2);
		memcpy(portoD,dato + 34,2);

//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
printf("xa estou dentro de geobroadcast \n");

	char *TC=NULL;
	TC = (char *)malloc(1);
	memcpy((void *) TC,dato +6,1);
	memcpy(ch.traffic_class,TC,1);
	char LEN[2] ; //colle perfectamente os valores sen facer a reserva de memoria
	memcpy(LEN,dato +4,2);
	memcpy(ch.payload_lenght,LEN,2);
	int lon_int=sprint_hex_data( LEN, 2);
	char *FLAGS=NULL;
	FLAGS = (char *)malloc(1);
	memcpy(FLAGS,dato +3,1);
	memcpy(ch.flags,FLAGS,1);
	char *HL=NULL;
	HL = (char *)malloc(1);
	memcpy(HL,dato +2,1);
	memcpy(ch.hop_limit,HL,1);
	char *HT=NULL;
	HT = (char *)malloc(2);
	memcpy(HT,dato ,2);
    memcpy(ch.version_nh,dato +7,1);
	memcpy(ch.HT_HST,HT,1);
	char *TS=NULL;
	TS = (char *)malloc(4);
	memcpy(TS,dato +12,4);
	char TS_default[1]={0xf2}; //habería que ver como facer a conversión de segundos a o sistema dun so byte que está formado polo multiplier e a base.

//	ch.txpower=0;
	//ch->forwarder_position_vector=;

//create extended header
	ch.forwarder_position_vector=* LPV;
	char *SO_pv=NULL;
	SO_pv= (char *)malloc(28);
	itsnet_geobroadcast_t gbc_h;
	memcpy(gbc_h.lt,TS_default,1);
	gbc_h.sequencenumber=SN_g;
	printf("SN %d\n",gbc_h.sequencenumber);
		SN_g++;
	char LAT[4] ;
	memcpy(LAT,dato +16,4);
	memcpy(gbc_h.dest_latitude,LAT,4);
	char LON[4] ;
	memcpy(LON,dato +20,4);
	memcpy(gbc_h.dest_longitude,LON,4);
	char dista[2] ;
	memcpy(dista,dato +24,2);
	memcpy(gbc_h.distanceA,dista,2);
	char distb[2] ;
	memcpy(distb,dato +26,2);
	memcpy(gbc_h.distanceB,distb,2);
	char angle[2] ;
	memcpy(angle,dato +28,2);
	memcpy(gbc_h.angle,angle,2);
	//tsb_h.source_position_vector=;
	//tsb_h->source_position_vector.node_id=; SN
	//...LPV
	memcpy(gbc_h.payload.payload,dato +36,lon_int);
	gbc_h.source_position_vector=* LPV;

	pkt->payload.itsnet_geobroadcast=gbc_h;
	pkt->common_header=ch;


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

	if (itsGnGeoBroadcastForwardingAlgorithm==0 || itsGnGeoBroadcastForwardingAlgorithm==1){

		//execute simple geobroadcast forwarding algorithm

		//implement F function to obtain LL_ADDR
	}







	if (memcmp(dato +7,tipoa,1)==0)
	{printf("é btp tipo a\n");
	//print_hex_data(portoD,2);
	//printf("\n");
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,portoD,2);
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,portoO,2);

	}
	else{
		memcpy(portoD,dato + 18,2);
		char *info_dest=NULL;
		info_dest = (char *)malloc(2);
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,portoD,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);

	}

	printf("saio de geobroadcast\n");
return(pkt);
}

void GeoUnicast(){}
void GeoAnycast(){}
void CommonHeader_processing(public_ev_arg_t *arg){

	////this is the first think we must do after reception of a GN pkt.
	char *dato= (char *)malloc(arg->len);
	memcpy(dato,arg->data,arg->len);

printf("entro en common header processing\n");

	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy( PV,dato +8,28);// ESTO TEÑO QUE METELO NO LOCT E POÑER O IS_NEIGHBOUR A TRUE
	flags_t * FLAG=NULL;//
	FLAG= (flags_t *)malloc(sizeof(flags_t));
		memcpy( FLAG,dato +3,1);// ESTO TEÑO QUE METELO NO LOCT E POÑER O IS_NEIGHBOUR A TRUE


	itsnet_node * data=NULL;//
		data= (itsnet_node *)malloc(sizeof(itsnet_node));

	data->IS_NEIGHBOUR=true;
	data->pos_vector= * PV;
	itsnet_time_stamp tst=data->pos_vector.time_stamp;
	//data->mac_id=arg->local_addr->sin_addr.s_addr; // teño que buscar esta dirección!
	data->node_id=PV->node_id;
	data->expires.tv_sec= itsGnLifetimeLocTE;
	data->tstation=FLAG->itsStation;
	//data.tqe;
	data->LS_PENDING=false;
	//segundo sexan dun tipo ou doutro terán SN no extended headerdata.Sequence_number=
	if (search_in_locT(data)==0){add_end_locT (  locT,*data);}


	char *HT=NULL;
		HT = (char *)malloc(2);
		memcpy(HT,dato ,2);
		int num=strtol(HT,NULL,16);

		//printf("%02x \n",num);
		//printf(HT);
		int num1=0x0f00*num;
		int num2=0x00f0*num;
		if (num1==0 || num2==0){

			//discard the packet
				//break;

		}



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

		printf("saio de common header processing\n");
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

itsnet_packet_f * TSB_f(void *data){}

itsnet_packet_f * SHB_f(void *dato){


	//	Create common header
		itsnet_packet_f * pkt = NULL;
		pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));


	printf("xa estou dentro de shb_f \n");

		memcpy(pkt->common_header.btp,dato+7,1);
		itsnet_position_vector * PV=NULL;//
			PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	    memcpy(PV,dato +8,28);
	    pkt->common_header.pv=*PV;
		memcpy(pkt->common_header.traffic_class,dato +6,1);

		char LEN[2] ; //colle perfectamente os valores sen facer a reserva de memoria
		memcpy(LEN,dato +4,2);
		int lon_int=sprint_hex_data( LEN, 2);
		memcpy(pkt->common_header.payload_lenght,LEN,2);
		memcpy(pkt->common_header.flags,dato +3,1);
		memcpy(pkt->common_header.hop_limit,dato+2,1);
		memcpy(pkt->payload.itsnet_unicast.payload,dato+36,lon_int);
		memcpy(pkt->common_header.pkt_type,dato,1); //non teño moi claro que realmente sexan estes os datos que se esperan no pkt type e subtype
		memcpy(pkt->common_header.pkt_stype,dato+1,1);

		printf(pkt->common_header.payload_lenght); // este para probar se realmente podemos saltarnos o paso anterior no que se garda o valor, neste caso eliminalo tamén nos tsb,shb e gbroadcast do outro lado.

		printf("saio de shb_f \n");
	return(pkt);


}

itsnet_packet_f * GeoBroadcast_f(void *dato){}

