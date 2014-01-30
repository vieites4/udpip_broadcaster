/**
 * @file cb_udp_events.h
 * @author Ángela Souto Vieites(vieites4[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
 * This file is part of udpip-broadcaster.
 * udpip-broadcaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * udpip-broadcaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with udpip-broadcaster.  If not, see <http://www.gnu.org/licenses/>.
 */
//#include "udp_events.h"

extern const unsigned char tsb0[1];
extern const unsigned char tsb1[1];
extern const unsigned char geobroad0[1];
extern const unsigned char geobroad1[1];
extern const unsigned char geobroad2[1];
extern const unsigned char geoanycast0[1];
extern const unsigned char geoanycast1[1];
extern const unsigned char geoanycast2[1];
extern const unsigned char geounicast[1];
extern const unsigned char beacon[1];
extern const unsigned char any[1];
extern const unsigned char ls0[1];
extern const unsigned char ls1[1];
extern const unsigned char single[1];
extern const unsigned char ETH_ADDR_BROADCAST[6];
#include "cb_udp_events.h"
extern const unsigned char ZEROS[ETH_ALEN];
extern const unsigned char TWOS[ETH_ALEN];
extern List_lsp *lsp_cbf_uc;
extern List_locT *locT_general;
extern List_lsp * lsp_uc_g;
extern List_lsp * lsp_bc_g;
extern time_t PDR;
extern itsnet_node_id GN_ADDR;
//time_t PDR_t;
extern itsnet_position_vector * LPV;
struct ev_loop * l_Beacon;
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
char type07[2]={0x07,0x07};
ev_timer t_Beacon;
void *thr_h3(void *arg){
	t_Beacon.data=arg;
	l_Beacon= EV_DEFAULT;
	int num=itsGnBeaconServiceMaxJitter+1;
	ev_timer_init(&t_Beacon,Beacon_send,0,itsGnBeaconServiceRetransmitTimer+ (rand()%num) ); //
	ev_timer_start(l_Beacon,&t_Beacon);
	return NULL;
}

#define __TX_DELAY_US 1000000		/**< (usecs) without sending. */
#define __DATA_BUFFER_LEN 100		/**< Length of the buffer. */

volatile sig_atomic_t keep_going = 1;
/* cb_forward_recvfrom */

void cb_forward_recvfrom(public_ev_arg_r *arg)
{
	char data[ITSNET_DATA_SIZE*3];
	arg->len = 0;int aa=0;int ae=0;int at=0;
	// 1) read UDP message from network level
	if ( ( arg->len = recv_message(arg->socket_fd,data))<0)
	{PRF("cb_forward_recvfrom: <recv_msg>  Could not receive message.\n");	return;}
	//		(self-broadcast messages are not received)
	if (memcmp((void *)type07,data+12,2)==0){		PRF("cb_forward_recv \n");
		PDR_update(data);
		char h_source[ETH_ALEN];
		get_mac_address(arg->socket_fd, net_name,(unsigned char *) h_source) ;
		if(memcmp((void *)data +6,h_source,6)==0){
		PRF(">>>@cb_forward_recvfrom: Message blocked!\n");
		return;	}

		memcpy(arg->data,data,arg->len);
		print_hex_data((char *)data,arg->len);PRF(" o paquete que chega \n");
		char datos[arg->len];
		memcpy(datos,(void *)(data) +14,arg->len);
		char HT[1];char HL[1];char LEN[2];
		memcpy(HT,(char *)(datos)+4 +1,1);
		itsnet_packet_f * pkt=NULL;
		memcpy(HL,(char *)(datos)+3,1);
		memcpy(LEN,(char *)(datos) +4+4,2);
		int lon_in=sprint_hex_data( LEN, 2);
		int hl_int=sprint_hex_data( HL, 1);
		if((memcmp(HT,tsb0,1)==0&& (hl_int>1))||memcmp(HT,tsb0,1)==0||memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0 ||memcmp(HT,geounicast,1)==0 ||memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0 || memcmp(HT,ls0,1)==0||memcmp(HT,ls1,1)==0){
			if(duplicate_control(datos,arg->locT)==0||memcmp((void *)&GN_ADDR,(char *)(datos)+16,8)==0)return;	}
		else if(memcmp(HT,beacon,1)==0 || (memcmp(HT,tsb0,1)==0)){	if(duplicate_control2(datos,arg->locT)==0||memcmp((void *)&GN_ADDR,(char *)(datos)+12,8)==0){PRF("return\n");return;}}
		if(BasicHeader_processing(arg)!=0){PRF("return\n");return;}
		//PRF("paso basicheadermak \n");
		print_hex_data(HL,1);PRF("hl \n");
		if(memcmp(HT,tsb0,1)==0&& (hl_int>1)){
			PRF("entro en tsb \n");
			pkt = TSB_f(datos);aa=1;
			//	print_hex_data((char *)pkt,lon_in +20);PRF(" cara arriba \n");
			send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +40	);
			PRF("saio de tsb_f \n");print_hex_data(pkt, lon_in +40);PRF(" envio tsb cara arriba\n");
		} else if(memcmp(HT,tsb0,1)==0){
			PRF("entro en shb\n");
			pkt = SHB_f(datos);
			send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +40);//	print_hex_data((char *)pkt,lon_in +20);PRF(" cara arriba \n");
			print_hex_data(pkt, lon_in +40);PRF(" envio shb cara arriba\n");
			PRF("envio realizado\n");
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			PRF("entro en geobroadcast \n");PRF(" longitud %d\n",lon_in);
			aa=1;
			pkt = GeoBroadcast_f(datos);
			int y =geo_limit(HT,pkt,LPV);PRF("geo_limit: %d\n",y);
			if (y>=0){
				send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +56	);}
			PRF("saio de geobroadcast_f \n");print_hex_data(pkt, lon_in +56);PRF(" envio geo cara arriba\n");
		}		else if(memcmp(HT,beacon,1)==0 ){
			PRF("entro en beacon\n");
		}else if(memcmp(HT,geounicast,1)==0 ){
			PRF("entro en geounicast \n");
			aa=1;
			char ADDR[8];
			memcpy(ADDR,(char *)(datos)+8+4+4+24,8);
			if(memcmp(ADDR,(char *)&GN_ADDR,8)==0){
				pkt = GeoUnicast_f(datos);
				send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +40	);
				PRF("saio de geounicast \n"); //check gn_address to know if we have to forward in LL
			}else{
				aa=1;
			}
		}
		else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0  ){
			aa=1;
			pkt = GeoAnycast_f(datos);
			int y =geo_limit(HT,pkt,LPV);
			if (y>=0){	aa=0;send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in+56	);}
		}else if(memcmp(HT,ls0,1)==0){

			char ADDR[8];
			memcpy(ADDR,(char *)(datos)+8+4+4+24,8);
			if(memcmp(ADDR,(char *)&GN_ADDR,8)==0){
				itsnet_packet * pkt1=NULL;
				pkt1 = LS_req_f(datos);
				char h_source2[ETH_ALEN];
				get_mac_address(arg->net_socket_fd, net_name,(unsigned char *) h_source2) ;
				ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source2);
				memcpy(tx_frame1->buffer.header.type,type07,2);
				memcpy(tx_frame1->buffer.data,(char *)  pkt1, 60);
				sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
				send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer,60 );
				//o envio é cara a rede no caso de reply, faise igual que se faría o forwardin cara a rede de GUC
			}else{
				//tsb forwarding
				aa=1;	at=1;		}		}
		else if(memcmp(HT,ls1,1)==0){

			char ADDR[8];
			memcpy(ADDR,(char *)(datos)+8+4+4+24,8);
			if(memcmp(ADDR,(char *)&GN_ADDR,8)==0){
				itsnet_packet * pkt1=NULL;
				mac_addr package;
				memcpy(package.address,ADDR+2,6);
				int pos=search_in_locT_m_pending(package,locT_general);
				Element_locT * position=locT_general->init;
				int cont=1;
				while(cont<pos){position=position->next;cont++;}
				position->data.LS_PENDING=false;
				sup_elem_t_lsp(position->data.SN1,3);//stop rtx because timer stops
				sup_timer(position->data.SN1,2);
				//aqui hai que borrar o ls porque no processing basase no source da 2º  mac enton non o fai directamente neste caso
			}else{
				aa=1;ae=1;	//forward coma en guc!
			}		}		else{}
		Element_locT *aux;
		aux = locT_general->init;
		int i=0;
		char MAC[6];
		memcpy(MAC,arg->data,6);
		int PDR_int=0;
		while (aux != NULL && i==0){
			if (memcmp(aux->data.mac_id.address,MAC,6)==0){
				i=1;PDR_int=aux->data.pdr;}else		aux = aux->next;	}

		if((memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0||memcmp(HT,tsb1,1)==0||memcmp(HT,tsb0,1)==0||memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0) ){
			free(pkt);pkt=NULL;}
		if (aa==7 && PDR_int<= itsGnMaxPacketDataRate){
			if (memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0|| memcmp(HT,tsb0,1)==0 ||memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0||memcmp(HT,geounicast,1)==0||ae==1 ){
				PRF("entro no envio do enlace cara o enlace \n");
				if (hl_int>1){
					PRF("o meu hop limit é maior que un %d\n",hl_int);
					itsnet_packet * pkt1=NULL;
					pkt1 =(itsnet_packet *)malloc(sizeof(itsnet_packet));
					memcpy(pkt1, data +14 ,arg->len);
					byte_struct *number;
					itsnet_position_vector *LPV_se= &pkt1->payload.itsnet_geobroadcast.source_position_vector;
					number=hl_int -1;print_hex_data(&number,1);PRF(" o number\n");
					memcpy(pkt1->basic_header.rhl,&number,1);
					if (memcmp(HT,tsb1,1)==0 || at==1){pkt1->payload.itsnet_tsb.source_position_vector=* LPV;}else {pkt1->payload.itsnet_geobroadcast.source_position_vector=* LPV;}
					itsnet_common_header *ch=NULL;
					ch =(itsnet_common_header *)malloc(sizeof(itsnet_common_header));
					memcpy(ch,datos+4,8);
					if  (((memcmp(HT,tsb0,1)==0||memcmp(HT,tsb1,1)==0|| at==1||memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0) && any_neighbours()==0)&& ch->traffic_class.scf==1){
						//pkt1=(itsnet_packet *)malloc(sizeof(itsnet_packet));
						int val=lsp_bc_g->size+8+4+28+4+sprint_hex_data((char *)(datos)+4+4,2);
						//delete old buffered elements if we need more size to add a new one.
						while (val>itsGnBcForwardingPacketBufferSize){
							lsp_bc_g=sup_elem_lsp(0xffff,0);
							val=lsp_bc_g->size+4+8+28+4+sprint_hex_data((char *)(datos)+4 +4,2);
							PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
						}
						int i =add_end_lsp(lsp_bc_g, *pkt1,0);PRF("entrei3\n");}else
							if((memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0) && any_neighbours()!=0){
								mac_addr nh;//= Greedy_Forwarding_UC(lpv_dest);
								if(itsGnGeoBroadcastForwardingAlgorithm==0||itsGnGeoBroadcastForwardingAlgorithm==1){
									memcpy(nh.address,ETH_ADDR_BROADCAST,6); //first send is broadcast
									int f=geo_limit_ll(pkt1,LPV);
									if (f>=0){}	else{

										if(search_in_locT_m_wo_n(LPV_se->node_id.mac,locT_general)>0 && (LPV_se->pai==1)){
											int f1=geo_limit_ll(pkt1,LPV_se);
											if (f1>=0){
												itsnet_position_vector A;
												A.latitude=pkt1->payload.itsnet_geobroadcast.dest_latitude;
												A.longitude=pkt1->payload.itsnet_geobroadcast.dest_longitude;
												mac_addr NH=Greedy_Forwarding_UC(&A);
												if (memcmp(nh.address,TWOS,6)==0 ||memcmp(nh.address,ZEROS,6)==0){free(pkt1);free(ch);return;}else{
													ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
													sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
													memcpy(tx_frame->buffer.header.type,type07,2);PRF("fallo despois desto?\n");
													memcpy(tx_frame->buffer.data, pkt1, IEEE_80211_BLEN);
													send_message(	(sockaddr_t *)dir,arg->forwarding_socket_fd,&tx_frame->buffer,sprint_hex_data(pkt1->common_header.payload_lenght,2)+ 44+4+8+14+4);free(tx_frame);free(pkt1);free(ch);return;

												}}else{	//discard
													free(pkt1);free(ch);
													return;	}			}else{	}		}
									get_mac_address(arg->socket_fd, net_name, (unsigned char *) h_source) ;
									ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
									sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
									memcpy(tx_frame->buffer.header.type,type07,2);PRF("fallo despois desto?\n");
									memcpy(tx_frame->buffer.data, pkt1, IEEE_80211_BLEN);
									send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,sprint_hex_data((char *)(datos)+4 +4,2)+ 44+4+8+14+4);free(dir);free(pkt1);free(tx_frame);return;
								}
								else if(itsGnGeoBroadcastForwardingAlgorithm==2){

									nh=CBF_BC(pkt1,sprint_hex_data((char *)(datos)+4 +4,2)+ 44+4+8+14+4,LPV); //first send PV_SE is LPV
									if (memcmp(nh.address,TWOS,6)==0 ||memcmp(nh.address,ZEROS,6)==0){free(pkt1);free(ch);return;}else{
										ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
										sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
										memcpy(tx_frame->buffer.header.type,type07,2);PRF("fallo despois desto?\n");
										memcpy(tx_frame->buffer.data, pkt1, IEEE_80211_BLEN);
										send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,sprint_hex_data((char *)(datos)+4 +4,2)+ 44+4+8+14+4);free(tx_frame);free(dir);free(pkt1);free(ch);return;}
								}	else if(itsGnGeoBroadcastForwardingAlgorithm==3){
									nh=	Advanced_BC(pkt1,sprint_hex_data((char *)(datos)+4 +4,2)+ 44+4+8+14+4,LPV,NULL,h_source);
									if (memcmp(nh.address,TWOS,6)==0 ||memcmp(nh.address,ZEROS,6)==0){free(pkt1);free(ch);return;}else{

										ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
										sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
										memcpy(tx_frame->buffer.header.type,type07,2);PRF("fallo despois desto?\n");
										memcpy(tx_frame->buffer.data, pkt1, IEEE_80211_BLEN);
										send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,sprint_hex_data((char *)(datos)+4 +4,2)+ 44+4+8+14+4);free(tx_frame);free(dir);free(pkt1);free(ch);return;}

									if((memcmp(HT,geounicast,1)==0 ||ae==1) && any_neighbours()==0 && ch->traffic_class.scf==1){
										//	if(memcmp(HT,geounicast,1)==0 && any_neighbours()==0 && ch.traffic_class.scf==0){broadcast normal}
										PRF("entrei4\n");
										int val=lsp_uc_g->size+8+4+48+4+sprint_hex_data((char *)(datos)+4 +4,2);
										//delete old buffered elements if we need more size to add a new one.
										while (val>itsGnBcForwardingPacketBufferSize){
											lsp_uc_g=sup_elem_lsp(0xffff,4);
											val=lsp_uc_g->size+4+8+48+4+sprint_hex_data((char *)(datos)+4 +4,2);
											PRF("aqui podo liala porque non se actualice lsp_uc_g a tempo");
										}
										int i =add_end_lsp(lsp_uc_g, *pkt1,4);free(pkt1);free(ch);return;
									}
									if(((memcmp(HT,geounicast,1)==0 ||ae==1) && any_neighbours()!=0)||((memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0)&&itsGnGeoAreaLineForwarding==true)){
										itsnet_position_vector *lpv_dest=NULL;
										memset(lpv_dest,0,24);

										if(memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){

											lpv_dest->latitude=pkt1->payload.itsnet_geobroadcast.dest_latitude;
											lpv_dest->longitude=pkt1->payload.itsnet_geobroadcast.dest_longitude;
										}else
											memcpy(lpv_dest,(void *)&pkt1->payload.itsnet_unicast.dest_position_vector,20);


										if(itsGnGeoUnicastForwardingAlgorithm==0||itsGnGeoUnicastForwardingAlgorithm==1){

											mac_addr nh= Greedy_Forwarding_UC(lpv_dest);
											if (memcmp(nh.address,TWOS,6)==0 ){free(pkt1);free(ch);return;}else if (memcmp(nh.address,ZEROS,6)==0){
												if((memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0) && pkt1->common_header.traffic_class.scf==1){
													int val=lsp_bc_g->size+8+4+44+4+sprint_hex_data((char *)(datos)+4 +4,2);
													//delete old buffered elements if we need more size to add a new one.
													while (val>itsGnBcForwardingPacketBufferSize){
														lsp_bc_g=sup_elem_lsp(0xffff,0);
														val=lsp_bc_g->size+8+4+44+4+sprint_hex_data((char *)(datos)+4 +4,2);
														PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
													}
													int i =add_end_lsp(lsp_bc_g, *pkt1,0);}

												free(pkt1);free(ch);
												return;}else{
													char h_source[ETH_ALEN];
													sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
													get_mac_address(arg->socket_fd, net_name, (unsigned char *) h_source) ;
													ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, nh.address,h_source);
													char type[2]={0x07,0x07};
													memcpy(tx_frame->buffer.header.type,type,2);PRF("fallo despois desto?\n");
													memcpy(tx_frame->buffer.data, pkt1, IEEE_80211_BLEN);
													send_message(	(sockaddr_t *)dir,arg->net_socket_fd,&tx_frame->buffer,sprint_hex_data((char *)(datos)+4 +4,2)+ 48+4+8+14+4);free(tx_frame);
													free(pkt1);free(ch);pkt1=NULL;free(dir);return;}				}
										else if(itsGnGeoUnicastForwardingAlgorithm==2){
											int a=CBF_UC(pkt,sprint_hex_data((char *)(datos)+4 +4,2)+ 48+4+8+14+4,lpv_dest);//only sends from timer
											if((memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0)&& a==0 && pkt1->common_header.traffic_class.scf==1){
												int val=lsp_bc_g->size+8+4+44+4+sprint_hex_data((char *)(datos)+4 +4,2);
												//delete old buffered elements if we need more size to add a new one.
												while (val>itsGnBcForwardingPacketBufferSize){
													lsp_bc_g=sup_elem_lsp(0xffff,0);
													val=lsp_bc_g->size+8+4+44+4+sprint_hex_data((char *)(datos)+4 +4,2);
													PRF("aqui podo liala porque non se actualice lsp_bc_g a tempo");
												}int i =add_end_lsp(lsp_bc_g, *pkt1,0);}
											free(pkt1);free(ch);
											return;
										}			}
									if(memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){free(pkt1);free(ch);return;}else{
										char h_source2[ETH_ALEN];
										get_mac_address(arg->net_socket_fd, net_name,(unsigned char *) h_source2) ;
										ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source2);
										memcpy(tx_frame1->buffer.header.type,type07,2);
										memcpy(tx_frame1->buffer.data,(char *)  pkt1, arg->len);
										sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
										send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer, arg->len);
										free(pkt1);free(ch);pkt1=NULL;free(tx_frame1);free(dir);return;}
								}
							}
					free(pkt1);free(ch);return;	}
			}
		}
	}
}


	/* cb_   _recvfrom */
	void cb_broadcast_recvfrom(public_ev_arg_r *arg){
		PRF("cb_broadcast_recvfrom\n");
		bool blocked = false;	arg->len = 0;
		// 1) read UDP message from application level
		//		(self-broadcast messages are not received)
		if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header, arg->local_addr->sin_addr.s_addr, &blocked) ) < 0 )
		{
			PRF("cb_broadcast_recvfrom: <recv_msg> Could not receive message.\n");
			return;		}
		char h_source[ETH_ALEN];
		get_mac_address(arg->socket_fd, net_name, (unsigned char *) h_source) ;
		ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_ADDR_BROADCAST,h_source);
		memcpy(tx_frame->buffer.header.type,type07,2);
		char datos[arg->len];
		memcpy(datos,arg->data,arg->len);//+4
		//print_hex_data(datos,arg->len);PRF(" datos\n");
		char HT[2];char LEN[2] ;char HL[1];
		memcpy(HT,arg->data,2);
		itsnet_packet * pkt=NULL;
		//pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));
		memcpy(LEN,(char *)(datos) +4,2);
		//print_hex_data(arg->data,arg->len);printf("entro en broadcast\n");
		int lon_int=sprint_hex_data( LEN, 2);
		memcpy(HL,(char *)(arg->data) +2,1);
		//print_hex_data(HL,1);printf("HL \n");
		if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)!=0)){
			PRF("entro en tsb1\n");
			pkt = TSB(datos,arg->lsp,arg->rep);
		} else if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)==0)){
			PRF("entro en tsb0\n");
			pkt = SHB((void *)datos,arg->lsp,arg->rep);
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			PRF("entro en geobroad\n");
			pkt = GeoBroadcast(datos,arg->lsp,arg->rep,arg);//GeoBroadcast
			PRF("entro en geobroad!\n");
		}else if(memcmp(HT,geounicast,1)==0){
			PRF("entro en geounicast\n");
			pkt = GeoUnicast(datos,arg->lsp,arg->rep,arg);// 9.3.7.1.2
		}else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){
			PRF("entro en geoanycast0\n");
			pkt = GeoAnycast(datos,arg->lsp,arg->rep);
		}else{}
		// 2) broadcast application level UDP message to network level
		memcmp(HT,(char *)&pkt->common_header.HT_HST,1);
		if((memcmp(HT,geobroad0,1)==0)||(memcmp(HT,tsb0,1)==0)||(memcmp(HT,tsb1,1)==0)|| memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0|| memcmp(HT,geounicast,1)==0|| memcmp(HT,ls0,1)==0){
			if (pkt!=NULL){
				//&& (memcmp(HL,single,1)!=0)
				int header_length=0;
				if(memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0 ||memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){header_length=44;}
				else if(memcmp(HT,tsb0,1)==0){header_length=28+6;}else if(memcmp(HT,geounicast,1)==0){header_length=48;} else if(memcmp(HT,ls0,1)==0){header_length=36;}else if(memcmp(HT,ls0,1)==0){header_length=48;}
				memcpy(tx_frame->buffer.data, (char *) pkt,lon_int+header_length+4+8+4);
				send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, header_length +lon_int+14+4+8+2);//==-1){}
				//print_hex_data(pkt, header_length +lon_int+4+8+2+6);PRF(" o paquete que metín!\n");
				if(memcmp(HT,tsb0,1)==0)ev_timer_again (l_Beacon,&t_Beacon);
				//print_hex_data(&tx_frame->buffer,header_length+ lon_int+4+8);
				PRF(" paquete enviado directo \n");
				free(pkt);pkt=NULL;
			}}
		free(tx_frame);
		//PRF("saio ben do cb_broadcast_recvfrom\n");
		//view_locT();
		PRF("pinto lista loct \n");
		//view_lsp();PRF("pinto lista lsp\n");
		//view_timers();PRF("pinto lista timers lsp \n");
		//return();
	}


