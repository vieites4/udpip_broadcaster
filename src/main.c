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
//este está en workspace/udp..
#include "main.h"


#include <pthread.h>

//#include <gpsd.h>

//#include "novas/pkt_handling.h"

/************************************************** Application definitions */

//static const char* __x_app_name = "udpip-broadcaster";
//static const char* __x_app_version = "0.1";

/******************************************************* INTERNAL FUNCTIONS */



//List_buf * UCb, BCb;
/* main */
	struct ev_loop * l_LPV;
		ev_timer t_LPV;
int SN_g;//sequence number

itsnet_node_id GN_ADDR;

void *thr_h1(){


	 //ollo!! teño que reiniciar o temporizador cando se renove o LPV por outros medios!
	l_LPV=EV_DEFAULT;

	ev_timer_init(&t_LPV,LPV_update,0.,itsGnMinimunUpdateFrequencyLPV); //
	ev_timer_start(l_LPV,&t_LPV);

//t_LPV.repeat=1.;
	return NULL;
}



int main(int argc, char **argv)
{
	List_locT * locT_g; //variable global
	List_lsp * lsp_bc_g;
pthread_t h1,h3, h_locT,h_lsp;
 //pthread_create(&h1, NULL , slowprintf , (void *)hola)  o ultimo parámetro é o dos argumentos
//pthread_create(&h2,NULL, thr_h2, NULL);
	// 1) Runtime configuration is read from the CLI (POSIX.2).
	log_app_msg(">>> Reading configuration...\n");
	configuration_t *cfg = create_configuration(argc, argv);
	log_app_msg(">>> Configuration read! Printing data...\n");
	print_configuration(cfg);


	//1.a Start-up
	// address configuration
	lsp_bc_g=init_lsp();
	List_lsp *rep_bc_g=init_lsp();
	locT_g=startup1();
	LPV_update();
	pthread_create(&h1,NULL, thr_h1, NULL);
	pthread_create(&h_locT,NULL,thr_h2,NULL);

	//pthread_create(&h_lsp,NULL,thr_h2,NULL);
	// 2) Create UDP socket event managers:
	udp_events_t *net_events = NULL;
	udp_events_t *app_events = NULL;

	if ( cfg->__tx_test == true ) //tería que cambiar esta condición
	{
		bool broad= memcmp( cfg->app_address,"255.255.255.255",16);

		printf("chego\n");
		net_events = init_tx_raw_events(cfg->if_name, cfg->tx_port, cb_raw_sendto,broad); //aqui teño a posibilidade de facer q non sexa solo broadcast
		printf("> RAW TX socket open, port = %d, fd = %d.\n", cfg->tx_port, net_events->socket_fd);
		app_events= init_tx_udp_events(cfg->if_name, cfg->app_tx_port,cfg->app_inet_addr, cb_udp_sendto,broad); //aqui teño a posibilidade de facer q non sexa solo broadcast
		printf("fin\n");
		printf("> UDP TX socket open, port = %d , fd= ", cfg->tx_port);//, net_events->socket_fd);//, fd = %d. // da erro o de net_events->socket_fd-> buscar a solución
	}
	else	{

		log_app_msg(">>> Opening UDP APP RX socket...\n");
		 void * argum=init_app_udp_events(cfg->app_rx_port, cfg->app_address,cfg->if_name, cfg->tx_port	, cb_broadcast_recvfrom,locT_g,lsp_bc_g,rep_bc_g);//broadcast
		 pthread_create(&h3,NULL, thr_h3, argum);	//Beacon_send(arg);
		 log_app_msg(">>> UDP APP RX socket open!\n");
		//print_udp_events(app_events, cfg->app_rx_port, cfg->tx_port);
		log_app_msg(">>> Opening RAW NET RX socket...\n");
		net_events = init_net_raw_events(cfg->rx_port, cfg->if_name , cfg->app_address, cfg->app_tx_port, cfg->nec_mode , cb_forward_recvfrom,locT_g,lsp_bc_g,rep_bc_g);
		log_app_msg(">>> raw NET RX socket open!\n");
		//print_udp_events(net_events, cfg->rx_port, cfg->app_tx_port);


		}
printf("next\n");
	ev_loop(net_events->loop, 0); //realmente é este o bucle infinito que vai sendo interrumpido por sinais ev_read que despertan o common_cb q desperta o broadcast ou o forwarding segundo chegue por un porto ou polo outro.

//pthread_join(h3,NULL);
	pthread_join(h1, NULL);
//	pthread_join(h_lsp, NULL);
	pthread_join(h_locT, NULL);
	pthread_join(h3, NULL);
	//pthread_join(h2, NULL);

	// 4) program finalization
	exit(EXIT_SUCCESS);

}
