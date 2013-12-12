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
extern uint16_t SN_g;
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

#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
const unsigned char BROADCAST[6]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
itsnet_packet * TSB(void *buffer, List_lsp *lsp, List_lsp *rep,bool g){
	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	PRF("xa estou dentro de tsb \n");
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.hop_limit,(char *)(buffer) +2,1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=1;

	if (g==false){
		memcpy(ch.payload_lenght+1,(char *)(buffer) +4,1);	memcpy(ch.payload_lenght,(char *)(buffer) +5,1);}
	else{memcpy(ch.payload_lenght,(char *)(buffer) +4,2);}
	memcpy(ch.version_nh,(char *)(buffer) +7,1);
	ch.forwarder_position_vector=* LPV;
	//create extended header
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	char SO_pv[28];
	itsnet_tsb_t tsb_h;
	tsb_h.sequencenumber=SN_g;
	PRF("SN %d\n",tsb_h.sequencenumber);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;

	char TS_zero[4]; memset(TS_zero,0,4);
	if (memcmp(TS,TS_zero,4)==0){ ts_num=itsGnMaxPacketLifeTime;}

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
	free(lt);
	memcpy(tsb_h.payload.payload,(char *)(buffer) +20,lon_int);
	//print_hex_data();printf("  no tsb\n");
	pkt->payload.itsnet_tsb=tsb_h;
	pkt->common_header=ch;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_tsb.payload.btp2,(char *)(buffer) + 16,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);		}
	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		int val=lsp_bc_g->size+sizeof(itsnet_common_header)+32+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+32+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}PRF("TSB ");
		int i =add_end_lsp(lsp_bc_g, *pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}

	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,buffer +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	//PRF("saio de tsb\n");
	return(pkt);
}

itsnet_packet * SHB(void *buffer, List_lsp *lsp, List_lsp *rep,bool g){

	PRF("SHB ");
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
	if (g==false){
		memcpy(ch.payload_lenght+1,(char *)(buffer) +4,1);	memcpy(ch.payload_lenght,(char *)(buffer) +5,1);}
	else{memcpy(ch.payload_lenght,(char *)(buffer) +4,2);}
	memcpy(ch.version_nh,(char *)(buffer) +7,1);
	ch.forwarder_position_vector=* LPV;
	itsnet_shb_t shb_h;
	SN_g++;//máximo SN?? % SN_MAX; //page 16
	//print_hex_data((char *)(buffer) +20,lon_int);printf("  no tsb\n");
	memcpy(shb_h.payload.payload,(char *)(buffer) +20,lon_int);

	pkt->common_header=ch;
	pkt->payload.itsnet_shb=shb_h;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,(char *)(buffer) + 16,2);
	}else{char info_dest [2];
	memset(info_dest,0,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);		}
	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		//pkt1=NULL;
		int val=lsp_bc_g->size+sizeof(itsnet_common_header)+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
			lsp_bc_g=sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+4+sprint_hex_data((char *)(buffer) +4,2);
		}
		int i =add_end_lsp(lsp, *pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}

	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer) +8,4);
	if(atoi(REP)==0){
		//add_end_rep(rep_bc_g, pkt);
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	//	PRF("saio de shb\n");
	//print_hex_data(pkt,lon_int +36 +4);printf("  no tsb\n");
	return(pkt);}
itsnet_packet * GeoUnicast(void *buffer, List_lsp *lsp, List_lsp *rep){


}
itsnet_packet * GeoBroadcast(void *buffer, List_lsp *lsp, List_lsp *rep,bool g){

	//	Create common header
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	//PRF("xa estou dentro de geobroadcast \n");
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer)  +4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(ch.traffic_class,(char *)(buffer)  +6,1);
	memcpy(ch.flags,(char *)(buffer)  +3,1);
	memcpy(ch.hop_limit,(char *)(buffer)  +2,1);

	memcpy(&ch.HT_HST,(char *)(buffer),1);
	//	print_hex_data(&ch.HT_HST,1);PRF(" o do HT\n");
	if (g==false){
		memcpy(ch.payload_lenght+1,(char *)(buffer) +4,1);	memcpy(ch.payload_lenght,(char *)(buffer) +5,1);}
	else{memcpy(ch.payload_lenght,(char *)(buffer) +4,2);}
	memcpy(ch.version_nh,(char *)(buffer)  +7,1);
	ch.forwarder_position_vector=* LPV;
	itsnet_geobroadcast_t gbc_h;
	char TS [4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);
	char TS_zero[4]; memset(TS_zero,0,4);
	if (memcmp(TS,TS_zero,4)==0){ ts_num=itsGnMaxPacketLifeTime;}

	int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(gbc_h.lt,(void *)lt,1);
	free(lt);
	//	ch.txpower=0;
	//create extended header
	//char SO_pv[28];
	gbc_h.sequencenumber=SN_g;
	PRF("SN %d\n",gbc_h.sequencenumber);
	SN_g++;//máximo SN?? % SN_MAX;

	//we have to reorder this bytes

	/**
	 * memcpy(gbc_h.dest_latitude,(char *)(buffer) +16,4);
	 * memcpy(gbc_h.dest_longitude,(char *)(buffer) +20,4);
	 * memcpy(gbc_h.distanceA,(char *)(buffer) +24,2);
	 * memcpy(gbc_h.distanceB,(char *)(buffer) +26,2);
	 * memcpy(gbc_h.angle,(char *)(buffer) +28,2);
	 * **/
	//print_hex_data(buffer,60);PRF("recibido desde as facilities\n");
	if (g==false){
		memcpy(gbc_h.dest_latitude,(char *)(buffer) +19,1);memcpy((char *)(gbc_h.dest_latitude)+1,(char *)(buffer) +18,1);memcpy((char *)(gbc_h.dest_latitude)+2,(char *)(buffer) +17,1);memcpy((char *)(gbc_h.dest_latitude)+3,(char *)(buffer) +16,1);
		memcpy(gbc_h.dest_longitude,(char *)(buffer) +23,1);memcpy((char *)(gbc_h.dest_longitude)+1,(char *)(buffer) +22,1);memcpy((char *)(gbc_h.dest_longitude)+2,(char *)(buffer) +21,1);memcpy((char *)(gbc_h.dest_longitude)+3,(char *)(buffer) +20,1);
		memcpy((char *)(gbc_h.distanceA)+1,(char *)(buffer) +24,1);memcpy(gbc_h.distanceA,(char *)(buffer) +25,1);
		memcpy((char *)(gbc_h.distanceB)+1,(char *)(buffer) +26,1);memcpy(gbc_h.distanceB,(char *)(buffer) +27,1);
		memcpy((char *)(gbc_h.angle)+1,(char *)(buffer) +28,1);memcpy(gbc_h.angle,(char *)(buffer) +29,1);
		memcpy((char *)(gbc_h.reserved)+1,(char *)(buffer) +30,1);memcpy(gbc_h.reserved,(char *)(buffer) +31,1);

	}
	else{memcpy(gbc_h.dest_latitude,(char *)(buffer) +16,4);
	memcpy(gbc_h.dest_longitude,(char *)(buffer) +20,4);
	memcpy(gbc_h.distanceA,(char *)(buffer) +24,2);
	memcpy(gbc_h.distanceB,(char *)(buffer) +26,2);
	memcpy(gbc_h.angle,(char *)(buffer) +28,2);
	memcpy(gbc_h.reserved,(char *)(buffer) +30,1);}

	memcpy(gbc_h.payload.payload,(char *)(buffer) +36,lon_int);
	gbc_h.source_position_vector=* LPV;
	pkt->payload.itsnet_geobroadcast=gbc_h;
	pkt->common_header=ch;



	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{//PRF("é btp tipo a\n");
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer)  + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,(char *)(buffer)  + 32,2);
	}else{
		char info_dest[2];
		//memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer)  + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);	}
	//NEIGHBOURS.



	if  (locT_general->len== 0){
		itsnet_packet * pkt1 = NULL;
		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		pkt1=NULL;int val=(lsp_bc_g->size)+48+4+(sizeof(itsnet_common_header))+(sprint_hex_data((char *)(buffer) +4,2));
		//PRF(" val: %d %d %d\n",lsp_bc_g->size,sizeof(itsnet_common_header),sprint_hex_data((char *)(buffer) +4,2));
		//delete old buffered elements if we need more size to add a new one.

		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff);
			val=lsp_bc_g->size+sizeof(itsnet_common_header)+48+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}PRF("GEO ");
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
	//if (itsGnGeoBroadcastForwardingAlgorithm==0 || itsGnGeoBroadcastForwardingAlgorithm==1){
	//execute simple geobroadcast forwarding algorithm
	//implement F function to obtain LL_ADDR
	//}
	return(pkt);
}

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
	//PRF("entro1\n");data

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
		char HL[1];
		memcpy(HL,buffer+14+7,1);
		int lon_int=sprint_hex_data( HL, 1);
		if ((memcmp(HT1,tsb1,1)==0 && (lon_int>1))||(memcmp(HT1,geobroad2,1)==0)||(memcmp(HT1,geobroad1,1)==0)||(memcmp(HT1,geobroad0,1)==0))
		{
			char SN[2];
			memcpy(SN,arg->data+36+15,1);
			memcpy(SN+1,arg->data+36+14,1);
			//	print_hex_data(arg->data,60);PRF("\n");print_hex_data(SN,2);
			uint16_t lon_int=sprint_hex_data( SN, 2);//PRF(" sequence number %d\n", lon_int);
			data->Sequence_number=lon_int;}else{data->Sequence_number=0;
			}
		data->LS_PENDING=false;
	}
	int val=search_in_locT(data,locT_general);
	if(val==0){add_end_locT (locT_general,*data);}else{AddTimer(dictionary[val],itsGnLifetimeLocTE,1);}
	//int num=strtol(HT1,NULL,16);PRF("entro4 %d\n",num);
	//int num1=0x0f00*num;
	//int num2=0x00f0*num;PRF("entro4\n");
	//if (num1==0 || num2==0){
	//discard the packet
	//	return(1);
	//}
	if(arg->lsp->len>0){
		//PRF("arg->lsp->len %d \n",arg->lsp->len);
		Element_lsp *pos=arg->lsp->init;
		while(pos!=NULL){
			char Hop[1] ;tTimer * temp;
			memcpy(Hop,pos->data.common_header.hop_limit,1);
			int lon_int=sprint_hex_data(pos->data.common_header.hop_limit, 1);
			char LEN[2] ;
			memcpy(LEN,pos->data.common_header.payload_lenght+1,1);
			memcpy(LEN+1,pos->data.common_header.payload_lenght,1);
			int size=sprint_hex_data(LEN, 2);
			char zero[1]={0x00};
			if (lon_int==1)memcpy(pos->data.common_header.hop_limit,zero,1); else sprintf(pos->data.common_header.hop_limit,"X02",lon_int-1);
			pos->data.common_header.forwarder_position_vector=* LPV; //
			char HT[1];uint16_t sn=0xffff;int base=0;int mult=0;LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};	int num3=0;int num4=0;
			memcpy(HT,&pos->data.common_header.HT_HST,1);
			if(memcmp(HT,tsb1,1)==0){
				//PRF("entro en tsb1\n");
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
				//PRF("no de abaixo %d %d \n",base, mult);
				sprintf(str2, "%04X",mult);
				sprintf(str1, "%01X",base);
				num4=strtol(str2,NULL,16);//PRF("%d\n", num4);
				num3=strtol(str1,NULL,16);//PRF("%d\n", num3);
				lt->base=num3;lt->multiple=num4;
				memcpy(pos->data.payload.itsnet_geobroadcast.lt,(void *)lt,1);

			}free(lt);lt=NULL;
			char h_source[ETH_ALEN];
			sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
			get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
			ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, BROADCAST,h_source);
			char tipo[2]={0x07,0x07};
			memcpy(tx_frame->buffer.header.type,tipo,2);

			memcpy(tx_frame->buffer.data, &pos->data, IEEE_80211_BLEN);
			//while (
			int header_length=0;

			if(memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0){header_length=84;}
			else if(memcmp(HT,tsb0,1)==0&& (memcmp(Hop,single,1)==0)){header_length=36;}else if(memcmp(HT,tsb0,1)==0&& (memcmp(Hop,single,1)!=0)){header_length=68;}else{PRF("son eu3!! \n");} //falta tsb

			send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer, header_length +size+14+4);//==-1){}
			//PRF("elementos enviados: %d \n",sizeof(itsnet_common_header)+ size);
			free(tx_frame);free(dir);
			//print_hex_data(&tx_frame->buffer, header_length +size+14+4);// header_length +lon_int+14+4);
			PRF(" paquete enviado a ll despois de lsp %d\n", header_length +size+14+4);
			ev_timer_again (l_Beacon,&t_Beacon);
PRF("con sn %d\n",sn);
			lsp_bc_g=sup_elem_lsp(sn);
			//free(&pos->data);
			//PRF("despois do sup_elem_lsp\n");
			pos=pos->next;
		}

	}
	free(data);data=NULL;
	//PRF("saio do common header");
	//flush the SE LS_pkt_buffer
	//forward the stored pkts
	//SE LS_pending=FALSE
	//}
	//IF (SE UC_forwarding_pkt_buffer!= empty){
	//flush the UC forwarding pkt buffer
	//forward stored pkts //quere dicir que se envíen os pkts que quitamos mediante o flush??
	//}

	return(0);
	//	PRF("saio de common header processing\n");
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

itsnet_packet_f * TSB_f(void *buffer,bool g){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_uni_t)+ sizeof(itsnet_common));
	//	PRF("xa estou dentro de tsb_f \n");
	memcpy(&pkt->common_header.btp,(char *)(buffer),1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	//pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;memcpy(LEN,(char *)(buffer) +5,1);memcpy(LEN+1,(char *)(buffer) +4,1);
	int lon_int=sprint_hex_data( LEN, 2);
	///if (g==false){
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	//}else{
	/**memcpy((char *)(pkt->common_header.pv.speed)+1,PV->speed,1);memcpy(pkt->common_header.pv.speed,(char *)(PV->speed) +1,1);
				memcpy((char *)(pkt->common_header.pv.heading)+1,PV->heading,1);memcpy(pkt->common_header.pv.heading,(char *)(PV->heading) +1,1);
				memcpy((char *)(pkt->common_header.pv.altitude)+1,PV->altitude,1);memcpy(pkt->common_header.pv.altitude,(char *)(PV->altitude) +1,1);
				memcpy((char *)(&pkt->common_header.pv.accuracy)+1,&PV->accuracy,1);memcpy(&pkt->common_header.pv.accuracy,(char *)(&PV->accuracy) +1,1);
				memcpy((char *)(pkt->common_header.pv.latitude)+3,PV->latitude,1);memcpy((char *)(pkt->common_header.pv.latitude)+2,(char *)(PV->latitude) +1,1);
				memcpy((char *)(pkt->common_header.pv.latitude)+1,(char *)(PV->latitude)+2,1);memcpy(pkt->common_header.pv.latitude,(char *)(PV->latitude) +3,1);
				memcpy((char *)(pkt->common_header.pv.longitude)+3,PV->longitude,1);memcpy((char *)(pkt->common_header.pv.longitude)+2,(char *)(PV->longitude) +1,1);
				memcpy((char *)(pkt->common_header.pv.longitude)+1,(char *)(PV->longitude)+2,1);memcpy(pkt->common_header.pv.longitude,(char *)(PV->longitude) +3,1);
				memcpy((char *)(pkt->common_header.pv.time_stamp)+3,PV->time_stamp,1);memcpy((char *)(pkt->common_header.pv.time_stamp)+2,(char *)(PV->time_stamp) +1,1);
				memcpy((char *)(pkt->common_header.pv.time_stamp)+1,(char *)(PV->time_stamp)+2,1);memcpy(pkt->common_header.pv.time_stamp,(char *)(PV->time_stamp) +3,1);
	 **///memcpy(pkt->common_header.payload_lenght,LEN+1,1);memcpy(pkt->common_header.payload_lenght+1,LEN,1);
	//}
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+7,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+68,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;
	free(PV);
	//PRF("saio de tsb_f \n");
	return(pkt);}

itsnet_packet_f * SHB_f(void *buffer,bool g){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_uni_t)+ sizeof(itsnet_common));
	//PRF("xa estou dentro de shb_f \n");
	memcpy(&pkt->common_header.btp,(char *)(buffer),1);
	//pkt->common_header.btp.HT=pkt->common_header.btp.HST;
	//pkt->common_header.btp.HST=0;
	itsnet_position_vector * PV=NULL;
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +5,1);memcpy(LEN+1,(char *)(buffer) +4,1);
	int lon_int=sprint_hex_data( LEN, 2);
	//if (g==false){
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	//}else{
	/**	memcpy((char *)(pkt->common_header.pv.speed)+1,PV->speed,1);memcpy(pkt->common_header.pv.speed,(char *)(PV->speed) +1,1);
		memcpy((char *)(pkt->common_header.pv.heading)+1,PV->heading,1);memcpy(pkt->common_header.pv.heading,(char *)(PV->heading) +1,1);
		memcpy((char *)(pkt->common_header.pv.altitude)+1,PV->altitude,1);memcpy(pkt->common_header.pv.altitude,(char *)(PV->altitude) +1,1);
		memcpy((char *)(&pkt->common_header.pv.accuracy)+1,&PV->accuracy,1);memcpy(&pkt->common_header.pv.accuracy,(char *)(&PV->accuracy) +1,1);
		memcpy((char *)(pkt->common_header.pv.latitude)+3,PV->latitude,1);memcpy((char *)(pkt->common_header.pv.latitude)+2,(char *)(PV->latitude) +1,1);
		memcpy((char *)(pkt->common_header.pv.latitude)+1,(char *)(PV->latitude)+2,1);memcpy(pkt->common_header.pv.latitude,(char *)(PV->latitude) +3,1);
		memcpy((char *)(pkt->common_header.pv.longitude)+3,PV->longitude,1);memcpy((char *)(pkt->common_header.pv.longitude)+2,(char *)(PV->longitude) +1,1);
		memcpy((char *)(pkt->common_header.pv.longitude)+1,(char *)(PV->longitude)+2,1);memcpy(pkt->common_header.pv.longitude,(char *)(PV->longitude) +3,1);
		memcpy((char *)(pkt->common_header.pv.time_stamp)+3,PV->time_stamp,1);memcpy((char *)(pkt->common_header.pv.time_stamp)+2,(char *)(PV->time_stamp) +1,1);
		memcpy((char *)(pkt->common_header.pv.time_stamp)+1,(char *)(PV->time_stamp)+2,1);memcpy(pkt->common_header.pv.time_stamp,(char *)(PV->time_stamp) +3,1);
	 **///memcpy(pkt->common_header.payload_lenght,LEN+1,1);memcpy(pkt->common_header.payload_lenght+1,LEN,1);
	//}
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+7,1);
	print_hex_data(pkt->common_header.hop_limit,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+36,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;
	free(PV);PV=NULL;
	return(pkt);
}
itsnet_packet_f * GeoUnicast_f(void *buffer){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	//PRF("xa estou dentro de GeoUnicast_f \n");
	memcpy(&pkt->common_header.btp,(char *)(buffer),1);
	itsnet_position_vector * PV=NULL;
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +5,1);memcpy(LEN+1,(char *)(buffer) +4,1);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+7,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+68+28,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;
	//print_hex_data(LEN,2);
	//PRF("medidas %d",lon_int);
	free(PV);PV=NULL;
	return(pkt);
}


itsnet_packet_f * GeoBroadcast_f(void *buffer,bool g){
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_geo_t)+ sizeof(itsnet_common));
	//PRF("xa estou dentro de geo_f \n");
	memcpy(&pkt->common_header.btp,(char *)(buffer),1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +8,28);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2] ;
	memcpy(LEN,(char *)(buffer) +5,1);memcpy(LEN+1,(char *)(buffer) +4,1);
	int lon_int=sprint_hex_data( LEN, 2);

	if (g==false){
		memcpy(pkt->common_header.payload_lenght,LEN,2);
		memcpy(pkt->payload.itsnet_geocast.angle,(char *)(buffer)+80,2);
		memcpy(pkt->payload.itsnet_geocast.distanceA,(char *)(buffer)+76,2);
		memcpy(pkt->payload.itsnet_geocast.distanceB,(char *)(buffer)+78,2);
		memcpy(pkt->payload.itsnet_geocast.repetitionInterval,(char *)(buffer)+8,4);
		memcpy(pkt->payload.itsnet_geocast.lt,(char *)(buffer)+38,4);

		//	pkt->common_header.longitude=PV->longitude;
		//	pkt->common_header.latitude=PV->latitude;
	}
	else{

		memcpy((char *)(pkt->common_header.payload_lenght)+1,LEN,1);memcpy(pkt->common_header.payload_lenght,LEN+1,1);
		memcpy(pkt->payload.itsnet_geocast.angle + 1,(char *)(buffer)+80,1);memcpy(pkt->payload.itsnet_geocast.angle,(char *)(buffer)+81,1);
		memcpy(pkt->payload.itsnet_geocast.distanceA + 1,(char *)(buffer)+76,1);memcpy(pkt->payload.itsnet_geocast.distanceA,(char *)(buffer)+77,1);
		memcpy(pkt->payload.itsnet_geocast.distanceB + 1,(char *)(buffer)+78,1);memcpy(pkt->payload.itsnet_geocast.distanceB,(char *)(buffer)+79,1);
		memcpy(pkt->payload.itsnet_geocast.repetitionInterval,(char *)(buffer)+11,1);memcpy(pkt->payload.itsnet_geocast.repetitionInterval+1,(char *)(buffer)+10,1);
		memcpy(pkt->payload.itsnet_geocast.repetitionInterval+2,(char *)(buffer)+9,1);memcpy(pkt->payload.itsnet_geocast.repetitionInterval+3,(char *)(buffer)+8,1);
		memcpy(pkt->payload.itsnet_geocast.lt,(char *)(buffer)+41,1);memcpy(pkt->payload.itsnet_geocast.lt+1,(char *)(buffer)+40,1);
		memcpy(pkt->payload.itsnet_geocast.lt+2,(char *)(buffer)+39,1);memcpy(pkt->payload.itsnet_geocast.lt+3,(char *)(buffer)+38,1);
		/**	memcpy((char *)(pkt->common_header.pv.speed)+1,PV->speed,1);memcpy(pkt->common_header.pv.speed,(char *)(PV->speed) +1,1);
				memcpy((char *)(pkt->common_header.pv.heading)+1,PV->heading,1);memcpy(pkt->common_header.pv.heading,(char *)(PV->heading) +1,1);
				memcpy((char *)(pkt->common_header.pv.altitude)+1,PV->altitude,1);memcpy(pkt->common_header.pv.altitude,(char *)(PV->altitude) +1,1);
				memcpy((char *)(&pkt->common_header.pv.accuracy)+1,&PV->accuracy,1);memcpy(&pkt->common_header.pv.accuracy,(char *)(&PV->accuracy) +1,1);
				memcpy((char *)(pkt->common_header.pv.latitude)+3,PV->latitude,1);memcpy((char *)(pkt->common_header.pv.latitude)+2,(char *)(PV->latitude) +1,1);
				memcpy((char *)(pkt->common_header.pv.latitude)+1,(char *)(PV->latitude)+2,1);memcpy(pkt->common_header.pv.latitude,(char *)(PV->latitude) +3,1);
				memcpy((char *)(pkt->common_header.pv.longitude)+3,PV->longitude,1);memcpy((char *)(pkt->common_header.pv.longitude)+2,(char *)(PV->longitude) +1,1);
				memcpy((char *)(pkt->common_header.pv.longitude)+1,(char *)(PV->longitude)+2,1);memcpy(pkt->common_header.pv.longitude,(char *)(PV->longitude) +3,1);
				memcpy((char *)(pkt->common_header.pv.time_stamp)+3,PV->time_stamp,1);memcpy((char *)(pkt->common_header.pv.time_stamp)+2,(char *)(PV->time_stamp) +1,1);
				memcpy((char *)(pkt->common_header.pv.time_stamp)+1,(char *)(PV->time_stamp)+2,1);memcpy(pkt->common_header.pv.time_stamp,(char *)(PV->time_stamp) +3,1);
		 **/
	}

	memcpy(pkt->payload.itsnet_geocast.payload,(char *)(buffer)+84,lon_int+4);
	memcpy(pkt->payload.itsnet_geocast.reserved,(char *)(buffer)+2,1);	//hai que engadir o reservado. suponse q son 2 bytes
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;
	memcpy(pkt->common_header.flags,(char *)(buffer) +3,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+7,1);

	return(pkt);

}
int geo_limit(void *HT,itsnet_packet_f *pkt)
{
	int x,y,r,total,a,b;
	//printf("latitude lonxitude %d %d %d %d",pkt->common_header.pv.latitude,  LPV->latitude,pkt->common_header.pv.longitude,LPV->longitude);
	x=abs(pkt->common_header.pv.latitude - LPV->latitude);
	y=abs(pkt->common_header.pv.longitude - LPV->longitude);
	char distA[2];
	char distB[2];
	memcpy(distA,pkt->payload.itsnet_geocast.distanceA+1,1);memcpy(distA+1,pkt->payload.itsnet_geocast.distanceA,1);
	memcpy(distB,pkt->payload.itsnet_geocast.distanceB+1,1);memcpy(distB+1,pkt->payload.itsnet_geocast.distanceB,1);
	if(memcmp(geobroad0,HT,1)==0){
		PRF("circular \n");print_hex_data(distA,2);
		r=sprint_hex_data(distA, 2)*100;//PRF("ben %d %d %d\n",x,y,r);
		total= 1- pow((x/r),2) - pow((y/r),2);

	}else
		if(memcmp(geobroad1,HT,1)==0){
			a=sprint_hex_data(distA, 2)*100;
			b=sprint_hex_data(distB, 2)*100;
			total= fmin(1- pow((x/a),2) ,1- pow((y/b),2));
			PRF("rectangular \n");
		} else
			if(memcmp(geobroad2,HT,1)==0){
				a=sprint_hex_data(distA, 2)*100;
				b=sprint_hex_data(distB, 2)*100;
				total= 1- pow((x/a),2) - pow((y/b),2);
				PRF("eliptica \n");
			}
	//free(pkt1);pkt1=NULL;
	return(total);
}
