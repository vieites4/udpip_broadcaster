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
extern itsnet_position_vector * LPV;

#include "cb_udp_events.h"
/* cb_print_recvfrom */
//extern int SN_g;
/**void cb_print_recvfrom(public_ev_arg_t *arg)
{

	bool blocked = false;
	//arg->len = 0;

	// 1) read UDP message from network level
	//		(self-broadcast messages are not received)
	if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header
			, arg->local_addr->sin_addr.s_addr
			, &blocked) ) < 0 )
	{
		log_app_msg("cb_print_recvfrom: <recv_msg> " \
				"Could not receive message.\n");
		return;
	}

	printf(">>> RECEIVED UDP MESSAGE >>>\n");
	print_hex_data(arg->data, arg->len);
	printf("\n");

}**/

#define __TX_DELAY_US 1000000		/**< (usecs) without sending. */

#define __DATA_BUFFER_LEN 100		/**< Length of the buffer. */
char data[__DATA_BUFFER_LEN];		/**< Static buffer. */

/* cb_broadcast_sendto */
/**void cb_udp_sendto(public_ev_arg_t *arg)
{
	printf("cb_udp_sendto\n");
	sprintf(data, "TEST#%d", arg->__test_number++);
	int len = strlen(data);

	sockaddr_t *dest_addr = (sockaddr_t *)
					init_sockaddr_in(arg->port, arg->forwarding_addr);
	printf("envio\n");
	printf(">>> TEST (fd = %d): sending test[%d] = %s\n"
			, arg->socket_fd, len, data);

	send_message(dest_addr, arg->socket_fd, data, len);

	if ( usleep(__TX_DELAY_US) < 0 )
	{
		log_app_msg("Could not sleep for %d (usecs).\n", __TX_DELAY_US);
		return;
	}

}**/


void cb_raw_sendto(public_ev_arg_r *arg)
{
	//	printf("cb_raw_sendto\n");
	//	sprintf(data, "BROADCAST-TEST#%d", arg->__test_number++);
	//	int len = strlen(data);
	//	printf("envio1\n");
	//	sockaddr_t *dest_addr = (sockaddr_t *)
	//			init_broadcast_sockaddr_ll(arg->port);
	//
	//	printf(">>> BROADCAST TEST (fd = %d): sending test[%d] = %s\n"
	//			, arg->socket_fd, len, data);
	//
	//	send_message(dest_addr, arg->socket_fd, data, len);
	//
	//	if ( usleep(__TX_DELAY_US) < 0 )
	//	{
	//		log_app_msg("Could not sleep for %d (usecs).\n", __TX_DELAY_US);
	//		return;
	//	}

}

/* cb_forward_recvfrom */
void cb_forward_recvfrom(public_ev_arg_r *arg)
{
	printf("cb_forward_recvfrom\n");
	bool blocked = false;
	arg->len = 0;
	// 1) read UDP message from network level
	if ( ( arg->len = recv_message(arg->socket_fd,arg->data))<0)//(arg->socket_fd, arg->msg_header, NULL, &blocked) ) < 0 )//arg->local_addr->sin_addr.s_addr
	{log_app_msg("cb_forward_recvfrom: <recv_msg> " \
			"Could not receive message.\n");
	return;}
	//printf("PAQUETE RECIBIDO POLO ENLACE\n");
	//	int i=print_hex_data(&arg->data, arg->len);
	char *datos= (char *)malloc(arg->len);
	memcpy(datos,arg->data,arg->len);

	CommonHeader_processing(arg);
	char *HT=NULL;
	HT = (char *)malloc(2);
	memcpy(HT,arg->data,2);
	itsnet_packet_f * pkt;
	pkt =(itsnet_packet_f *)malloc(sizeof(itsnet_packet_f));
	if(memcmp(HT,tsb1,1)==0){
		printf("entro en tsb\n");
		//if (search_in_locT(data)==0){add_end_locT (  locT,*data);}		-->modificar aqui para a actualización
		if(duplicate_control(datos)==1){
			//discard packet
			//omit next steps
		}

		// update de PV(SO)
		pkt = TSB_f(datos);
		//memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet_f) );
		int fwd_bytes = send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
		itsnet_packet * pkt1;
		pkt1 =(itsnet_packet *)malloc(sizeof(itsnet_packet));
		memcpy(pkt1, arg->data,arg->len);

		char Hop[1] ; //colle perfectamente os valores sen facer a reserva de memoria
		memcpy(Hop,datos+2,1);
		int lon_int=sprint_hex_data(Hop, 2);
		if (lon_int==1){//descartamos o paquete e omitimos os seguintes pasos
			}

			sprintf(pkt1->common_header.hop_limit,"X02",lon_int-1);
			pkt1->common_header.forwarder_position_vector=* LPV; //


			int fwd_bytes2 = send_message(	(sockaddr_t *)arg->local_addr,arg->socket_fd,&pkt1, arg->len	);

			printf("saio de tsb_f\n");
		} else if(memcmp(HT,tsb0,1)==0){
			printf("entro en shb\n");
			pkt = SHB_f(datos);
			printf("saio de shb_f\n");
			int fwd_bytes = send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			printf("entro en geobroadcast\n");
			pkt = GeoBroadcast_f(datos);
			printf("saio de geobroadcast_f\n");
		}
		// 2) in case the message comes from the localhost, it is discarded
		if ( blocked == true )
		{
			log_app_msg(">>>@cb_forward_recvfrom: Message blocked!\n");
			return;	}


		// 3) forward network level UDP message to application level

		//memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );



		//int fwd_bytes = send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&pkt, arg->len	);

		/**	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> fwd(net:%d>app:%d), msg[%.2d] = {"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("}\n");
	}**/
	//	printf("fin de cb_forward_recvfrom\n");

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
			log_app_msg("cb_broadcast_recvfrom: <recv_msg> " \
					"Could not receive message.\n");
			return;
		}

		char h_source[ETH_ALEN];
		get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;

		ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_ADDR_BROAD,h_source);
		//tx_frame->buffer.frame_type=0xdc05;
		//		tx_frame->buffer.frame_len = sizeof(arg->data) +42;
		char tipo[2]={0x07,0x07};
		memcpy(tx_frame->buffer.header.type,tipo,2);

		char *datos= (char *)malloc(arg->len);
		memcpy(datos,arg->data,arg->len);
		//			print_hex_data(datos,20);
		//			printf("\n");

		//agora debo buscar o que pasa cando chega unha gn_data

		char *HT=NULL;
		HT = (char *)malloc(2);
		memcpy(HT,arg->data,2);
		itsnet_packet * pkt;
		pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));

		if(memcmp(HT,tsb1,1)==0){
			//	printf("entro en tsb\n");
			pkt = TSB(datos);


			//printf("aqui chego1\n");
		} else if(memcmp(HT,tsb0,1)==0){
			//	printf("entro en tsb\n");
			pkt = SHB(datos);

			//printf("aqui chego1\n");
		} else if(memcmp(HT,geobroad0,1)==0 || memcmp(HT,geobroad1,1)==0 || memcmp(HT,geobroad2,1)==0){
			//	printf("entro en tsb\n");
			pkt = GeoBroadcast(datos);

			//printf("aqui chego1\n");
		}


		if(memcmp(HT,geounicast,1)==0){}
		//if(memcmp(HT,geounicast,1)==0){}
		// este é o punto onde teño que modificar os datos do buffer que envio.

		//	memcpy(tx_frame->buffer.data,arg->data,arg->len);
		memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );

		// 2) broadcast application level UDP message to network level
		int fwd_bytes = send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, arg->len);

		printf("ENVIO UN PAQUETE\n");
		int i=print_hex_data(&tx_frame->buffer, arg->len);
		printf("envio trama\n");

		//	if ( arg->print_forwarding_message == true )
		//{
		//	log_app_msg(">>> BROADCAST(app:%d>net:%d), msg[%.2d] = {"
		//				, arg->port, arg->forwarding_port, fwd_bytes);
		//	print_hex_data(arg->data, arg->len);
		//	log_app_msg("}\n");
		//	}
	//	printf("saio ben do cb_broadcast_recvfrom\n");
		//return();
	}


