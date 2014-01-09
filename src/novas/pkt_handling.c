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
extern const unsigned char ZEROS[ETH_ALEN];
extern const unsigned char TWOS[ETH_ALEN];
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
extern ev_timer t_Beacon;
extern unsigned short dictionary[16];
extern List_locT *locT_general;
extern List_lsp * lsp_bc_g;
extern List_lsp *ls_buffer;
extern List_lsp * lsp_uc_g;
int LS_num=0;
extern time_t PDR_ini;
extern time_t PDR;
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
const unsigned char BROADCAST[6]={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };

itsnet_packet * TSB(void *buffer, List_lsp *lsp, List_lsp *rep){
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( (char *)(buffer) +4, 2);
	memcpy(&ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.mhl,(char *)(buffer) +2,1);
	int hl_int=sprint_hex_data( (char *)(buffer) +2, 1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=1;
	memset(ch.reserved,0,1);
	memcpy(&ch.nh_reserved,(char *)(buffer) +7,1);
	ch.nh_reserved.HT=ch.nh_reserved.HST;
	ch.nh_reserved.HST=0;
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	char str3[9] = {'\0'};
	sprintf(str3,"%08X",hl_int-1);
	memcpy(bh.rhl,(void *)str3,1);
	itsnet_tsb_t tsb_h;
	tsb_h.sequencenumber=SN_g;
	//PRF("SN %d\n",tsb_h.sequencenumber);
	SN_g++; //máximo SN?? % SN_MAX;
	tsb_h.source_position_vector=* LPV;
	memset(tsb_h.reserved,0,2);
	memcpy(tsb_h.payload.payload,(char *)(buffer) +20,lon_int);
	pkt->payload.itsnet_tsb=tsb_h;//print_hex_data(pkt->payload.itsnet_tsb.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_tsb.payload.btp2,(char *)(buffer) + 16,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 18,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);}
	if  ((locT_general->len== 0 || any_neighbours()==0)&& ch.traffic_class.scf==1){
		itsnet_packet * pkt1 = NULL;
		int val=lsp_bc_g->size+8+4+28+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff,0);
			val=lsp_bc_g->size+4+8+28+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
		int i =add_end_lsp(lsp_bc_g, *pkt,0);free(pkt);
		return(pkt1);
		//buffer in BC AND omit next executions
	}else if  (any_neighbours()==0&& ch.traffic_class.scf==0){return(pkt);}

	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,buffer +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	//PRF("saio de tsb\n");
	free(lt);return(pkt);
}

itsnet_packet * SHB(void *buffer, List_lsp *lsp, List_lsp *rep){
	PRF("SHB ");
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( (char *)(buffer) +4, 2);
	memcpy(&ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.mhl,(char *)(buffer) +2,1);
	int hl_int=sprint_hex_data( (char *)(buffer) +2, 1);
	ch.HT_HST.HT=5;
	ch.HT_HST.HST=0;
	memset(ch.reserved,0,1);
	memcpy(&ch.nh_reserved,(char *)(buffer) +7,1);
	ch.nh_reserved.HT=ch.nh_reserved.HST;
	ch.nh_reserved.HST=0;
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	char str3[9] = {'\0'};
	sprintf(str3,"%08X",hl_int-1);
	memcpy(bh.rhl,(void *)str3,1);
	itsnet_shb_t shb_h;
	//tsb_h.sequencenumber=SN_g;	//PRF("SN %d\n",tsb_h.sequencenumber);
	SN_g++; //máximo SN?? % SN_MAX;
	shb_h.source_position_vector=* LPV;
	memset(shb_h.reserved,0,4);
	memcpy(shb_h.payload.payload,(char *)(buffer) +20,lon_int);
	pkt->payload.itsnet_shb=shb_h;//print_hex_data(pkt->payload.itsnet_shb.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
	memcpy(pkt->payload.itsnet_shb.payload.btp2,(char *)(buffer) + 16,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_shb.payload.btp1,(char *)(buffer) + 18,2);
		memcpy(pkt->payload.itsnet_shb.payload.btp2,info_dest,2);		}
	if  ((locT_general->len== 0 || any_neighbours()==0)&& ch.traffic_class.scf==1){
		itsnet_packet * pkt1 = NULL;
		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		int val=lsp_bc_g->size+4+8+28+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff,0);
			val=lsp_bc_g->size+8+4+28+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}		int i =add_end_lsp(lsp_bc_g, *pkt,0);free(pkt);
		return(pkt1);		//buffer in BC AND omit next executions
	}

	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer) +8,4);
	if(atoi(REP)==0){
		//add_end_rep(rep_bc_g, pkt);
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	free(lt);return(pkt);}
itsnet_packet * GeoUnicast(void *buffer, List_lsp *lsp, List_lsp *rep,public_ev_arg_r *arg){

	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( (char *)(buffer) +4, 2);
	memcpy(&ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.mhl,(char *)(buffer) +2,1);
	int hl_int=sprint_hex_data( (char *)(buffer) +2, 1);
	ch.HT_HST.HT=2;
	ch.HT_HST.HST=0;
	memset(ch.reserved,0,1);
	memcpy(&ch.nh_reserved,(char *)(buffer) +7,1);
	ch.nh_reserved.HT=ch.nh_reserved.HST;
	ch.nh_reserved.HST=0;
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);/**
	 * @fn geo_limit
	 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
	 * @param HT, to now if it's circular(0), rectangular(1) or elliptical(2).
	 * @param pkt,packet we will send to the upper layer if we are into the desired area
	 * @return if it is <0 it is out of area.
	 */
	int geo_limit(void *HT,itsnet_packet_f *pkt);


	/**
	 * @fn geo_limit_ll
	 * @brief geo_limit. This function makes the calculate to know if it is into the geographical limits
	 * @param pkt, packet we will send to link layer if we are into the desired area
	 * @return if it is <0 it is out of area.
	 */


	int geo_limit_ll(itsnet_packet *pkt);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	char str3[9] = {'\0'};
	sprintf(str3,"%08X",hl_int-1);
	memcpy(bh.rhl,(void *)str3,1);
	itsnet_unicast_t un_h;
	un_h.sequencenumber=SN_g++;
	//SN_g++; //máximo SN?? % SN_MAX;
	un_h.source_position_vector=* LPV;
	itsnet_position_short_vector *LPV_dest=(itsnet_position_short_vector *)malloc(sizeof(itsnet_position_short_vector));
	memcpy(&LPV_dest->node_id,(char *)(buffer)+16,8);
	un_h.dest_position_vector=* LPV_dest;
	memset(un_h.reserved,0,2);
	memcpy(un_h.payload.payload,(char *)(buffer) +28,lon_int);
	pkt->payload.itsnet_unicast=un_h;//print_hex_data(pkt->payload.itsnet_tsb.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 26,2);
	memcpy(pkt->payload.itsnet_tsb.payload.btp2,(char *)(buffer) + 24,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp1,(char *)(buffer) + 26,2);
		memcpy(pkt->payload.itsnet_tsb.payload.btp2,info_dest,2);}

	int val=search_in_locT_m_wo_n(LPV_dest->node_id.mac,locT_general);
	if(val!=0){
		sup_timer(dictionary[val],1);
		AddTimer(dictionary[val],itsGnLifetimeLocTE,1);
		itsnet_packet * pkt_lsp = NULL;
		//engadir a loct con LS_pending.
		if  (search_in_locT_m_pending (LPV_dest->node_id.mac,locT_general)!=0 ){  //&& ch.traffic_class.scf==1
			itsnet_packet * pkt1 = NULL;
			int val=ls_buffer->size+8+4+52+4+sprint_hex_data((char *)(buffer) +4,2);
			//delete old buffered elements if we need more size to add a new one.
			while (val>itsGnBcForwardingPacketBufferSize){
				ls_buffer=sup_elem_lsp(0xffff,3);
				val=ls_buffer->size+4+8+52+4+sprint_hex_data((char *)(buffer) +4,2);
				PRF("aqui podo liala porque non se actualice ls_buffer");
			}
			int i =add_end_lsp(ls_buffer, *pkt,3);free(pkt);
			return(pkt1);
		}else{
			int pos_loct=search_in_locT_m(LPV_dest->node_id.mac,locT_general);
			if ( pos_loct==0 && ch.traffic_class.scf==1){
				int val=lsp_uc_g->size+8+4+52+4+sprint_hex_data((char *)(buffer) +4,2);
				//delete old buffered elements if we need more size to add a new one.
				while (val>itsGnBcForwardingPacketBufferSize){
					lsp_uc_g=sup_elem_lsp(0xffff,4);
					val=lsp_uc_g->size+4+8+52+4+sprint_hex_data((char *)(buffer) +4,2);
					PRF("aqui podo liala porque non se actualice lsp_uc_g a tempo");
				}
				int i =add_end_lsp(lsp_uc_g, *pkt,4);free(pkt);
				return(pkt_lsp);
				//buffer in BC AND omit next executions
			} else if (pos_loct==0 &&ch.traffic_class.scf==0){

				return(pkt);
				//broadcast, no more
			} else if(pos_loct!=0) {
				//envío cara abaixo da guc, quizás debería facelo desde aqui e sacar un NULL a fora para facer a diferencia co caso broadcast
				int same=0;
				int cont=0;
				Element_locT *position=locT_general->init;
				while(position!=NULL && same==0){if(cont==pos_loct)same=1; else {cont++;position=position->next;}}
				memcpy(&un_h.dest_position_vector,position,20);
				pkt->payload.itsnet_unicast=un_h;

				if(itsGnGeoUnicastForwardingAlgorithm==0||itsGnGeoUnicastForwardingAlgorithm==1){
					mac_addr nh= Greedy_Forwarding_UC(&position->data.pos_vector);
					if (memcmp(nh.address,TWOS,6)==0 ||memcmp(nh.address,ZEROS,6)==0){return(pkt_lsp);}else{
						char h_source[ETH_ALEN];
						sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
						get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
						ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
						char type[2]={0x07,0x07};
						memcpy(tx_frame->buffer.header.type,type,2);
						memcpy(tx_frame->buffer.data, pkt, IEEE_80211_BLEN);
						send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,lon_int+ 48+4+8+14+4);free(tx_frame);return(pkt_lsp);
					}				}
				else if(itsGnGeoUnicastForwardingAlgorithm==2){
					CBF_UC(pkt,lon_int+ 48+4+8+14+4,&position->data.pos_vector);
					return(pkt_lsp);
				}			}}
	}else{
		PRF("LOCATION SERVICE\n");
		if (itsGnSecurity==1){PRF("Security en ls\n");}
		itsnet_packet * pkt_ls = NULL;
		pkt_ls=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		pkt_ls->basic_header=bh;
		ch.HT_HST.HT=6;
		ch.HT_HST.HST=0;
		pkt_ls->common_header=ch;
		itsnet_ls_req_t ls_req;
		memcpy(&ls_req.GN_ADDR,(char *)(buffer)+16,8);
		ls_req.source_position_vector=* LPV;
		ls_req.sequencenumber=SN_g++;
		if (memcmp((char *)(buffer) +7,tipoa,1)==0)
		{		memcpy(ls_req.payload.btp1,(char *)(buffer) + 26,2);
		memcpy(ls_req.payload.btp2,(char *)(buffer) + 24,2);
		}else{
			char info_dest[2];
			memset(info_dest,0,2);
			memcpy(ls_req.payload.btp1,(char *)(buffer) + 26,2);
			memcpy(ls_req.payload.btp2,info_dest,2);}
		pkt_ls->payload.itsnet_ls_req=ls_req;
		itsnet_node * data=NULL;//
		data= (itsnet_node *)malloc(sizeof(itsnet_node));
		memcpy(data->mac_id.address,buffer+6,6);
		data->IS_NEIGHBOUR=false;
		itsnet_time_stamp tst=data->pos_vector.time_stamp;
		data->tst.tv_sec= tst;
		char VERSION[1];
		memcpy(VERSION,buffer+14,1);
		ht_hst_t *conv=(ht_hst_t *)VERSION;
		data->version=conv->HT;
		data->pdr= PDR;
		data->LS_PENDING=true;
		data->SN1=ls_req.sequencenumber;
		add_end_locT(locT_general,*data);
		//ls retransmisor
		AddTimer(ls_req.sequencenumber,itsGnLocationServiceRetransmitTimer,2);//antes poñía ls_req.sequencenumber pero penso que é mellor que leve o sn do unicast, q non cambia con cada paso

		int val=ls_buffer->size+8+4+52+4+sprint_hex_data((char *)(pkt_ls->common_header.payload_lenght),2);
		while (val>itsGnLocationServicePacketBufferSize){
			ls_buffer=sup_elem_lsp(0xffff,3);
			val=ls_buffer->size+4+8+52+4+sprint_hex_data((char *)(pkt_ls->common_header.payload_lenght),2);
			PRF("aqui podo liala porque non se actualice ls_buffer");
		}
		add_end_lsp(ls_buffer, *pkt_ls,3);
		int val1=ls_buffer->size+8+4+52+4+sprint_hex_data((char *)(pkt->common_header.payload_lenght),2);
		//delete old buffered elements if we need more size to add a new one.
		while (val1>itsGnLocationServicePacketBufferSize){
			ls_buffer=sup_elem_lsp(0xffff,3);
			val1=ls_buffer->size+4+8+52+4+sprint_hex_data((char *)(pkt->common_header.payload_lenght),2);
			PRF("aqui podo liala porque non se actualice ls_buffer");
		}		add_end_lsp(ls_buffer, *pkt,3);	}
	free(LPV_dest);
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,buffer +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	//PRF("saio de tsb\n");
	free(lt);return(pkt);

}
itsnet_packet * GeoBroadcast(void *buffer, List_lsp *lsp, List_lsp *rep,public_ev_arg_r *arg){
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( (char *)(buffer) +4, 2);
	memcpy(&ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.mhl,(char *)(buffer) +2,1);
	int hl_int=sprint_hex_data( (char *)(buffer) +2, 1);
	memcpy(&ch.HT_HST,(char *)(buffer),1);
	memset(ch.reserved,0,1);
	memcpy(&ch.nh_reserved,(char *)(buffer) +7,1);
	ch.nh_reserved.HT=ch.nh_reserved.HST;
	ch.nh_reserved.HST=0;
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	char str3[9] = {'\0'};
	sprintf(str3,"%08X",hl_int-1);
	memcpy(bh.rhl,(void *)str3,1);
	itsnet_geobroadcast_t gbc_h;
	gbc_h.sequencenumber=SN_g;
	SN_g++; //máximo SN?? % SN_MAX;
	gbc_h.source_position_vector=* LPV;
	itsnet_packet * pkt1 = NULL;
	memset(gbc_h.reserved,0,2);
	memcpy(gbc_h.dest_latitude,(char *)(buffer) +16,4);
	memcpy(gbc_h.dest_longitude,(char *)(buffer) +20,4);
	memcpy(gbc_h.distanceA,(char *)(buffer) +24,2);
	memcpy(gbc_h.distanceB,(char *)(buffer) +26,2);
	memcpy(gbc_h.angle,(char *)(buffer) +28,2);
	memcpy(gbc_h.reserved,(char *)(buffer) +30,1);
	memcpy(gbc_h.payload.payload,(char *)(buffer) +36,lon_int);
	pkt->payload.itsnet_geobroadcast=gbc_h;//print_hex_data(pkt->payload.itsnet_geobroadcast.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer) + 34,2);
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,(char *)(buffer) + 32,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer) + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);		}
	if  ((locT_general->len== 0 || any_neighbours()==0) && ch.traffic_class.scf==1){

		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		int val=lsp_bc_g->size+8+4+48+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff,0);
			val=lsp_bc_g->size+8+4+48+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
		int i =add_end_lsp(lsp_bc_g, *pkt,0);free(pkt);
		return(pkt1);		//buffer in BC AND omit next executions
	}
	mac_addr nh;
	if(itsGnGeoBroadcastForwardingAlgorithm==0||itsGnGeoBroadcastForwardingAlgorithm==1){
		memcpy(nh.address,BROADCAST,6); //first send is broadcast
		/**int F=Distance(LPV,lpv_dest);

		if(F>=0) memcpy(nh.address,BROADCAST,6);else{
		*nh= Greedy_Forwarding_UC(lpv_dest);
		if (memcmp(nh.address,TWOS,6)==0 ||memcmp(nh.address,ZEROS,6)==0){return(pkt1);}else{**/
			char h_source[ETH_ALEN];
			sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
			get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
			ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
			char type[2]={0x07,0x07};
			memcpy(tx_frame->buffer.header.type,type,2);
			memcpy(tx_frame->buffer.data, pkt, IEEE_80211_BLEN);
			send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,lon_int+ 44+4+8+14+4);free(tx_frame);return(pkt1);
				}
	else if(itsGnGeoBroadcastForwardingAlgorithm==2){
		CBF_BC(pkt,lon_int+ 44+4+8+14+4,LPV); //first send PV_SE is LPV
		return(pkt1);
	}	else if(itsGnGeoBroadcastForwardingAlgorithm==3){}
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer)  +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	free(lt);return(pkt);}

itsnet_packet *  GeoAnycast(void *buffer, List_lsp *lsp, List_lsp *rep){
	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(ch.payload_lenght,(char *)(buffer) +4,2);
	int lon_int=sprint_hex_data( (char *)(buffer) +4, 2);
	memcpy(&ch.traffic_class,(char *)(buffer) +6,1);
	memcpy(ch.flags,(char *)(buffer) +3,1);
	memcpy(ch.mhl,(char *)(buffer) +2,1);
	int hl_int=sprint_hex_data( (char *)(buffer) +2, 1);
	memcpy(&ch.HT_HST,(char *)(buffer),1);
	memset(ch.reserved,0,1);
	memcpy(&ch.nh_reserved,(char *)(buffer) +7,1);
	ch.nh_reserved.HT=ch.nh_reserved.HST;
	ch.nh_reserved.HST=0;
	char TS[4];
	memcpy(TS,(char *)(buffer) +12,4);
	int ts_num=sprint_hex_data(TS,4);int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	char str3[9] = {'\0'};
	sprintf(str3,"%08X",hl_int-1);
	memcpy(bh.rhl,(void *)str3,1);
	itsnet_geobroadcast_t gbc_h;
	gbc_h.sequencenumber=SN_g;
	SN_g++; //máximo SN?? % SN_MAX;
	gbc_h.source_position_vector=* LPV;
	memset(gbc_h.reserved,0,2);
	memcpy(gbc_h.dest_latitude,(char *)(buffer) +16,4);
	memcpy(gbc_h.dest_longitude,(char *)(buffer) +20,4);
	memcpy(gbc_h.distanceA,(char *)(buffer) +24,2);
	memcpy(gbc_h.distanceB,(char *)(buffer) +26,2);
	memcpy(gbc_h.angle,(char *)(buffer) +28,2);
	memcpy(gbc_h.reserved,(char *)(buffer) +30,1);
	memcpy(gbc_h.payload.payload,(char *)(buffer) +36,lon_int);
	pkt->payload.itsnet_geobroadcast=gbc_h;//print_hex_data(pkt->payload.itsnet_geobroadcast.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	//NEIGHBOURS.
	if (memcmp((char *)(buffer) +7,tipoa,1)==0)
	{		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer) + 34,2);
	memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,(char *)(buffer) + 32,2);
	}else{
		char info_dest[2];
		memset(info_dest,0,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp1,(char *)(buffer) + 34,2);
		memcpy(pkt->payload.itsnet_geobroadcast.payload.btp2,info_dest,2);		}
	if  ((locT_general->len== 0 || any_neighbours()==0) && ch.traffic_class.scf==1){
		itsnet_packet * pkt1 = NULL;
		//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
		int val=lsp_bc_g->size+8+4+48+4+sprint_hex_data((char *)(buffer) +4,2);
		//delete old buffered elements if we need more size to add a new one.
		while (val>itsGnBcForwardingPacketBufferSize){
			lsp_bc_g=sup_elem_lsp(0xffff,0);
			val=lsp_bc_g->size+8+4+48+4+sprint_hex_data((char *)(buffer) +4,2);
			PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
		}
		int i =add_end_lsp(lsp_bc_g, *pkt,0);free(pkt);
		return(pkt1);		//buffer in BC AND omit next executions
	}
	//REPETITION INTERVAL
	char REP[4];
	memcpy(REP,(char *)(buffer)  +8,4);
	if(atoi(REP)==0){
		//GARDAR O PAQUETE
		//RTX THE PACKET WITH PERIOD SPECIFIED IN REP UNTIL HL.
	}
	free(lt);return(pkt);}


int BasicHeader_processing(public_ev_arg_r *arg){

	char buffer[arg->len];
	memcpy(buffer,arg->data,arg->len);
	ht_hst_t VERSION;
	memcpy(&VERSION,buffer+14,1);
	int error=0;
	if (VERSION.HT!=itsGnProtocolVersion){return(1);}
	if (itsIfType==1){PRF("Media-dependent funcionalities for ITS-G5\n");}
	if (VERSION.HST!=2){error =CommonHeader_processing(arg);}else{ PRF("SECURE!\n");}
	return(error);

}
int CommonHeader_processing(public_ev_arg_r *arg){
	////this is the first thing we must do after reception of a GN pkt.
	PRF("ENTRO EN COMMON HEADER\n");
	char buffer[arg->len];
	memcpy(buffer,arg->data,arg->len);
	char HL[1];char HL2[1];char HT1[1];
	memcpy(HL,buffer+14+10,1);
	int mhl=sprint_hex_data( HL, 1);
	memcpy(HL2,buffer+14+3,1);
	int rhl=sprint_hex_data( HL2, 1);
	if (rhl>mhl){return(1);}
	memcpy(HT1,(char *)(buffer)+14+1+4,1);
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	if(memcmp(HT1,beacon,1)==0 ||memcmp(HT1,tsb0,1)==0){ //aquí xa están renombrados tsb0 e tsb1
		memcpy( PV,buffer +8+14+4,24);}else {memcpy( PV,buffer +8+14+4+4,24);}
	flags_t * FLAG=NULL;//
	FLAG= (flags_t *)malloc(sizeof(flags_t));
	memcpy( FLAG,buffer +3+14+4,1);
	itsnet_node * data=NULL;//
	data= (itsnet_node *)malloc(sizeof(itsnet_node));
	memcpy(data->mac_id.address,(char *)(buffer)+6,6);
	if (memcmp(HL,tsb0,1)==0 ||memcmp(HL,beacon,1)==0) data->IS_NEIGHBOUR=true;else data->IS_NEIGHBOUR=false; //seguro????????????si
	data->pos_vector= * PV;
	itsnet_time_stamp tst=data->pos_vector.time_stamp;
	free(PV);PV=NULL;
	data->tst.tv_sec= tst;
	char VERSION[1];
	memcpy(VERSION,buffer+14,1);
	ht_hst_t *conv=(ht_hst_t *)VERSION;
	data->version=conv->HT; //comprobar si es correcto
	data->itss_type=FLAG->itsStation;
	data->tpdr= PDR;
	data->pdr=1/(PDR-PDR_ini);
	//data->pdr
	free(FLAG);FLAG=NULL;
	uint16_t lon_int;
	if(memcmp(HT1,beacon,1)!=0 && (memcmp(HT1,tsb0,1)!=0) ){
		char SN[2];
		memcpy(SN,arg->data+8+14+4,2);
		lon_int=sprint_hex_data( SN, 2);//PRF(" sequence number %d\n", lon_int);
		data->Sequence_number=lon_int;}else{data->Sequence_number=0;
		//dará problemas igualar esto a cero??
		}
	data->LS_PENDING=false;
	char DE_LPV[20];
	memcpy(DE_LPV,buffer+arg->len-20,20);
	char h_source[ETH_ALEN];
	ht_hst_t *NH;
	memcpy(NH,buffer+14,1);
	get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	if(memcmp(HT1,geounicast,1)==0 && memcmp(DE_LPV+2,h_source,6)!=0){
		if(NH->HST==0 ||NH->HST==1 ){
			itsnet_node * data1=NULL;//
			data1= (itsnet_node *)malloc(sizeof(itsnet_node));
			memcpy(&data1->pos_vector,DE_LPV,20);
			memcpy(data1->mac_id.address,data1->pos_vector.node_id.mac.address,6);
			int val0=search_in_locT(data1,locT_general);
			int val01=search_in_locT_m(data1->mac_id,locT_general);
			if(val0==0){add_end_locT(locT_general,*data1);}else if(val01==0){
				int in=0;
				Element_locT *position=locT_general->init;
				while (in<(val0))
				{in++;position = position->next;//print_hex_data(position->data.mac_id.address,6);PRF(" \n");
				}
				memset(&position->data.pos_vector,0,24);
				memcpy(&position->data.pos_vector,DE_LPV,20);	}		}	}
	int val=search_in_locT(data,locT_general);
	if(val==0){locT_general=add_end_locT (locT_general,*data);}else{
		locT_general=mod_t_locT (val,locT_general,*data,lon_int,tst);
		sup_timer(dictionary[val],1);//eliminamos anterior temp e poñemos o novo
		AddTimer(dictionary[val],itsGnLifetimeLocTE,1);}

	int a= search_in_locT_m_pending(data->mac_id,locT_general);
	if (a!=0){
		Element_locT *position=locT_general->init;
		int in=0;
		while (in<(a))
		{in++;position = position->next;//print_hex_data(position->data.mac_id.address,6);PRF(" \n");
		}
		position->data.LS_PENDING=false;
		Element_lsp *pos1=ls_buffer->init;
		while(pos1!=NULL){
			if(memcmp((char *)&pos1->data.common_header.HT_HST,geounicast,1)==0){
				if(memcmp(pos1->data.payload.itsnet_unicast.dest_position_vector.node_id.mac.address,data->mac_id.address,6)==0){
					tTimer * temp;
					int lon_int=sprint_hex_data(pos1->data.basic_header.rhl, 1);
					int size=sprint_hex_data(pos1->data.common_header.payload_lenght, 2);
					if (lon_int==1){memset(pos1->data.basic_header.rhl,0,1);} else {memset(pos1->data.basic_header.rhl,lon_int-1,1);}
					uint16_t sn=0xffff;int base=0;int mult=0;LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};	int num3=0;int num4=0;
					sn = pos1->data.payload.itsnet_unicast.sequencenumber;
					temp=FindTimer(sn,3);
					if (temp->Period>640){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult= (int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
					sprintf(str2, "%04X",mult);
					sprintf(str1, "%01X",base);
					num4=strtol(str2,NULL,16);//PRF("%d\n", num4);
					num3=strtol(str1,NULL,16);//PRF("%d\n", num3);
					lt->base=num3;lt->multiple=num4;
					memcpy(pos1->data.basic_header.lt,(void *)lt,1);
					pos1->data.payload.itsnet_unicast.source_position_vector=* LPV;
					free(lt);lt=NULL;
					char h_source[ETH_ALEN];
					sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
					get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
					ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, BROADCAST,h_source);
					char type[2]={0x07,0x07};
					memcpy(tx_frame->buffer.header.type,type,2);
					memcpy(tx_frame->buffer.data, &pos1->data, IEEE_80211_BLEN);

					send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,48+ size+4+8+14+4);//==-1){}
					print_hex_data(&tx_frame->buffer,52+ size+4+8);
					PRF(" paquete enviado a ll despois de ls_buffer \n");free(tx_frame);free(dir);

					ls_buffer=sup_elem_lsp(sn,3);}
			}else if(memcmp((char *)&pos1->data.common_header.HT_HST,ls0,1)==0){
				if(memcmp(pos1->data.payload.itsnet_ls_req.GN_ADDR.mac.address,data->mac_id.address,6)==0){
					int sn = pos1->data.payload.itsnet_unicast.sequencenumber;
					ls_buffer=sup_elem_lsp(sn,3);
				}}
			pos1=pos1->next;
		}
	}
	if(lsp_uc_g->len>0){
		Element_lsp *pos0=lsp_uc_g->init;
		while(pos0!=NULL)
			if(memcmp((char *)&pos0->data.common_header.HT_HST,geounicast,1)==0){
				if(memcmp(pos0->data.payload.itsnet_unicast.dest_position_vector.node_id.mac.address,data->mac_id.address,6)==0){
					tTimer * temp;
					int lon_int=sprint_hex_data(pos0->data.basic_header.rhl, 1);
					int size=sprint_hex_data(pos0->data.common_header.payload_lenght, 2);
					if (lon_int==1){memset(pos0->data.basic_header.rhl,0,1);} else {memset(pos0->data.basic_header.rhl,lon_int-1,1);}
					uint16_t sn=0xffff;int base=0;int mult=0;LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};	int num3=0;int num4=0;
					sn = pos0->data.payload.itsnet_unicast.sequencenumber;
					temp=FindTimer(sn,4);
					if (temp->Period>640){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult= (int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
					sprintf(str2, "%04X",mult);
					sprintf(str1, "%01X",base);
					num4=strtol(str2,NULL,16);//PRF("%d\n", num4);
					num3=strtol(str1,NULL,16);//PRF("%d\n", num3);
					lt->base=num3;lt->multiple=num4;
					memcpy(pos0->data.basic_header.lt,(void *)lt,1);
					pos0->data.payload.itsnet_unicast.source_position_vector=* LPV;

					free(lt);lt=NULL;
					char h_source[ETH_ALEN];
					sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
					get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
					ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, data->mac_id.address,h_source);
					char type[2]={0x07,0x07};
					memcpy(tx_frame->buffer.header.type,type,2);
					memcpy(tx_frame->buffer.data, &pos0->data, IEEE_80211_BLEN);

					send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,48+ size+4+8+14+4);//==-1){}
					PRF(" paquete enviado a ll despois de ls_buffer \n");free(tx_frame);free(dir);

					lsp_uc_g=sup_elem_t_lsp(sn,4); //igual teño que cambiar esto se pode haber uc para diferentes MAC, usar sup_t_elem_lsp

				}}
		pos0=pos0->next;
	}

	if(any_neighbours()>0 &&lsp_bc_g->len>0){
		Element_lsp *pos=lsp_bc_g->init;
		while(pos!=NULL){
			tTimer * temp;
			int lon_int=sprint_hex_data(pos->data.basic_header.rhl, 1);
			int size=sprint_hex_data(pos->data.common_header.payload_lenght, 2);
			if (lon_int==1){memset(pos->data.basic_header.rhl,0,1);} else {memset(pos->data.basic_header.rhl,lon_int-1,1);}
			//que pasa cando chega un 0 hop???
			char HT[1];uint16_t sn=0xffff;int base=0;int mult=0;LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};	int num3=0;int num4=0;
			memcpy(HT,&pos->data.common_header.HT_HST,1);
			print_hex_data(HT,1);printf("\n");
			if(memcmp(HT,tsb1,1)==0){
				sn = pos->data.payload.itsnet_tsb.sequencenumber;
				temp=FindTimer(sn,2);PRF("ESTE FINDTIMER\n");
				if (temp->Period>6401){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult=(int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
				sprintf(str2, "%04X",mult);
				sprintf(str1, "%01X",base);
				num4=strtol(str2,NULL,16);
				num3=strtol(str1,NULL,16);
				lt->base=num3;lt->multiple=num4;
				memcpy(pos->data.basic_header.lt,(void *)lt,1);
				pos->data.payload.itsnet_tsb.source_position_vector=* LPV;
			}else if(memcmp(HT,tsb0,1)==0){PRF("seguinte3\n");
			pos->data.payload.itsnet_shb.source_position_vector=* LPV;
			}else if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){
				sn = pos->data.payload.itsnet_geobroadcast.sequencenumber;
				temp=FindTimer(sn,2);PRF("ESTE FINDTIMER geo\n");
				if (temp->Period>640){base=3; mult=(int) ceil(temp->Period/100); } else if (temp->Period>64) {base=2; mult= (int) ceil(temp->Period/10);}else {base=1; mult=temp->Period;}
				sprintf(str2, "%04X",mult);
				sprintf(str1, "%01X",base);
				num4=strtol(str2,NULL,16);//PRF("%d\n", num4);
				num3=strtol(str1,NULL,16);//PRF("%d\n", num3);
				lt->base=num3;lt->multiple=num4;
				memcpy(pos->data.basic_header.lt,(void *)lt,1);
				pos->data.payload.itsnet_geobroadcast.source_position_vector=* LPV;
			}
			free(lt);lt=NULL;
			char h_source[ETH_ALEN];
			sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
			get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
			ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, BROADCAST,h_source);
			char type[2]={0x07,0x07};
			memcpy(tx_frame->buffer.header.type,type,2);
			memcpy(tx_frame->buffer.data, &pos->data, IEEE_80211_BLEN);

			int header_length=0;
			if(memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0){header_length=44;} //cambiar esto por datos novos
			else if(memcmp(HT,tsb0,1)==0){header_length=28;}
			send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,header_length+ size+4+8+14+4);//==-1){}
			print_hex_data(&tx_frame->buffer,header_length+ size+4+8);
			PRF(" paquete enviado a ll despois de lsp \n");free(tx_frame);free(dir);
			if(memcmp(HT,tsb0,1)==0)ev_timer_again (l_Beacon,&t_Beacon);
			lsp_bc_g=sup_elem_lsp(sn,0);
			pos=pos->next;
		}
	}
	free(data);data=NULL;
	return(0);
	//	PRF("saio de common header processing\n");
}

itsnet_packet_f * TSB_f(void *buffer){
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_uni_t)+ sizeof(itsnet_common));
	memcpy(&pkt->common_header.btp,(char *)(buffer)+4,1);
	pkt->common_header.btp.HST=pkt->common_header.btp.HT;PRF("AQI SI\n");
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));PRF("AQI SI\n");
	memcpy(PV,(char *)(buffer) +4+4+8,24);
	memcpy(&pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2];memcpy(LEN,(char *)(buffer) +4+4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.flags,(char *)(buffer) +3+4,1);PRF("AQI SI\n");
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+3,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+4+4+24+8,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1+4,1);PRF("AQI SI\n");
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;free(PV);
	return(pkt);}

itsnet_packet_f * SHB_f(void *buffer){

	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_uni_t)+ sizeof(itsnet_common));
	memcpy(&pkt->common_header.btp,(char *)(buffer)+4,1);
	pkt->common_header.btp.HST=pkt->common_header.btp.HT;
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +4+8,24);
	memcpy(&pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2];memcpy(LEN,(char *)(buffer) +4+4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
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
			 memcpy(pkt->common_header.payload_lenght,LEN+1,1);memcpy(pkt->common_header.payload_lenght+1,LEN,1);
		}**/
	memcpy(pkt->common_header.flags,(char *)(buffer) +3+4,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+3,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+4+4+24+8,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;free(PV);
	return(pkt);
}
itsnet_packet_f * GeoUnicast_f(void *buffer){
	//	Create common header
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_uni_t)+ sizeof(itsnet_common));
	memcpy(&pkt->common_header.btp,(char *)(buffer)+4,1);
	pkt->common_header.btp.HST=pkt->common_header.btp.HT;
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer) +4+4+8+24,24);
	memcpy(&pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2];memcpy(LEN,(char *)(buffer) +4+4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	memcpy(pkt->common_header.flags,(char *)(buffer) +3+4,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+3,1);
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+4+4+24+8,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;free(PV);
	return(pkt);}


itsnet_packet_f * GeoBroadcast_f(void *buffer){
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_geo_t)+ sizeof(itsnet_common));
	memcpy(&pkt->common_header.btp,(char *)(buffer)+4,1);
	pkt->common_header.btp.HST=pkt->common_header.btp.HT;
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer)+ 4+4+8,24);
	memcpy(&pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2];memcpy(LEN,(char *)(buffer) +4+4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	memcpy(pkt->payload.itsnet_geocast.angle,(char *)(buffer)+8+4+4+24+12,2);
	memcpy(pkt->payload.itsnet_geocast.distanceA,(char *)(buffer)+8+4+4+24+8,2);
	memcpy(pkt->payload.itsnet_geocast.distanceB,(char *)(buffer)+8+4+4+24+10,2);
	//		memcpy(pkt->payload.itsnet_geocast.repetitionInterval,(char *)(buffer)+8,4); //non se establece en ningun sitio
	//memcpy(pkt->payload.itsnet_geocast.lt,(char *)(buffer)+38,4); //convertir o valor a 4 bytes outra vez
	memcpy(pkt->common_header.flags,(char *)(buffer) +3+4,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+3,1);   //rhl
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+4+4+24+8+16,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;free(PV);
	return(pkt);

}
itsnet_packet_f * GeoAnycast_f(void *buffer){
	itsnet_packet_f * pkt = NULL;
	pkt=(itsnet_packet_f *)malloc(sizeof(itsnet_geo_t)+ sizeof(itsnet_common));
	memcpy(&pkt->common_header.btp,(char *)(buffer)+4,1);
	pkt->common_header.btp.HST=pkt->common_header.btp.HT;
	itsnet_position_vector * PV=NULL;//
	PV= (itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(PV,(char *)(buffer)+ 4+4+8,24);
	memcpy(&pkt->common_header.traffic_class,(char *)(buffer) +6,1);
	char LEN[2];memcpy(LEN,(char *)(buffer) +4+4,2);
	int lon_int=sprint_hex_data( LEN, 2);
	memcpy(pkt->common_header.payload_lenght,LEN,2);
	pkt->common_header.pv=*PV;
	memcpy(pkt->payload.itsnet_geocast.angle,(char *)(buffer)+8+4+4+24+12,2);
	memcpy(pkt->payload.itsnet_geocast.distanceA,(char *)(buffer)+8+4+4+24+8,2);
	memcpy(pkt->payload.itsnet_geocast.distanceB,(char *)(buffer)+8+4+4+24+10,2);
	//		memcpy(pkt->payload.itsnet_geocast.repetitionInterval,(char *)(buffer)+8,4); //non se establece en ningun sitio
	//memcpy(pkt->payload.itsnet_geocast.lt,(char *)(buffer)+38,4); //convertir o valor a 4 bytes outra vez
	memcpy(pkt->common_header.flags,(char *)(buffer) +3+4,1);
	memcpy(pkt->common_header.hop_limit,(char *)(buffer)+3,1);   //rhl
	memcpy(pkt->payload.itsnet_unicast.payload,(char *)(buffer)+4+4+24+8+16,lon_int+4);
	memcpy(&pkt->common_header.pkt_type,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_type.HST=0;
	memcpy(&pkt->common_header.pkt_stype,(char *)(buffer)+1+4,1);
	pkt->common_header.pkt_stype.HT=pkt->common_header.pkt_stype.HST;
	pkt->common_header.pkt_stype.HST=0;free(PV);
	return(pkt);

}

itsnet_packet * LS_req_f(void *buffer){

	itsnet_packet * pkt = NULL;
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	itsnet_common_header ch;
	itsnet_basic_header bh;
	memcpy(&bh,(char *)(buffer),4);
	memcpy(&ch,(char *)(buffer) +4,8);
	memset(ch.mhl,0,1);
	ch.HT_HST.HT=2;
	ch.HT_HST.HST=1;
	int ts_num=itsGnMaxPacketLifeTime; //tamén podería usar o Default en vez do Max
	int base=0;int mult=0;int num4=0;int num3=0;
	LT_s *lt;lt=(LT_s *)malloc(sizeof(LT_s));char str1[2] = {'\0'};	char str2[6] = {'\0'};
	if (ts_num>6401){base=3; mult=(int) ceil(ts_num/100); } else if (ts_num>64) {base=2; mult=(int) ceil(ts_num/10);}else {base=1; mult=ts_num;}
	sprintf(str2, "%04X",mult);
	sprintf(str1, "%01X",base);
	num4=strtol(str2,NULL,16);
	num3=strtol(str1,NULL,16);
	lt->base=num3;lt->multiple=num4;
	memcpy(bh.lt,(void *)lt,1);
	memset(bh.reserved,0,1);
	bh.version_nh.HT=itsGnProtocolVersion;
	if (itsGnSecurity==1)bh.version_nh.HST=2;else bh.version_nh.HST=1;
	itsnet_unicast_t un_h;
	un_h.sequencenumber=SN_g;
	//PRF("SN %d\n",tsb_h.sequencenumber);
	SN_g++; //máximo SN?? % SN_MAX;
	memcpy(&un_h.dest_position_vector,LPV,20);
	itsnet_position_vector *LPV_so=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	memcpy(LPV_so,(char *)(buffer)+12,24);
	un_h.source_position_vector=* LPV_so;
	memset(un_h.reserved,0,2);
	pkt->payload.itsnet_unicast=un_h;//print_hex_data(pkt->payload.itsnet_tsb.payload.payload,lon_int);printf(" .\n");
	pkt->common_header=ch;
	pkt->basic_header=bh;
	return(pkt);

}


