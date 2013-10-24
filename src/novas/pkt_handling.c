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
const unsigned char single[1]={0x01};
extern const unsigned char ETH_ADDR_BROADCAST[6];
extern struct ev_loop * l_Beacon;
extern unsigned short variables[16];
extern List_locT *locT_general;
extern List_lsp * lsp_bc_g;
extern ev_timer t_Beacon;
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
	char TS[4];
	memcpy(TS,dato +12,4);
	char SO_pv[28];
	itsnet_tsb_t tsb_h;
	tsb_h.sequencenumber=SN_g;
	printf("SN %d\n",tsb_h.sequencenumber);
	char TS_default[1]={0xf2}; //habería que ver como facer a conversión de segundos a o sistema dun so byte que está formado polo multiplier e a base.
	memcpy(tsb_h.lt,TS_default,1);
	SN_g++; //máximo SN?? % SN_MAX;
	tsb_h.source_position_vector=* LPV;
	memcpy(tsb_h.payload.payload,dato +20,lon_int);
	pkt->payload.itsnet_tsb=tsb_h;
	pkt->common_header=ch;
	//NEIGHBOURS.
	if  (locT_general->len== 0){
	itsnet_packet * pkt1 = NULL;
	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);

	//delete old buffered elements if we need more size to add a new one.
	while (val>itsGnBcForwardingPacketBufferSize){
		sup_elem_lsp(0);
		val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
	}
		int i =add_end_lsp(lsp_bc_g, *pkt);
	return(pkt1);
		//buffer in BC AND omit next executions
}
	if (memcmp(dato +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,dato + 18,2);
	memcpy(pkt->payload.itsnet_tsb.payload.btp2,dato + 16,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,dato + 18,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);		}
	//REPETITION INTERVAL
	char REP[4];
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
	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
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
	SN_g++;//máximo SN?? % SN_MAX;
	memcpy(shb_h.payload.payload,dato +20,lon_int);
	pkt->common_header=ch;
	pkt->payload.itsnet_shb=shb_h;
	//NEIGHBOURS.
	if  (locT_general->len== 0){
	itsnet_packet * pkt1 = NULL;
	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	pkt1=NULL;int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
	//delete old buffered elements if we need more size to add a new one.
	while (val>itsGnBcForwardingPacketBufferSize){
			sup_elem_lsp(0);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
	printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
	int i =add_end_lsp(lsp, *pkt);
	return(pkt1);
	//buffer in BC AND omit next executions
	}
	if (memcmp(dato +7,tipoa,1)==0)
	{memcpy(pkt->payload.itsnet_shb.payload.btp1,dato + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,dato + 16,2);
	}else{char info_dest [2];
	memset(info_dest,0,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp1,dato + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);		}
	//REPETITION INTERVAL
	char REP[4];
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
	char TS [4];
	memcpy(TS,dato +12,4);
	char TS_default[1]={0xf2}; //habería que ver como facer a conversión de segundos a o sistema dun so byte que está formado polo multiplier e a base.
	//	ch.txpower=0;
	//create extended header
	ch.forwarder_position_vector=* LPV;
	char SO_pv[28];
	itsnet_geobroadcast_t gbc_h;
	memcpy(gbc_h.lt,TS_default,1);
	gbc_h.sequencenumber=SN_g;
	printf("SN %d\n",gbc_h.sequencenumber);
	SN_g++;//máximo SN?? % SN_MAX;
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
	if  (locT_general->len== 0){

	itsnet_packet * pkt1 = NULL;
	pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	pkt1=NULL;int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
	//delete old buffered elements if we need more size to add a new one.
	while (val>itsGnBcForwardingPacketBufferSize){
			sup_elem_lsp(0);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
	printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
	int i =add_end_lsp(lsp, *pkt);
	return(pkt1);
	//buffer in BC AND omit next executions
	}

	//REPETITION INTERVAL
	char REP[4];
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
	{//printf("é btp tipo a\n");
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,dato + 34,2);
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,dato + 32,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,dato + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);	}
	return(pkt);
}

void GeoUnicast(){}
void GeoAnycast(){}
int CommonHeader_processing(public_ev_arg_r *arg){
	////this is the first thing we must do after reception of a GN pkt.
	char dato[arg->len];
	memcpy(dato,arg->data,arg->len);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy( PV,dato +8+14,28);
	//print_hex_data(dato+8+14,28);printf("\n");
	flags_t * FLAG=NULL;//
	FLAG= (flags_t *)malloc(sizeof(flags_t));
	memcpy( FLAG,dato +3+14,1);
	itsnet_node * data=NULL;//
	data= (itsnet_node *)malloc(sizeof(itsnet_node));
	memcpy(data->mac_id.address,dato+6,6);
	data->IS_NEIGHBOUR=true;
	data->pos_vector= * PV;
	itsnet_time_stamp tst=data->pos_vector.time_stamp;
	free(PV);PV=NULL;
	printf("no common header\n");
	data->expires.tv_sec= itsGnLifetimeLocTE;
	data->tstation=FLAG->itsStation;
	printf("no common header\n");
	free(FLAG);FLAG=NULL;
	printf("no common header\n");
	//data.tqe;
	char HT1[1];
	memcpy(HT1,dato+14,1);
	printf("no common header\n");
	char SN[2];
	memcpy(SN,arg->data+36,2);
	printf("no common header\n");
	int lon_int=sprint_hex_data( SN, 2);
	printf("no common header\n");
	if ((memcmp(HT1,tsb1,1)==0)||(memcmp(HT1,geobroad2,1)==0)||(memcmp(HT1,geobroad1,1)==0)||(memcmp(HT1,geobroad0,1)==0))
	{data->Sequence_number=lon_int;}else{data->Sequence_number=0;
	}
	printf("no common header\n");
	data->LS_PENDING=false;
	printf("no common header\n");
	int val=search_in_locT(data,arg->locT);
	printf("no common header\n");
	if(val==0){add_end_locT (  arg->locT,*data);}else{AddTimer(variables[val],itsGnLifetimeLocTE,1);}
	printf("no common header\n");
	int num=strtol(HT1,NULL,16);
	printf("no common header\n");
	int num1=0x0f00*num;
	int num2=0x00f0*num;
	if (num1==0 || num2==0){
		//discard the packet
	//	return(1);
	}	printf("no common header\n");
	if(arg->lsp->len>0){

		Element_lsp *pos=arg->lsp->init;
		while(pos!=NULL){
			printf("no common header\n");
			//itsnet_packet * pkt1;
			//pkt1 = & pos->data;
			char Hop[1] ;
			memcpy(Hop,pos->data.common_header.hop_limit,1);
			int lon_int=sprint_hex_data(pos->data.common_header.hop_limit, 1);
			int size=sprint_hex_data(pos->data.common_header.payload_lenght, 2);
			char zero[1]={0x00};
			if (lon_int==1)memcpy(pos->data.common_header.hop_limit,zero,1); else sprintf(pos->data.common_header.hop_limit,"X02",lon_int-1);
			pos->data.common_header.forwarder_position_vector=* LPV; //
			char h_source[ETH_ALEN];
			//get_mac_address(arg->forwarding_socket_fd, "wlan0",(unsigned char *) h_source) ;
			//ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->port, ETH_ADDR_BROADCAST,h_source);
			//memcpy(&tx_frame->buffer, pkt1,strlen(pkt1) );
			sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
			send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&pos->data,sizeof(itsnet_common_header)+ size);
			printf("elementos enviados: %d \n",sizeof(itsnet_common_header)+ size);
			ev_timer_again (l_Beacon,&t_Beacon);
			char HT[1];int sn;
			printf("despois do again\n");
				memcpy(HT,&pos->data.common_header.HT_HST,1);
				printf("despois do memcpy\n");
				if(memcmp(HT,tsb1,1)==0){
					printf("entro en 1\n");
					sn = pos->data.payload.itsnet_tsb.sequencenumber;
				}else if(memcmp(HT,geobroad0,1)==0 ||memcmp(pos->data.common_header.HT_HST.HT,geobroad1,1)==0 ||memcmp(pos->data.common_header.HT_HST.HT,geobroad2,1)==0){
					printf("entro en 2\n");sn = pos->data.payload.itsnet_geobroadcast.sequencenumber;}
				printf("despois da asignacion do sn\n");
			sup_elem_lsp(sn);
			printf("despois do sup_elem_lsp\n");
			pos=pos->next;
		}
		free(data);data=NULL;
	}
	////flush pkt buffers
	printf("saio do common header");
	//flush the SE LS_pkt_buffer
	//forward the stored pkts
	//SE LS_pending=FALSE
	//}
	//IF (SE UC_forwarding_pkt_buffer!= empty){
	//flush the UC forwarding pkt buffer
	//forward stored pkts //quere dicir que se envíen os pkts que quitamos mediante o flush??
	//}

return(0);
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
	memcpy(PV,dato +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,dato +6,1);

	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,dato +3,1);
	memcpy(pkt->common_header.hop_limit,dato+7,1);
	memcpy(pkt->payload.itsnet_unicast.payload,dato+64,lon_int);
	memcpy(pkt->common_header.pkt_type,dato,1);
	memcpy(pkt->common_header.pkt_stype,dato+1,1);
	// este para probar se realmente podemos saltarnos o paso anterior no que se garda o valor, neste caso eliminalo tamén nos tsb,shb e gbroadcast do outro lado.
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
	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,dato +3,1);
	memcpy(pkt->common_header.hop_limit,dato+7,1);
	memcpy(pkt->payload.itsnet_unicast.payload,dato+36,lon_int);
	memcpy(pkt->common_header.pkt_type,dato,1);
	memcpy(pkt->common_header.pkt_stype,dato+1,1);
	// este para probar se realmente podemos saltarnos o paso anterior no que se garda o valor, neste caso eliminalo tamén nos tsb,shb e gbroadcast do outro lado.
	return(pkt);
}

itsnet_packet_f * GeoBroadcast_f(void *dato){

	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	printf("xa estou dentro de geo_f \n");
	memcpy(pkt->common_header.btp,dato+7,1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,dato +8,28);
	pkt->common_header.pv=*PV; //o problema está aqui!!
	//print_hex_data(pkt->common_header.pv,28);printf("\n");
	memcpy(pkt->common_header.traffic_class,dato +6,1);
	char LEN[2] ;
	memcpy(LEN,dato +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,dato +3,1);
	memcpy(pkt->common_header.hop_limit,dato+7,1);
	memcpy(pkt->payload.itsnet_geocast.angle,dato+80,2);
	memcpy(pkt->payload.itsnet_geocast.distanceA,dato+76,2);
	memcpy(pkt->payload.itsnet_geocast.distanceB,dato+78,2);
	memcpy(pkt->payload.itsnet_geocast.payload,dato+84,lon_int);
	memcpy(pkt->payload.itsnet_geocast.repetitionInterval,dato+8,4);
	memcpy(pkt->payload.itsnet_geocast.lt,dato+38,4);
	memcpy(pkt->payload.itsnet_geocast.reserved,dato+2,1);
	memcpy(pkt->common_header.pkt_type,dato,1);
	memcpy(pkt->common_header.pkt_stype,dato+1,1);

	return(pkt);

}
int geo_limit(void *HT,itsnet_packet_f *pkt)
{
		int x,y,r,total,a,b;
	x=abs(pkt->common_header.pv.latitude - LPV->latitude);
//	printf("num %d %d \n",pkt->common_header.pv.latitude , LPV->latitude);
	y=abs(pkt->common_header.pv.longitude - LPV->longitude);
	if(memcmp(geobroad0,HT,1)==0){
		printf("circular \n");
		r=sprint_hex_data(pkt->payload.itsnet_geocast.distanceA, 2);
		//printf("aqui %d %d %d\n",y,x,r);
		total= 1- pow((x/r),2) - pow((y/r),2);
		//printf("aqui\n");
	}else
			if(memcmp(geobroad1,HT,1)==0){
				a=sprint_hex_data(pkt->payload.itsnet_geocast.distanceA, 2);
				b=sprint_hex_data(pkt->payload.itsnet_geocast.distanceB, 2);
				total= fmin(1- pow((x/a),2) ,1- pow((y/b),2));
				printf("rectangular \n");} else
					if(memcmp(geobroad2,HT,1)==0){
						a=sprint_hex_data(pkt->payload.itsnet_geocast.distanceA, 2);
						b=sprint_hex_data(pkt->payload.itsnet_geocast.distanceB, 2);
						total= 1- pow((x/a),2) - pow((y/b),2);
						printf("eliptica \n");}
	//printf("aqui\n");
	return(total);

}

