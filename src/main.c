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
#include "logger.h"
#include "configuration.h"
#include "udpev/udp_events.h"
#include "udpev/cb_udp_events.h"
#include "novas/netmanagement.h"
#include "novas/pkt_handling.h"

/************************************************** Application definitions */

static const char* __x_app_name = "udpip-broadcaster";
static const char* __x_app_version = "0.1";

/******************************************************* INTERNAL FUNCTIONS */

/* print_help */
void print_help()
{
	fprintf(stdout, "HELP, %s\n", __x_app_name);
}

/* print_version */
void print_version()
{
	fprintf(stdout, "Version = %s\n", __x_app_version);
}

/* main */
int main(int argc, char **argv)
{
	
	// 1) Runtime configuration is read from the CLI (POSIX.2).
	log_app_msg(">>> Reading configuration...\n");
	configuration_t *cfg = create_configuration(argc, argv);
	log_app_msg(">>> Configuration read! Printing data...\n");
	print_configuration(cfg);


	//1.a Start-up
	// address configuration
	startup1();


	// 2) Create UDP socket event managers:
	udp_events_t *net_events = NULL;
	udp_events_t *app_events = NULL;

	if ( cfg->__tx_test == true ) //tería que cambiar esta condición
	{
		bool broad= memcmp( cfg->app_address,"255.255.255.255",16);

		printf("chego\n");
		if (cfg->ladoapp ==false){
		net_events = init_tx_raw_events(cfg->if_name, cfg->tx_port, cb_raw_sendto,broad); //aqui teño a posibilidade de facer q non sexa solo broadcast
		printf("> RAW TX socket open, port = %d, fd = %d.\n", cfg->tx_port, net_events->socket_fd);
		}
		else{
			printf("chego\n");
		app_events= init_tx_udp_events(cfg->if_name, cfg->app_tx_port,cfg->app_inet_addr, cb_udp_sendto,broad); //aqui teño a posibilidade de facer q non sexa solo broadcast
	//	cb_udp_sendto();
		printf("fin\n");
		printf("> UDP TX socket open, port = %d , fd= ", cfg->tx_port);//, net_events->socket_fd);//, fd = %d. // da erro o de net_events->socket_fd-> buscar a solución
		}
	}
	else
	{printf("chego1\n");
		if (cfg->ladoapp ==false){
		log_app_msg(">>> Opening RAW NET RX socket...\n");
		net_events = init_net_raw_events ///esto non ten sentido que esté asi, fala de cousas in e non ll.  init_if_sockaddr_in
						(cfg->rx_port, cfg->if_name , cfg->app_rx_port, cfg->nec_mode , cb_forward_recvfrom);
		log_app_msg(">>> raw NET RX socket open!\n");
		print_udp_events(net_events, cfg->rx_port, cfg->app_rx_port);
		}
				else{
		log_app_msg(">>> Opening UDP APP RX socket...\n");
		app_events = init_app_udp_events
						(cfg->app_tx_port, cfg->app_address,cfg->if_name, cfg->tx_port	, cb_broadcast_recvfrom);
		log_app_msg(">>> UDP APP RX socket open!\n");
		print_udp_events(app_events, cfg->app_tx_port, cfg->tx_port);

	}}
	printf("sae ben do bucle\n");

	// 3)int open_broadcast_udp_socket(const char *if_name, const int port); loop that waits for net_events to occur...
	ev_loop(net_events->loop, 0);
printf("ultimo!\n");
	// 4) program finalization
	exit(EXIT_SUCCESS);

}
