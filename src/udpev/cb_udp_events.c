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
	//ollo!! teño que reiniciar o temporizador cando se renove o LPV por outros medios!
	t_Beacon.data=arg;
	l_Beacon= EV_DEFAULT;
	int num=itsGnBeaconServiceMaxJitter+1;
	ev_timer_init(&t_Beacon,Beacon_send,0,itsGnBeaconServiceRetransmitTimer+ (rand()%num) ); //
	ev_timer_start(l_Beacon,&t_Beacon);
	return NULL;
}

#define __TX_DELAY_US 1000000		/**< (usecs) without sending. */
#define __DATA_BUFFER_LEN 100		/**< Length of the buffer. */
char data[__DATA_BUFFER_LEN];		/**< Static buffer. */
volatile sig_atomic_t keep_going = 1;
/* cb_forward_recvfrom */

void cb_forward_recvfrom(public_ev_arg_r *arg)
{
	char tipoX[2]={0x07,0x07};
	//	bool blocked = false;
	arg->len = 0;
	// 1) read UDP message from network level
	char data[ITSNET_DATA_SIZE];

	if ( ( arg->len = recv_message(arg->socket_fd,data))<0)//(arg->socket_fd, arg->data, NULL, &blocked) ) < 0 )//arg->local_addr->sin_addr.s_addr
	{log_app_msg("cb_forward_recvfrom: <recv_msg>  Could not receive message.\n");	return;}


	if (memcmp((void *)tipoX,data+12,2)==0){
		printf("cb_forward_recvfrom1 \n");
		printf("RECIBO UN PAQUETE\n");
		print_hex_data(data, arg->len);printf("\n");
		memcpy(arg->data,data,arg->len);

		char tipo[2];
		memcpy(tipo,arg->data + 12 ,2);
		char datos[arg->len];
		memcpy(datos,arg->data +14,arg->len -14);
		//CommonHeader_processing(arg);
		char HT[1];
		memcpy(HT,arg->data +15,1);
		itsnet_packet_f * pkt=NULL;
		char HL[1];
		memcpy(HL,arg->data +2,1);
		if(memcmp(HT,tsb0,1)==0&& (memcmp(HL,single,1)!=0)){
			printf("entro en tsb \n");
			//if (search_in_locT(data)==0){add_end_locT (  locT,*data);}		-->modificar aqui para a actualización
			CommonHeader_processing(arg);
			if(duplicate_control(datos,arg->locT)==1){
				//discard packet
				//omit next steps
			}

			// update de PV(SO)
			pkt = TSB_f(datos);
			//memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet_f) );
			//if(exist_neighbours(arg->locT) )
			send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
			printf("saio de tsb_f \n");
		} else if(memcmp(HT,tsb0,1)==0){
			//printf("entro en shb\n");
			CommonHeader_processing(arg);
			pkt = SHB_f(datos);
			//if(exist_neighbours(arg->locT) )
				send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			printf("entro en geobroadcast \n");
			CommonHeader_processing(arg);
			pkt = GeoBroadcast_f(datos);
			int y =geo_limit(HT,pkt);
			if (y>=0){
				//if(exist_neighbours(arg->locT) )
				send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);}
			printf("saio de geobroadcast_f \n");
		}
		else if(memcmp(HT,beacon,1)==0 ){

			printf("entro en beacon\n");
			CommonHeader_processing(arg);
			printf("saio de beacon \n");

		}else{}free(pkt);pkt=NULL;
		// 2) in case the message comes from the localhost, it is discarded
		//if ( blocked == true )
		//{
		//	log_app_msg(">>>@cb_forward_recvfrom: Message blocked!\n");
		//	return;	}
		if (memcmp(HT,tsb0,1)==0 ||(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0)){
			printf("entro no envio do enlace cara o enlace \n");

			unsigned char Hop[1] ; //colle perfectamente os valores sen facer a reserva de memoria
			memcpy(Hop,datos+7,1);
			int lon_int=sprint_hex_data(&Hop, 1);



			if (lon_int>1){//descartamos o paquete e omitimos os seguintes pasos //ESTO TEÑO QUE CAMBIALO POR >1
				printf("o meu hop limit é maior que un\n");
				itsnet_packet * pkt1=NULL;
				pkt1 =(itsnet_packet *)malloc(arg->len);

				memcpy(pkt1, data +14 ,arg->len );
				strings_an *number;
				number=(strings_an *)lon_int -1;
				memcpy(pkt1->common_header.hop_limit,&number,1);

				//print_hex_data(pkt1->common_header.hop_limit,1);printf("\n");
				pkt1->common_header.forwarder_position_vector=* LPV; //

				printf("ENVIO UN PAQUETE\n");	print_hex_data(pkt1, arg->len);printf("\n");

				char h_source[ETH_ALEN];
				get_mac_address(arg->net_socket_fd, "wlan0",(unsigned char *) h_source) ;

				ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_ADDR_BROADCAST,h_source);
				char tipo[2]={0x07,0x07};
				memcpy(tx_frame1->buffer.header.type,tipo,2);
				if ((memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0)){
					memcpy(tx_frame1->buffer.data,(char *)  pkt1, arg->len );
				}else if (memcmp(HT,tsb0,1)==0){
					memcpy(tx_frame1->buffer.data,(char *) pkt1, arg->len );

				}else {}

				//free(data);free(datos);free(HT);
				sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
				int fwd_bytes = send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer, arg->len);


				ev_timer_again (l_Beacon,&t_Beacon);free(pkt1);pkt1=NULL;
			}
			printf("remata cb_forward_recvfrom \n");
		}}
}

/* cb_broadcast_recvfrom */
void cb_broadcast_recvfrom(public_ev_arg_r *arg)
{

	printf("cb_broadcast_recvfrom\n");
	bool blocked = false;
	arg->len = 0;

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
	memcpy(datos,arg->data,arg->len);
	char HT[1];
	memcpy(HT,arg->data,1);
	itsnet_packet * pkt=NULL;
	pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));
	char HL[1];
	memcpy(HL,arg->data +2,1);

	if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)!=0)){
		printf("entro en tsb1\n");
		pkt = TSB(datos,arg->lsp,arg->rep);
	} else if((memcmp(HT,tsb0,1)==0)&& (memcmp(HL,single,1)==0)){
		printf("entro en tsb0\n");
		pkt = SHB((void *)datos,arg->lsp,arg->rep);
	} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
		printf("entro en geobroad\n");
		pkt = GeoBroadcast(datos,arg->lsp,arg->rep);
	}else if(memcmp(HT,geounicast,1)==0){}
	else if(memcmp(HT,geounicast,1)==0){}else{}
	memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );
	// 2) broadcast application level UDP message to network level
	if((memcmp(HT,geobroad0,1)==0)||(memcmp(HT,tsb0,1)==0)|| memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
		send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, arg->len);
		ev_timer_again (l_Beacon,&t_Beacon);
	}
	free(pkt);pkt=NULL; //	printf("ENVIO UN PAQUETE\n");	//int i=print_hex_data(&tx_frame->buffer, arg->len);
	printf("saio ben do cb_broadcast_recvfrom\n");
	view_locT(arg->locT);
	//return();
}


