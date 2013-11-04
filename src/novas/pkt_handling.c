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
extern struct ev_loop * l_Beacon;
extern unsigned short dictionary[16];
extern List_locT *locT_general;
extern List_lsp * lsp_bc_g;
extern ev_timer t_Beacon;
itsnet_packet * TSB(void *buffer, List_lsp *lsp, List_lsp *rep){
	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	printf("xa estou dentro de tsb \n");
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.hop_limit,(char *)(buffer) +2,1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=1;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	memcpy(ch.version_nh,(char *)(buffer) +7,1);
	ch.forwarder_position_vector=* LPV;
	//create extended header
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	char SO_pv[28];
	itsnet_tsb_t tsb_h;
	tsb_h.sequencenumber=SN_g;
	printf("SN %d\n",tsb_h.sequencenumber);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(tsb_h.lt,(void *)lt,1);
	SN_g++; //máximo SN?? % SN_MAX;
	tsb_h.source_position_vector=* LPV;
	memcpy(tsb_h.payload.payload,buffer +20,lon_int);
	pkt->payload.itsnet_tsb=tsb_h;
	pkt->common_header=ch;
	//NEIGHBOURS.
	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);

		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
			printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
		int i =add_end_lsp(lsp_bc_g, *pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_tsb.payload.btp2,(char *)(buffer) + 16,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);		}
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,buffer +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	printf("saio de tsb\n");
	return(pkt);
}

itsnet_packet * SHB(void *buffer, List_lsp *lsp, List_lsp *rep){

	printf("SHB ");
	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.hop_limit,(char *)(buffer) +2,1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=0;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	memcpy(ch.version_nh,(char *)(buffer) +7,1);
	ch.forwarder_position_vector=* LPV;
	itsnet_shb_t shb_h;
	SN_g++;//máximo SN?? % SN_MAX;
	memcpy(shb_h.payload.payload,(char *)(buffer) +20,lon_int);
	pkt->common_header=ch;
	pkt->payload.itsnet_shb=shb_h;
	//NEIGHBOURS.
	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		pkt1=NULL;int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
			sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
		}
		int i =add_end_lsp(lsp, *pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,(char *)(buffer) + 16,2);
	}else{char info_dest [2];
	memset(info_dest,0,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);		}
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer) +8,4);
	if(atoi(REP)==0){
		//add_end_rep(rep_bc_g, pkt);
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	//	printf("saio de shb\n");
	return(pkt);}

itsnet_packet * GeoBroadcast(void *buffer, List_lsp *lsp, List_lsp *rep){

	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	printf("xa estou dentro de geobroadcast \n");
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer)  +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,(char *)(buffer)  +6,1);
	memcpy(ch.flags,(char *)(buffer)  +3,1);
	memcpy(ch.hop_limit,(char *)(buffer)  +2,1);
	ch.HT_HST.HT=4;
	ch.HT_HST.HST=0; //ESTE HAI QUE ADAPTALO
	memcpy(ch.payload_lenght,(char *)(buffer)  +4,2);
	memcpy(ch.version_nh,(char *)(buffer)  +7,1);
	ch.forwarder_position_vector=* LPV;
	itsnet_geobroadcast_t * gbc_h=NULL;
	gbc_h=(itsnet_geobroadcast_t *)malloc(sizeof(itsnet_geobroadcast_t));
	char TS [4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(gbc_h->lt,(void *)lt,1);
	//char TS_default[1]={0xf2};
	//	ch.txpower=0;
	//create extended header
	ch.forwarder_position_vector=* LPV;
	char SO_pv[28];
	gbc_h->sequencenumber=SN_g;
	printf("SN %d\n",gbc_h->sequencenumber);
	SN_g++;//máximo SN?? % SN_MAX;
	memcpy(gbc_h->dest_latitude,(char *)(buffer) +16,4);
	memcpy(gbc_h->dest_longitude,(char *)(buffer) +20,4);
	memcpy(gbc_h->distanceA,(char *)(buffer) +24,2);
	memcpy(gbc_h->distanceB,(char *)(buffer) +26,2);
	memcpy(gbc_h->angle,(char *)(buffer) +28,2);
	memcpy(gbc_h->payload.payload,(char *)(buffer) +36,lon_int);
	gbc_h->source_position_vector=* LPV;
	memcpy(&pkt->payload.itsnet_geobroadcast,gbc_h,lon_int +48);//print_hex_data(gbc_h,20);printf(" gbc_h\n");
	pkt->common_header=ch;
	//NEIGHBOURS.
	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		pkt1=NULL;int val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+sprint_hex_data(pkt->common_header.payload_lenght,2);
			printf("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
		int i =add_end_lsp(lsp, *pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer)  +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	if (itsGnGeoBroadcastForwardingAlgorithm==0 || itsGnGeoBroadcastForwardingAlgorithm==1){
		//execute simple geobroadcast forwarding algorithm
		//implement F function to obtain LL_ADDR
	}
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{//printf("é btp tipo a\n");
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer)  + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,(char *)(buffer)  + 32,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer)  + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);	}
	return(pkt);
}

void GeoUnicast(){}
void GeoAnycast(){}
int CommonHeader_processing(public_ev_arg_r *arg){
	////this is the first thing we must do after reception of a GN pkt.
	char buffer[arg->len];
	memcpy(buffer,arg->data,arg->len);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy( PV,buffer +8+14,28);
	flags_t * FLAG=NULL;//
	FLAG= (flags_t *)malloc(sizeof(flags_t));
	memcpy( FLAG,buffer +3+14,1);
	itsnet_node * data=NULL;//
	data= (itsnet_node *)malloc(sizeof(itsnet_node));
	memcpy(data->mac_id.address,buffer+6,6);
	data->IS_NEIGHBOUR=true;
	data->pos_vector= * PV;
	itsnet_time_stamp tst=data->pos_vector.time_stamp;
	free(PV);PV=NULL;
	data->expires.tv_sec= itsGnLifetimeLocTE;
	data->tstation=FLAG->itsStation;
	free(FLAG);FLAG=NULL;
	char HT1[1];
	memcpy(HT1,buffer+15,1);

	if(memcmp(HT1,beacon,1)!=0 ){
	//printf("entro1\n");
	char HL[1];
	memcpy(HL,buffer+14+7,1);
	int lon_int=sprint_hex_data( HL, 1);
	//printf("entro2 %d\n", lon_int);
	//print_hex_data(buffer,40);printf("\n");

	if ((memcmp(HT1,tsb1,1)==0 && (lon_int>1))||(memcmp(HT1,geobroad2,1)==0)||(memcmp(HT1,geobroad1,1)==0)||(memcmp(HT1,geobroad0,1)==0))
	{printf("entro3\n");
		char SN[2];
		memcpy(SN,arg->data+36+15,1);
		memcpy(SN+1,arg->data+36+14,1);
	//	print_hex_data(arg->data,60);printf("\n");print_hex_data(SN,2);
		int lon_int=sprint_hex_data( SN, 2);printf(" sequence number %d\n", lon_int);
		data->Sequence_number=lon_int;}else{data->Sequence_number=0;
		}
	data->LS_PENDING=false;
	}
	int val=search_in_locT(data,locT_general);printf("%d \n",val);
	if(val==0){add_end_locT (locT_general,*data);}else{AddTimer(dictionary[val],itsGnLifetimeLocTE,1);}printf("entro4\n");
	//int num=strtol(HT1,NULL,16);printf("entro4 %d\n",num);
	//int num1=0x0f00*num;
	//int num2=0x00f0*num;printf("entro4\n");
	//if (num1==0 || num2==0){
		//discard the packet
		//	return(1);
	//}
	if(arg->lsp->len>0){
printf("arg->lsp->len %d \n",arg->lsp->len);
		Element_lsp *pos=arg->lsp->init;
		while(pos!=NULL){
			char Hop[1] ;tTimer * temp;
			memcpy(Hop,pos->data.common_header.hop_limit,1);
			int lon_int=sprint_hex_data(pos->data.common_header.hop_limit, 1);
			int size=sprint_hex_data(pos->data.common_header.payload_lenght, 2);
			char zero[1]={0x00};
			if (lon_int==1)memcpy(pos->data.common_header.hop_limit,zero,1); else sprintf(pos->data.common_header.hop_limit,"X02",lon_int-1);
			pos->data.common_header.forwarder_position_vector=* LPV; //
			char HT[1];int sn=0xffff;int base=0;int mult=0;LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};	int num3=0;int num4=0;
			memcpy(HT,&pos->data.common_header.HT_HST,1);
			if(memcmp(HT,tsb1,1)==0){
				printf("entro en tsb1\n");
				sn = pos->data.payload.itsnet_tsb.sequencenumber;
				temp=FindTimer(sn,2);
				if (temp->Period>6401){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult=(int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
				sprintf(str2, "%04X",mult);
				sprintf(str1, "%01X",base);
				num4=strtol(str2,NULL,16);
				num3=strtol(str1,NULL,16);
				lt->base=num3;lt->multiple=num4;
				memcpy(pos->data.payload.itsnet_tsb.lt,(void *)lt,1);

			}else if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){
				sn = pos->data.payload.itsnet_geobroadcast.sequencenumber;
				temp=FindTimer(sn,2);
				if (temp->Period>640){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult= (int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
				printf("no de abaixo %d %d \n",base, mult);
				sprintf(str2, "%04X",mult);
				sprintf(str1, "%01X",base);
				num4=strtol(str2,NULL,16);printf("%d\n", num4);
				num3=strtol(str1,NULL,16);printf("%d\n", num3);
				lt->base=num3;lt->multiple=num4;
				memcpy(pos->data.payload.itsnet_geobroadcast.lt,(void *)lt,1);

			}free(lt);lt=NULL;
			char h_source[ETH_ALEN];
			sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
			send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&pos->data,sizeof(itsnet_common_header)+ size);
			printf("elementos enviados: %d \n",sizeof(itsnet_common_header)+ size);
			ev_timer_again (l_Beacon,&t_Beacon);

			sup_elem_lsp(sn);
			printf("despois do sup_elem_lsp\n");
			pos=pos->next;
		}
		free(data);data=NULL;
	}
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

itsnet_packet_f * TSB_f(void *buffer){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	printf("xa estou dentro de tsb_f \n");
	memcpy(pkt->common_header.btp,(char *)(buffer)+7,1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+2,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+64,lon_int);
	memcpy(pkt->common_header.pkt_type,buffer,1);
	memcpy(pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	printf("saio de tsb_f \n");
	return(pkt);}

itsnet_packet_f * SHB_f(void *buffer){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	printf("xa estou dentro de shb_f \n");
	memcpy(pkt->common_header.btp,(char *)(buffer)+7,1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+2,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+36,lon_int);
	memcpy(pkt->common_header.pkt_type,buffer,1);
	memcpy(pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	printf("medidas %d< %d",lon_int, ITSNET_DATA_SIZE);
	free(PV);PV=NULL;
	return(pkt);
}

itsnet_packet_f * GeoBroadcast_f(void *buffer){
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	printf("xa estou dentro de geo_f \n");
	memcpy(pkt->common_header.btp,(char *)(buffer)+7,1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+2,1);
	memcpy(pkt->payload.itsnet_geocast.angle,(char *)(buffer)+80,2);
	memcpy(pkt->payload.itsnet_geocast.distanceA,(char *)(buffer)+76,2);
	memcpy(pkt->payload.itsnet_geocast.distanceB,(char *)(buffer)+78,2);
	memcpy(pkt->payload.itsnet_geocast.payload,(char *)(buffer)+84,lon_int);
	memcpy(pkt->payload.itsnet_geocast.repetitionInterval,(char *)(buffer)+8,4);
	memcpy(pkt->payload.itsnet_geocast.lt,(char *)(buffer)+38,4);
	memcpy(pkt->payload.itsnet_geocast.reserved,(char *)(buffer)+2,1);
	memcpy(pkt->common_header.pkt_type,buffer,1);
	memcpy(pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	return(pkt);

}
int geo_limit(void *HT,itsnet_packet_f *pkt)
{
	int x,y,r,total,a,b;
	x=abs(pkt->common_header.pv.latitude - LPV->latitude);
	y=abs(pkt->common_header.pv.longitude - LPV->longitude);
	if(memcmp(geobroad0,HT,1)==0){
		printf("circular \n");
		r=sprint_hex_data(pkt->payload.itsnet_geocast.distanceA, 2);
		total= 1- pow((x/r),2) - pow((y/r),2);
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
	return(total);
}
