/*
 * pkt_handling.c
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */


//#include "../udpev/define.h"


#include "pkt_handling.h"
//#include "../udpev/cb_udp_events.c"
const unsigned char tipoa[1]={0x01};
const unsigned char tipob[1]={0x02};
extern itsnet_position_vector * LPV;
extern int SN_g;
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
const unsigned char ETH_ADDR_BR[ETH_ALEN]                    ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
itsnet_packet * TSB(void *dato, List_lsp *lsp, List_lsp *rep){

//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
printf("xa estou dentro de tsb \n");
	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,dato +6,1);
	memcpy(ch.flags,dato +3,1);
	memcpy(ch.hop_limit,dato +2,1);
	ch.HT_HST.HT=5;
		ch.HT_HST.HST=1;
    memcpy(ch.payload_lenght,dato +4,2);
    memcpy(ch.version_nh,dato +7,1);
	ch.forwarder_position_vector=* LPV;

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
	memcpy(tsb_h.payload.payload,dato +20,lon_int);
	pkt->payload.itsnet_tsb=tsb_h;
	pkt->common_header=ch;


//NEIGHBOURS.
//if  (exist_neighbours()== false){

	itsnet_packet * pkt1 = NULL;
	//	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		// *pkt1=NULL;
	//int i =add_end_lsp(lsp, pkt);
//return(pkt1);
		//buffer in BC AND omit next executions

	//}

	if (memcmp(dato +7,tipoa,1)==0)
		{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,dato + 18,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,dato + 16,2);
		}else{
			char *info_dest=NULL;
			info_dest = (char *)malloc(2);
			memset(info_dest,0,2);
			memcpy(pkt->payload.itsnet_tsb.payload.btp1,dato + 18,2);
			memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);		}
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

itsnet_packet * SHB(void *dato, List_lsp *lsp, List_lsp *rep){


	printf("SHB ");
	//printf(geobroad1);
	//	printf("\n");
	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	//printf("xa estou dentro de shb \n");
	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,dato +6,1);
	memcpy(ch.flags,dato +3,1);
	memcpy(ch.hop_limit,dato +2,1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=0;
    memcpy(ch.payload_lenght,dato +4,2);
    memcpy(ch.version_nh,dato +7,1);
    ch.forwarder_position_vector=* LPV;
	itsnet_shb_t shb_h;
	SN_g++;
	memcpy(shb_h.payload.payload,dato +20,lon_int);
	pkt->common_header=ch;
	pkt->payload.itsnet_shb=shb_h;

	//NEIGHBOURS.
	//	if  (exist_neighbours()== false){

		itsnet_packet * pkt1 = NULL;
		//	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
			// *pkt1=NULL;
		//int i =add_end_lsp(lsp, pkt);
	//return(pkt1);
			//buffer in BC AND omit next executions
	//}
	if (memcmp(dato +7,tipoa,1)==0)
	{memcpy(pkt->payload.itsnet_shb.payload.btp1,dato + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,dato + 16,2);
	}else{char *info_dest=NULL;
	info_dest = (char *)malloc(2);
	memset(info_dest,0,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp1,dato + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);		}
	//REPETITION INTERVAL
	char *REP=NULL;
	REP = (char *)malloc(4);
	memcpy(REP,dato +8,4);
	if(atoi(REP)==0){
		//add_end_rep(rep_bc_g, pkt);

			//GARDAR O PAQUETE
			//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
			 }
//	printf("saio de shb\n");
	return(pkt);}

itsnet_packet * GeoBroadcast(void *dato, List_lsp *lsp, List_lsp *rep){

//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	printf("xa estou dentro de geobroadcast \n");
	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,dato +6,1);
	memcpy(ch.flags,dato +3,1);
	memcpy(ch.hop_limit,dato +2,1);
	ch.HT_HST.HT=4;
		ch.HT_HST.HST=0; //ESTE HAI QUE ADAPTALO
	memcpy(ch.payload_lenght,dato +4,2);
	memcpy(ch.version_nh,dato +7,1);
	ch.forwarder_position_vector=* LPV;
	char *TS=NULL;
	TS = (char *)malloc(4);
	memcpy(TS,dato +12,4);
	char TS_default[1]={0xf2}; //habería que ver como facer a conversión de segundos a o sistema dun so byte que está formado polo multiplier e a base.
//	ch.txpower=0;
//create extended header
	ch.forwarder_position_vector=* LPV;
	char *SO_pv=NULL;
	SO_pv= (char *)malloc(28);
	itsnet_geobroadcast_t gbc_h;
	memcpy(gbc_h.lt,TS_default,1);
	gbc_h.sequencenumber=SN_g;
	printf("SN %d\n",gbc_h.sequencenumber);
	SN_g++;
	memcpy(gbc_h.dest_latitude,dato +16,4);
	memcpy(gbc_h.dest_longitude,dato +20,4);
	memcpy(gbc_h.distanceA,dato +24,2);
	memcpy(gbc_h.distanceB,dato +26,2);
	memcpy(gbc_h.angle,dato +28,2);
	memcpy(gbc_h.payload.payload,dato +36,lon_int);
	gbc_h.source_position_vector=* LPV;
	pkt->payload.itsnet_geobroadcast=gbc_h;
	pkt->common_header=ch;
//NEIGHBOURS.
//	if  (exist_neighbours()== false){

	itsnet_packet * pkt1 = NULL;
	//	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		// *pkt1=NULL;
	//int i =add_end_lsp(lsp, pkt);
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
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,dato + 34,2);
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,dato + 32,2);
	}else{char *info_dest=NULL;
		info_dest = (char *)malloc(2);
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,dato + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);	}
	printf("saio de geobroadcast\n");
	//free(TS);free(SO_pv);
return(pkt);
}

void GeoUnicast(){}
void GeoAnycast(){}
void CommonHeader_processing(public_ev_arg_r *arg){

	////this is the first thing we must do after reception of a GN pkt.
	char *dato= (char *)malloc(arg->len);
	memcpy(dato,arg->data,arg->len);
    //printf("entro en common header processing\n");
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy( PV,dato +8,28);
	flags_t * FLAG=NULL;//
	FLAG= (flags_t *)malloc(sizeof(flags_t));
	memcpy( FLAG,dato +3,1);
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
	char *HT1=NULL;
	HT1 = (char *)malloc(2);
	memcpy(HT1,dato,2);
	char *SN=NULL;
	SN = (char *)malloc(2);
	memcpy(SN,arg->data+36,2);

	int lon_int=sprint_hex_data( SN, 2);

    if ((memcmp(HT1,tsb1,1)==0)||(memcmp(HT1,geobroad2,1)==0)||(memcmp(HT1,geobroad1,1)==0)||(memcmp(HT1,geobroad0,1)==0))
	{data->Sequence_number=lon_int;    printf("entro en asignacion do SN\n");}else{data->Sequence_number=0;    //printf("entro en asignacion a 0\n");


	}

	data->LS_PENDING=false;
	//segundo sexan dun tipo ou doutro terán SN no extended headerdata.Sequence_number=
	if (search_in_locT(data,arg->locT)==0){add_end_locT (  arg->locT,*data);}
		int num=strtol(HT1,NULL,16);
		int num1=0x0f00*num;
		int num2=0x00f0*num;
		if (num1==0 || num2==0){
			//discard the packet
				//break;
		}
if(arg->lsp->len>0){
Element_lsp *pos=arg->lsp->init;
//recorrer a lista e enviar todo
while(pos!=NULL){

	itsnet_packet * pkt1;
		pkt1 = & pos->data;

		char Hop[1] ; //colle perfectamente os valores sen facer a reserva de memoria
				memcpy(Hop,pos->data.common_header.hop_limit,1);
				printf("esta conversion\n");
				int lon_int=sprint_hex_data(pos->data.common_header.hop_limit, 2);
		sprintf(pkt1->common_header.hop_limit,"X02",lon_int-1);
							pkt1->common_header.forwarder_position_vector=* LPV; //

				char h_source[ETH_ALEN];
						get_mac_address(arg->forwarding_socket_fd, "wlan0",(unsigned char *) h_source) ;
				ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->port, ETH_ADDR_BR,h_source);
				memcpy(tx_frame->buffer.data, data, sizeof(itsnet_packet) );
						int fwd_bytes = send_message((sockaddr_t *)arg->forwarding_addr,arg->socket_fd,&pkt1, arg->len);
						pos=pos->next;
}

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


	//	printf("saio de common header processing\n");
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

itsnet_packet_f * TSB_f(void *dato){
	//	Create common header

		itsnet_packet_f * pkt = NULL;
		pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	    printf("xa estou dentro de tsb_f \n");
		memcpy(pkt->common_header.btp,dato+7,1);
		itsnet_position_vector * PV=NULL;//
		PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	    memcpy(PV,dato +40,28);
	    pkt->common_header.pv=*PV;
		memcpy(pkt->common_header.traffic_class,dato +6,1);

		char LEN[2] ; //colle perfectamente os valores sen facer a reserva de memoria
		memcpy(LEN,dato +4,2);
		int lon_int=sprint_hex_data( LEN, 2);
		memcpy(pkt->common_header.payload_lenght,LEN,2);
		memcpy(pkt->common_header.flags,dato +3,1);
		memcpy(pkt->common_header.hop_limit,dato+2,1);
		memcpy(pkt->payload.itsnet_unicast.payload,dato+68,lon_int);
		memcpy(pkt->common_header.pkt_type,dato,1); //non teño moi claro que realmente sexan estes os datos que se esperan no pkt type e subtype
		memcpy(pkt->common_header.pkt_stype,dato+1,1);
		printf(pkt->common_header.payload_lenght); // este para probar se realmente podemos saltarnos o paso anterior no que se garda o valor, neste caso eliminalo tamén nos tsb,shb e gbroadcast do outro lado.
		printf("saio de tsb_f \n");
	return(pkt);}

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
	return(pkt);
}

itsnet_packet_f * GeoBroadcast_f(void *dato){}

