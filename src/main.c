/**
 * @file main.c
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
 *  Created on: May 6, 2013
 *      Author: Ricardo Tubío (rtpardavila[at]gmail.com)
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
#include "main.h"
#include <pthread.h>

/************************************************** Application definitions */

//static const char* __x_app_name = "udpip-broadcaster";
//static const char* __x_app_version = "0.1";

/******************************************************* INTERNAL FUNCTIONS */

/* main */
struct ev_loop * l_LPV;
ev_timer t_LPV;
int SN_g;//sequence number
itsnet_node_id GN_ADDR;
List_lsp * lsp_bc_g;
void *thr_h1(void * arg){

	t_LPV.data=arg;
	//ollo!! teño que reiniciar o temporizador cando se renove o LPV por outros medios!
	l_LPV=EV_DEFAULT;

	ev_timer_init(&t_LPV,LPV_update,0.,itsGnMinimunUpdateFrequencyLPV); //
	ev_timer_start(l_LPV,&t_LPV);
	return NULL;
}



int main(int argc, char **argv)
{
	List_locT * locT_g;

	pthread_t h1,h3, h_locT,h_lsp;
	log_app_msg(">>> Reading configuration...\n");
	configuration_t *cfg = create_configuration(argc, argv);
	log_app_msg(">>> Configuration read! Printing data...\n");
	print_configuration(cfg);
	lsp_bc_g=init_lsp();
	List_lsp *rep_bc_g=init_lsp();
	locT_g=startup1();

	// 2) Create UDP socket event managers:
	udp_events_t *net_events = NULL;
	udp_events_t *app_events = NULL;
	log_app_msg(">>> Opening UDP APP RX socket...\n");
	void * argum=init_app_udp_events(cfg->app_rx_port, cfg->app_address,cfg->if_name, cfg->tx_port	, cb_broadcast_recvfrom,locT_g,lsp_bc_g,rep_bc_g);//broadcast
	pthread_create(&h3,NULL, thr_h3, argum);	//Beacon_send(arg);
	log_app_msg(">>> UDP APP RX socket open!\n");
	log_app_msg(">>> Opening RAW NET RX socket...\n");
	net_events = init_net_raw_events(cfg->tx_port,cfg->rx_port, cfg->if_name , cfg->app_address, cfg->app_tx_port, cfg->nec_mode , cb_forward_recvfrom,locT_g,lsp_bc_g,rep_bc_g);
	log_app_msg(">>> raw NET RX socket open!\n");
	public_ev_arg_r * argument= (public_ev_arg_r *)argum;
	char h_source[ETH_ALEN];
	get_mac_address(argument->forwarding_socket_fd, "wlan0", (unsigned char *) h_source) ;
	t_LPV.data=(void *)h_source;
	int revents;
	LPV_update(l_LPV, &t_LPV, revents);
	pthread_create(&h1,NULL, thr_h1, (void *)h_source);
	pthread_create(&h_locT,NULL,thr_h2,(void *)locT_g);
	ev_loop(net_events->loop, 0);
	pthread_join(h1, NULL);
	pthread_join(h_locT, NULL);
	pthread_join(h3, NULL);
	// 4) program finalization
	exit(EXIT_SUCCESS);

}
