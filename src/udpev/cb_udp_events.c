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
	char data[ITSNET_DATA_SIZE*2];
	if ( ( arg->len = recv_message(arg->socket_fd,data))<0)
	{log_app_msg("cb_forward_recvfrom: <recv_msg>  Could not receive message.\n");	return;}

	//		(self-broadcast messages are not received)
	if (memcmp((void *)type07,data+12,2)==0){

	char h_source[ETH_ALEN];
	get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	if(memcmp((void *)data +6,h_source,6)==0){
		log_app_msg(">>>@cb_forward_recvfrom: Message blocked!\n");
		return;	}

	//	printf("cb_forward_recvfmakerom1 \n");
	//printf("RECIBO UN PAQUETE\n");
	//print_hex_data(data, arg->len);printf("\n");
	memcpy(arg->data,data,arg->len);
	char tipo[2];
	memcpy(tipo,(char *)(arg->data) + 12 ,2);
	char datos[arg->len-14];
	memcpy(datos,data +14,arg->len -14);
	char HT[1];
	memcpy(HT,data +15,1);
	itsnet_packet_f * pkt=NULL;
	char HL[1];
	memcpy(HL,(char *)(arg->data) +14+7,1);
	int lon_int=sprint_hex_data( HL, 1);
	//print_hex_data(HT,1); printf(" VALOR DO HT\n");
	if(memcmp(HT,tsb0,1)==0&& (lon_int>1)){
		//printf("entro en tsb \n");
		//if (search_in_locT(data)==0){add_end_locT (  locT,*data);}		-->modificar aqui para a actualización
		error =CommonHeader_processing(arg);
		if (error==0 && duplicate_control(datos,arg->locT)==0){
			pkt = TSB_f(datos,arg->gn);aa=1; 		print_hex_data(&pkt,arg->len);//printf("  cara as facilities tsb \n");
			send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
			//printf("saio de tsb_f \n");
			}
	} else if(memcmp(HT,tsb0,1)==0){
		//printf("entro en shb\n");
		error =CommonHeader_processing(arg);//print_hex_data(datos,arg->len);printf("  chegada desde ll shb \n");
		pkt = SHB_f(datos,arg->gn); 		//print_hex_data(pkt,arg->len);printf("  cara as facilities shb \n");
		send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
		//	printf("envio realizado\n");
	} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
		//	printf("entro en geobroadcast \n");
		error =CommonHeader_processing(arg);
		//printf("entro en geobroadcast! %d \n",error);
		if (error==0 && duplicate_control(datos,arg->locT)==0){
			aa=1;
			pkt = GeoBroadcast_f(datos,arg->gn);
			int y =geo_limit(HT,pkt);
			if (y>=0){	send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);}
			//	print_hex_data(&pkt,arg->len);printf("  cara as facilities geo \n");
			//	printf("saio de geobroadcast_f \n");
		}else printf("%d \n",duplicate_control(datos,arg->locT));
	}
	else if(memcmp(HT,beacon,1)==0 ){
		//printf("entro en beacon\n");
		CommonHeader_processing(arg);
		//	printf("saio de beacon \n");
	}else if(memcmp(HT,geounicast,1)==0 ){
		//printf("entro en geounicast \n");
		error =CommonHeader_processing(arg);
		if (error==0 && duplicate_control(datos,arg->locT)==0){
			aa=1;
			pkt = GeoUnicast_f(datos);
			//flush ls uc if its the one they was waiting
			//send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
			//	printf("saio de geounicast \n"); //check gn_address to know if we have to forward in LL
		}
	}
	else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0  ){
		error =CommonHeader_processing(arg);
		if (error==0 && duplicate_control(datos,arg->locT)==0){
			aa=1;
			pkt = GeoBroadcast_f(datos);
			int y =geo_limit(HT,pkt);
		//	if (y>=0){	send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);}
		}
	}else if(memcmp(HT,ls0,1)==0){}
	else if(memcmp(HT,ls1,1)==0){}
	else{}
	if(aa==1 && (memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0||memcmp(HT,tsb0,1)==0||memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0) ){
		//printf("entro en free\n");
		free(pkt);pkt=NULL;}

	//	printf("comprobación %d\n",aa);
	if (aa==1){
		if ( memcmp(HT,tsb0,1)==0 ||memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0||memcmp(HT,geounicast,1)==0 ){
			printf("entro no envio do enlace cara o enlace \n");
			if (lon_int>1){
				//	printf("o meu hop limit é maior que un\n");
				itsnet_packet * pkt1=NULL;
				pkt1 =(itsnet_packet *)malloc(sizeof(itsnet_packet));
				memcpy(pkt1, data +14 ,arg->len-14);
				strings_an *number;
				number=(strings_an *)lon_int -1;
				memcpy(pkt1->common_header.hop_limit,&number,1);
				pkt1->common_header.forwarder_position_vector=* LPV; //
				//	printf("ENVIO UN PAQUETE\n");	print_hex_data(pkt1, arg->len);printf("\n");
				char h_source2[ETH_ALEN];
				get_mac_address(arg->net_socket_fd, "wlan0",(unsigned char *) h_source2) ;
				ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source2);
				char tipo[2]={0x07,0x07};
				memcpy(tx_frame1->buffer.header.type,tipo,2);
				//if ((memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0)){
				memcpy(tx_frame1->buffer.data,(char *)  pkt1, IEEE_80211_BLEN);
				//}else if (memcmp(HT,tsb0,1)==0){
				//	memcpy(tx_frame1->buffer.data,(char *) pkt1,IEEE_80211_BLEN );
				//}else {}
				//	printf("strlen(pkt1)  %d\n",strlen(pkt1));

				sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
				//while(
				send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer, arg->len);//==-1){}
				print_hex_data(&tx_frame1->buffer,arg->len);printf("  forward******* \n");
				ev_timer_again (l_Beacon,&t_Beacon);free(pkt1);pkt1=NULL;
			}

		}
	}
	}//else{printf("NON SON DESE TIPO!!\n");}
	//printf("remata cb_forward_recvfrom \n");
}

/* cb_broadcast_recvfrom */
void cb_broadcast_recvfrom(public_ev_arg_r *arg)
{//	printf("cb_broadcast_recvfrom\n");
bool blocked = false;	arg->len = 0;
// 1) read UDP message from application level
//		(self-broadcast messages are not received)
if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header, arg->local_addr->sin_addr.s_addr, &blocked) ) < 0 )
{
	log_app_msg("cb_broadcast_recvfrom: <recv_msg> Could not receive message.\n");
	return;		}


char h_source[ETH_ALEN];
get_mac_address(arg->socket_fd, "wlan0", (unsigned char *) h_source) ;
ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_ADDR_BROADCAST,h_source);
char tipo[2]={0x07,0x07};
memcpy(tx_frame->buffer.header.type,tipo,2);
char datos[arg->len];
memcpy(datos,arg->data,arg->len);//+4
//print_hex_data(datos,arg->len);printf("\n");
char HT[2];
memcpy(HT,arg->data,2);
itsnet_packet * pkt=NULL;
pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));
char HL[1];
memcpy(HL,(char *)(arg->data) +2,1);
if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)!=0)){
	//printf("entro en tsb1\n");
	pkt = TSB(datos,arg->lsp,arg->rep,arg->gn);
} else if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)==0)){
	//printf("entro en tsb0\n");
	pkt = SHB((void *)datos,arg->lsp,arg->rep,arg->gn);
} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
	//printf("entro en geobroad\n");
	pkt = GeoBroadcast(datos,arg->lsp,arg->rep,arg->gn);
}else if(memcmp(HT,geounicast,1)==0){
	//printf("entro en geounicast\n");
	pkt = GeoUnicast(datos,arg->lsp,arg->rep); //hai que decirlle a que dirección vai
}
else if(memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){
	//printf("entro en geoanycast0\n");
	pkt = GeoBroadcast(datos,arg->lsp,arg->rep);//hai que decirlle a que dirección vai
}else{}

// 2) broadcast application level UDP message to network level
if((memcmp(HT,geobroad0,1)==0)||(memcmp(HT,tsb0,1)==0&& (memcmp(HL,single,1)!=0))|| memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
	if (pkt!=NULL){
		memcpy(tx_frame->buffer.data, (char *) pkt, arg->len);

		//while(

		send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, arg->len);//==-1){}
			//print_hex_data(&pkt,arg->len);printf("   \n");
		//printf("porto de envio cara arriba: %d porto de recepcion : %d e lonxitude do envio %d arg->len %d\n",arg->forwarding_socket_fd,arg->socket_fd,strlen((char *)&tx_frame->buffer),strlen((char *)pkt));
		ev_timer_again (l_Beacon,&t_Beacon);
		//print_hex_data((char *)&tx_frame->buffer,40);
		//printf(" paquete enviado a ll\n");
		free(pkt);pkt=NULL; //	printf("ENVIO UN PAQUETE\n");	//int i=print_hex_data(&tx_frame->buffer, arg->len);
	}

}


//printf("saio ben do cb_broadcast_recvfrom\n");
view_locT();//printf("pinto lista loct \n");
//view_lsp();printf("pinto lista lsp\n");
//view_timers();printf("pinto lista timers lsp \n");
//return();
}


