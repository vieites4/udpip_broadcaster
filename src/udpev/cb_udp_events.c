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
#include "novas/itsnet_header.h"
#include "cb_udp_events.h"
const unsigned char ETH_ADDR_BROAD[ETH_ALEN] ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
/* cb_print_recvfrom */
//extern int SN_g;
void cb_print_recvfrom(public_ev_arg_t *arg)
{

	bool blocked = false;
	arg->len = 0;

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

}

#define __TX_DELAY_US 1000000		/**< (usecs) without sending. */

#define __DATA_BUFFER_LEN 100		/**< Length of the buffer. */
char data[__DATA_BUFFER_LEN];		/**< Static buffer. */

/* cb_broadcast_sendto */
void cb_udp_sendto(public_ev_arg_t *arg)
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

}


void cb_raw_sendto(public_ev_arg_t *arg)
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
void cb_forward_recvfrom(public_ev_arg_t *arg)
{
	printf("cb_forward_recvfrom\n");
	bool blocked = false;
	arg->len = 0;
	void* datos;
	// 1) read UDP message from network level
if ( ( arg->len = recv_message(arg->socket_fd,arg->data))<0)//(arg->socket_fd, arg->msg_header, NULL, &blocked) ) < 0 )//arg->local_addr->sin_addr.s_addr


	{
		log_app_msg("cb_forward_recvfrom: <recv_msg> " \
						"Could not receive message.\n");
		return;
	}

	// 2) in case the message comes from the localhost, it is discarded
	if ( blocked == true )
	{
		log_app_msg(">>>@cb_forward_recvfrom: Message blocked!\n");
		return;
	}



	// 3) forward network level UDP message to application level
	int fwd_bytes = send_message
						(	(sockaddr_t *)arg->forwarding_addr,
							arg->forwarding_socket_fd,
							arg->data, arg->len	);

	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> fwd(net:%d>app:%d), msg[%.2d] = {"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("}\n");
	}
	printf("fin de cb_forward_recvfrom\n");

}

/* cb_broadcast_recvfrom */
void cb_broadcast_recvfrom(public_ev_arg_t *arg)
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

	const unsigned char tipoa[1]={0x01};
	const unsigned char tipob[1]={0x02};
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

	if (memcmp(arg->data +7,tipoa,1)==0)
	{printf("é btp tipo a\n");

	char *portoD=NULL;
	char *portoO=NULL;
	portoD = (char *)malloc(2);
	portoO = (char *)malloc(2);
	memcpy(portoO,arg->data + 16,2);
	memcpy(portoD,arg->data + 18,2);
	//print_hex_data(portoD,2);
	//printf("\n");
	memcpy(tx_frame->buffer.btp1, portoD,2);
	memcpy(tx_frame->buffer.btp2, portoO,2);

	}
	else{
		char *portoD=NULL;
		portoD = (char *)malloc(2);
		memcpy(portoD,arg->data + 18,2);
		char *info_dest=NULL;
		info_dest = (char *)malloc(2);
		memset(info_dest,0,2);
		memcpy(tx_frame->buffer.btp1, portoD,2);
		memcpy(tx_frame->buffer.btp2, info_dest,2);
	}
	char *datos= (char *)malloc(20);
			memcpy(datos,arg->data,20);
			//print_hex_data(datos,20);
			//printf("\n");

//agora debo buscar o que pasa cando chega unha gn_data

	char *HT=NULL;
	HT = (char *)malloc(2);
	memcpy(HT,arg->data,2);
	itsnet_packet * pkt;
	pkt =(itsnet_packet *)malloc(sizeof(itsnet_packet));

	if(memcmp(HT,tsb0,1)==0){
	//	printf("entro en tsb\n");
	pkt = TSB(datos);
	//printf("aqui chego1\n");
	}
	if(memcmp(HT,geounicast,1)==0){}
	//if(memcmp(HT,geounicast,1)==0){}
	// este é o punto onde teño que modificar os datos do buffer que envio.

//	memcpy(tx_frame->buffer.data,arg->data,arg->len);
memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );

	// 2) broadcast application level UDP message to network level
	int fwd_bytes = send_message(	(sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, arg->len);
	int i=print_hex_data(&tx_frame->buffer, arg->len);
	printf("envio trama\n");

	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> BROADCAST(app:%d>net:%d), msg[%.2d] = {"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("}\n");
	}
	printf("saio ben do cb_broadcast_recvfrom\n");

}


