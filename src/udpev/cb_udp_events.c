/**
 * @file cb_udp_events.h
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
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
extern itsnet_position_vector * LPV;
struct ev_loop * l_Beacon;
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
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
	char type07[2]={0x07,0x07};
	arg->len = 0;int error=0;int aa=0;
	// 1) read UDP message from network level
	char data[ITSNET_DATA_SIZE*3];
	if ( ( arg->len = recv_message(arg->socket_fd,data))<0)
	{PRF("cb_forward_recvfrom: <recv_msg>  Could not receive message.\n");	return;}

	//		(self-broadcast messages are not received)
	if (memcmp((void *)type07,data+12,2)==0){

		char h_source[ETH_ALEN];
		get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
		/**if(memcmp((void *)data +6,h_source,6)==0){
		PRF(">>>@cb_forward_recvfrom: Message blocked!\n");
		return;	}**/
		//ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source);
		//		memcpy(tx_frame1->buffer.header.type,type07,2);
		//if ((memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0)){

		PRF("cb_forward_recvfmakerom1 \n");
		PRF("RECIBO UN PAQUETE\n");
		//print_hex_data(data, arg->len);PRF("\n");
		memcpy(arg->data,data,arg->len);
		//print_hex_data((char *)data,arg->len);PRF(" cara arriba \n");
		char tipo[2];
		memcpy(tipo,(char *)(arg->data) + 12 ,2);
		char datos[arg->len];
		memcpy(datos,data +14,arg->len);

		char HT[1];
		memcpy(HT,data +15,1);
		itsnet_packet_f * pkt=NULL;
		char HL[1];
		memcpy(HL,(char *)(arg->data) +14+7,1);
		char LEN[2];
		memcpy(LEN,(char *)(datos) +5,1);memcpy(LEN+1,(char *)(datos) +4,1);
		int lon_in=sprint_hex_data( LEN, 2);PRF("lon_in %d %d \n",lon_in,arg->len);
		int lon_int=sprint_hex_data( HL, 1);
		if(memcmp(HT,tsb0,1)==0&& (lon_int>1)){
			PRF("entro en tsb \n");
			//if (search_in_locT(data)==0){add_end_locT (  locT,*data);}		-->modificar aqui para a actualización
			error =CommonHeader_processing(arg);
			if (error==0 && duplicate_control(datos,arg->locT)==1){
				pkt = TSB_f(datos,arg->gn);aa=1;
				//	memcpy(tx_frame1->buffer.data,(char *)  pkt, lon_in +20);
				//	print_hex_data((char *)pkt,lon_in +20);PRF(" cara arriba \n");
				send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +40	);
				PRF("saio de tsb_f \n");

			}
		} else if(memcmp(HT,tsb0,1)==0){
			PRF("entro en shb\n");
			error =CommonHeader_processing(arg);//print_hex_data(datos,arg->len);PRF("  chegada desde ll shb \n");
			pkt = SHB_f(datos,arg->gn); //	memcpy(tx_frame1->buffer.data,(char *)  pkt, lon_in +20);
			send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +40);//	print_hex_data((char *)pkt,lon_in +20);PRF(" cara arriba \n");
			PRF("envio realizado\n");
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			PRF("entro en geobroadcast \n");
			error =CommonHeader_processing(arg);
			PRF("entro en geobroadcast! %d \n",error);
			if (error==0 && duplicate_control(datos,arg->locT)==1){
				aa=1;
				//print_hex_data(datos,arg->len);PRF("datos entrada \n");
				pkt = GeoBroadcast_f(datos,arg->gn);
				int y =geo_limit(HT,pkt);
				//memcpy(tx_frame1->buffer.data,(char *)  pkt, lon_in);//print_hex_data(pkt,arg->len);PRF("\n");
				if (y>=0){
					send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,pkt, lon_in +56	);}
		//	print_hex_data(pkt,lon_in +56);PRF("datos entrada \n");
				PRF("saio de geobroadcast_f %d \n", y);
			}else PRF("%d \n",duplicate_control(datos,arg->locT));
		}
		else if(memcmp(HT,beacon,1)==0 ){
			PRF("entro en beacon\n");
			CommonHeader_processing(arg);
			PRF("saio de beacon \n");
		}else if(memcmp(HT,geounicast,1)==0 ){
			PRF("entro en geounicast \n");
			error =CommonHeader_processing(arg);
			if (error==0 && duplicate_control(datos,arg->locT)==1){
				aa=1;
				pkt = GeoUnicast_f(datos);
				//flush ls uc if its the one they was waiting
				//send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
				//	PRF("saio de geounicast \n"); //check gn_address to know if we have to forward in LL
			}
		}
		else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0  ){
			error =CommonHeader_processing(arg);
			if (error==0 && duplicate_control(datos,arg->locT)==1){
				aa=1;
				pkt = GeoBroadcast_f(datos);
				int y =geo_limit(HT,pkt);
				//	if (y>=0){	send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);}
			}
		}else if(memcmp(HT,ls0,1)==0){}
		else if(memcmp(HT,ls1,1)==0){}
		else{}
		if(aa==1 && (memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0||memcmp(HT,tsb0,1)==0||memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0) ){
			//PRF("entro en free\n");
			free(pkt);pkt=NULL;}

		//	PRF("comprobación %d\n",aa);
		if (aa==3){
			if ( memcmp(HT,tsb0,1)==0 ||memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0||memcmp(HT,geounicast,1)==0 ){
				PRF("entro no envio do enlace cara o enlace \n");
				if (lon_int>1){
					PRF("o meu hop limit é maior que un\n");
					itsnet_packet * pkt1=NULL;
					pkt1 =(itsnet_packet *)malloc(sizeof(itsnet_packet));
					memcpy(pkt1, data +14 ,arg->len);
					byte_struct *number;
					number=(byte_struct *)lon_int -1;
					memcpy(pkt1->common_header.hop_limit,&number,1);
					pkt1->common_header.forwarder_position_vector=* LPV; //
					PRF("ENVIO UN PAQUETE\n");	print_hex_data(pkt1, arg->len);PRF("\n");
					char h_source2[ETH_ALEN];
					get_mac_address(arg->net_socket_fd, "wlan0",(unsigned char *) h_source2) ;
					ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source2);
					char tipo[2]={0x07,0x07};
					memcpy(tx_frame1->buffer.header.type,tipo,2);
					//if ((memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0)){
					memcpy(tx_frame1->buffer.data,(char *)  pkt1, arg->len);
					//}else if (memcmp(HT,tsb0,1)==0){
					//	memcpy(tx_frame1->buffer.data,(char *) pkt1,IEEE_80211_BLEN );
					//}else {}
					//	PRF("strlen(pkt1)  %d\n",strlen(pkt1));

					sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
					//while(
					send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer, arg->len);//==-1){}
					//print_hex_data(&tx_frame1->buffer,arg->len);// header_length +lon_int+14+4);
							//PRF(" paquete enviado a ll despois de reenvio \n");
					//print_hex_data(&tx_frame1->buffer,arg->len);PRF("  forward******* \n");
					ev_timer_again (l_Beacon,&t_Beacon);free(pkt1);pkt1=NULL;
				}

			}
		}
	}//else{PRF("NON SON DESE TIPO!!\n");}

}

/* cb_broadcast_recvfrom */
void cb_broadcast_recvfrom(public_ev_arg_r *arg)
{PRF("cb_broadcast_recvfrom\n");
bool blocked = false;	arg->len = 0;
// 1) read UDP message from application level
//		(self-broadcast messages are not received)
if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header, arg->local_addr->sin_addr.s_addr, &blocked) ) < 0 )
{
	PRF("cb_broadcast_recvfrom: <recv_msg> Could not receive message.\n");
	return;		}


char h_source[ETH_ALEN];
get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_ADDR_BROADCAST,h_source);
char tipo[2]={0x07,0x07};


PRF(" paquete \n");
memcpy(tx_frame->buffer.header.type,tipo,2);
char datos[arg->len];
memcpy(datos,arg->data,arg->len);//+4

//print_hex_data(datos,arg->len);PRF("\n");
char HT[2];
memcpy(HT,arg->data,2);
itsnet_packet * pkt=NULL;
pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));

char LEN[2] ;
memcpy(LEN,(char *)(datos) +4,2);//print_hex_data(arg->data,arg->len);
int lon_int=sprint_hex_data( LEN, 2);
char HL[1];
memcpy(HL,(char *)(arg->data) +2,1);
if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)!=0)){
	//print_hex_data(datos,arg->len);
	PRF("entro en tsb1\n");
	pkt = TSB(datos,arg->lsp,arg->rep,arg->gn);
} else if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)==0)){
	PRF("entro en tsb0\n");
	pkt = SHB((void *)datos,arg->lsp,arg->rep,arg->gn);
} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
	PRF("entro en geobroad\n");
	pkt = GeoBroadcast(datos,arg->lsp,arg->rep,arg->gn);

}else if(memcmp(HT,geounicast,1)==0){
	PRF("entro en geounicast\n");
	pkt = GeoUnicast(datos,arg->lsp,arg->rep); //hai que decirlle a que dirección vai
}
else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){
	PRF("entro en geoanycast0\n");
	pkt = GeoBroadcast(datos,arg->lsp,arg->rep);
}else{}

// 2) broadcast application level UDP message to network level
if((memcmp(HT,geobroad0,1)==0)||(memcmp(HT,tsb0,1)==0)||(memcmp(HT,tsb1,1)==0)|| memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
	if (pkt!=NULL){
		//&& (memcmp(HL,single,1)!=0)

		//while(
		//PRF(" arg->len %d lon_int %d \n",arg->len, lon_int);
		int header_length=0;

		if(memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0){header_length=84;}
		else if(memcmp(HT,tsb0,1)==0&& (memcmp(HL,single,1)==0)){header_length=36;}else if(memcmp(HT,tsb0,1)==0&& (memcmp(HL,single,1)!=0)){header_length=68;}else{PRF("son eu3!! \n");} //falta tsb
		memcpy(tx_frame->buffer.data, (char *) pkt,lon_int+header_length+4);
		send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, header_length +lon_int+14+4);//==-1){}
		ev_timer_again (l_Beacon,&t_Beacon);

		int total=header_length +lon_int +14+4;
		int n_sends= floor(total/1500);
		//if (n_sends==0) print_hex_data(&tx_frame->buffer, header_length +lon_int+14+4);


		//PRF(" paquete enviado a ll %d %d.\n",header_length +lon_int+14+4,arg->len);
		free(pkt);pkt=NULL;  //print_hex_data(&tx_frame->buffer, header_length +lon_int+14+4);
		PRF("ENVIO UN PAQUETE DE CERO\n");
	}

}


//PRF("saio ben do cb_broadcast_recvfrom\n");
view_locT();PRF("pinto lista loct \n");
//view_lsp();PRF("pinto lista lsp\n");
//view_timers();PRF("pinto lista timers lsp \n");
//return();
}


