/**
 * @file udp_socket.c
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

#include "udp_socket.h"
#include <sys/socket.h>
const unsigned char ETH_ADDR_BROADCAST[ETH_ALEN]                    ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
const unsigned char ETH_ADDR_ANY[ETH_ALEN]                                    ={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned char ETH_ADDR_FAKE[ETH_ALEN]= { 0x00,0x18,0x39,0xAE,0x7D,0xD5 };
const unsigned char ETH_ADDR_NULL[ETH_ALEN]   = { 0x00,0x00,0x00,0x00,0x00,0x00 };



int get_mac_address
	(const int socket_fd, const char *if_name ,const unsigned char *mac)
{

	int len_if_name = -1;

	if ( if_name == NULL )		{ return(EX_NULL_PARAM);printf("no"); }
//if ( mac == NULL )		{ return(EX_NULL_PARAM);printf("no"); }

	if ( socket_fd < 0 )		{ return(EX_WRONG_PARAM);printf("no"); }

	len_if_name = strlen(if_name);

	if ( len_if_name == 0 )		{ return(EX_EMPTY_PARAM);printf("no"); }
	if ( len_if_name > IF_NAMESIZE )		{ return(EX_WRONG_PARAM);printf("no"); }

	ifreq_t *ifr = new_ifreq();
	strncpy(ifr->ifr_name, if_name, len_if_name);

	if ( ioctl(socket_fd, SIOCGIFHWADDR, ifr) < 0 )	{		log_sys_error("Could not get interface index");		return(EX_SYS);printf("no");}


	memcpy(mac, ifr->ifr_hwaddr.sa_data, ETH_ALEN);
	return(EX_OK);

}


/* new_ifreq */
ifreq_t *new_ifreq()
{

	ifreq_t *s = NULL;

	if ( ( s = (ifreq_t *)malloc(LEN__IFREQ) ) == NULL )
		{ handle_sys_error("new_ifreq: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__IFREQ) == NULL )
		{ handle_sys_error("new_ifreq: <memset> returns NULL.\n"); }

	return(s);

}

/* init_ifreq */
ifreq_t *init_ifreq(const char *if_name)
{

	ifreq_t *s = new_ifreq();
	int if_name_len = 0;
	int ifr_name_len = sizeof(s->ifr_name);

	if ( ( if_name_len = strlen(if_name) ) < 0 )
		{ handle_sys_error("init_ifreq: <strlen> returns < 0.\n"); }
	if ( if_name_len > ifr_name_len )
		{ handle_app_error("init_ifreq: if_name's length bigger than ifr's " \
							"buffer.\n"); }

	if ( strncpy(s->ifr_name, if_name, if_name_len) == NULL )
		{ handle_sys_error("init_ifreq: <strncpy> returns NULL.\n"); }

	return(s);

}

/* new_sockaddr */
sockaddr_t *new_sockaddr()
{

	sockaddr_t *s = NULL;

	if ( ( s = (sockaddr_t *)malloc(LEN__SOCKADDR) ) == NULL )
		{ handle_sys_error("new_sockaddr: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__SOCKADDR) == NULL )
		{ handle_sys_error("new_sockaddr: <memset> returns NULL.\n"); }

	return(s);

}

/* new_sockaddr_in */
sockaddr_in_t *new_sockaddr_in()
{

	sockaddr_in_t *s = NULL;

	if ( ( s = (sockaddr_in_t *)malloc(LEN__SOCKADDR_IN) ) == NULL )
		{ handle_sys_error("new_sockaddr_in: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__SOCKADDR) == NULL )
		{ handle_sys_error("new_sockaddr_in: <memset> returns NULL.\n"); }

	return(s);

}

/* new_sockaddr_ll */
sockaddr_ll_t *new_sockaddr_ll()
{
	sockaddr_ll_t *buffer = NULL;
	buffer = (sockaddr_ll_t *)malloc(LEN__SOCKADDR_LL);
	memset(buffer, 0, LEN__SOCKADDR_LL);
	return(buffer);
}





/* new_iovec */
iovec_t *new_iovec()
{

	iovec_t *s = NULL;

	if ( ( s = (iovec_t *)malloc(LEN__IOVEC) ) == NULL )
		{ handle_sys_error("new_iovec: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__IOVEC) == NULL )
		{ handle_sys_error("new_iovec: <memset> returns NULL.\n"); }

	return(s);

}

/* new_msg_header */
msg_header_t *new_msg_header()
{

	msg_header_t *s = NULL;

	if ( ( s = (msg_header_t *)malloc(LEN__MSG_HEADER) ) == NULL )
		{ handle_sys_error("new_msg_header: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__MSG_HEADER) == NULL )
		{ handle_sys_error("new_msg_header: <memset> returns NULL.\n"); }

	if ( ( s->msg_control = malloc(CONTROL_BUFFER_LEN) ) == NULL )
		{ handle_sys_error("init_msg_header: <malloc> returns NULL.\n"); }
	if ( memset(s->msg_control, 0, CONTROL_BUFFER_LEN) == NULL )
		{ handle_sys_error("init_msg_header: <memset> returns NULL.\n"); }
	s->msg_controllen = CONTROL_BUFFER_LEN;

	s->msg_flags = 0;

	s->msg_name = new_sockaddr_in();
	s->msg_namelen = LEN__SOCKADDR_IN;

	s->msg_iov = new_iovec();
	s->msg_iovlen = 0;

	return(s);

}

/* init_msg_header */
msg_header_t *init_msg_header(void* buffer, const int buffer_len)
{

	msg_header_t *s = new_msg_header();

	s->msg_iovlen = 1;
	s->msg_iov->iov_base = buffer;
	s->msg_iov->iov_len = buffer_len;

	return(s);

}

/* init_broadcast_sockaddr_in */
sockaddr_in_t *init_sockaddr_in(const int port,in_addr_t addr)
{

	sockaddr_in_t *s = new_sockaddr_in();

	s->sin_family = AF_INET;
	s->sin_port = (in_port_t)htons(port);
	s->sin_addr.s_addr = addr;//htonl(INADDR_BROADCAST);

	return(s);
}


sockaddr_ll_t *init_broadcast_sockaddr_ll(const int port)
{

	sockaddr_ll_t *s = new_sockaddr_ll();


	s->sll_family = PF_PACKET;
	s->sll_ifindex = if_nametoindex("wlan0");

	memcpy(s->sll_addr,ETH_ADDR_BROADCAST , ETH_ALEN);
s->sll_protocol=htons(0x0707);
	return(s);

}
/* init_any_sockaddr_in */
sockaddr_in_t *init_any_sockaddr_in(const int port)
{

	sockaddr_in_t *s = new_sockaddr_in();

	s->sin_family = AF_INET;
	s->sin_port = (in_port_t)htons(port);
	s->sin_addr.s_addr = htonl(INADDR_ANY);

	return(s);

}

sockaddr_ll_t *init_any_sockaddr_ll(const int port) //af inet? no creo
{

	sockaddr_ll_t *s = new_sockaddr_ll();

	s->sll_family = PF_PACKET;
	s->sll_ifindex = (in_port_t)htons(port);//??
	memcpy(s->sll_addr,ETH_ADDR_ANY , ETH_ALEN);

	return(s);

}




sockaddr_ll_t *init_sockaddr_ll(const char *address, const int port)
{


	//	s.sll_ifindex  = if_index;//if_nametoindex(if_index);
	sockaddr_ll_t *s = new_sockaddr_ll();
	s->sll_family = PF_PACKET;
	s->sll_halen = ETH_ALEN;
	//memset(s->sll_addr, 0xff, ETH_ALEN);
	//s->sin_port = (in_port_t)htons(port);
	s->sll_ifindex =if_nametoindex("wlan0");//if_index; -> FALTA DAR ESTA INFO
	//s->sll_protocol=htons(port);
	s->sll_protocol = htons(ETH_P_ALL);	// htons(0x0707);//engadido
	memset(s->sll_addr,address,ETH_ALEN);
//	if ( ( s->sll_addr.s_addr = inet_addr(address) ) < 0 )
	//	{ handle_sys_error("init_sockaddr_in: " \

	//					"<inet_addr> returns error.\n"); }
	return(s);


	//	if (is_transmitter) {t->sll_protocol = htons(ll_socket->ll_sap);}else t->sll_protocol=htons(ETH_P_ALL);


}


/* init_if_sockaddr_in */
sockaddr_ll_t *init_if_sockaddr_ll(const char *if_name, const int port)
{

	sockaddr_ll_t *s = NULL;
	struct ifaddrs *ifaddr, *ifa;
    int i;
    char host[NI_MAXHOST];
    bool if_found = false;

    if ( getifaddrs(&ifaddr) < 0 )
    {
    	handle_sys_error("init_if_sockaddr_ll: " \
    						"<getifaddrs> returned error. Error: ");
    }

    for ( ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next )
    {

        if ( ifa->ifa_addr == NULL ) { continue; }

        i = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_ll)
        					, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if ( 	( strcmp(ifa->ifa_name,if_name) == 0 ) &&
        		( ifa->ifa_addr->sa_family == AF_INET )		)
        {

            if ( i != 0 )
            {
                handle_app_error("init_if_sockaddr_in: " \
                					"<getnameinfo> failed: %s\n"
                						, gai_strerror(i));
            }

            s = init_sockaddr_ll(host, port);
            if_found = true;

        }

    }

    freeifaddrs(ifaddr);

    if ( if_found == false )
    {
    	handle_app_error("Could not get local interface, if_name = %s.\n"
    						, if_name);
    }

	return(s);

}



/* init_if_sockaddr_in */
sockaddr_in_t *init_if_sockaddr_in(const char *if_name, const int port)
{

	sockaddr_in_t *s = NULL;
	struct ifaddrs *ifaddr, *ifa;
    int i;
    char host[NI_MAXHOST];
    bool if_found = false;

    if ( getifaddrs(&ifaddr) < 0 )
    {
    	handle_sys_error("init_if_sockaddr_in: " \
    						"<getifaddrs> returned error. Error: ");
    }

    for ( ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next )
    {

        if ( ifa->ifa_addr == NULL ) { continue; }

        i = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in)
        					, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if ( 	( strcmp(ifa->ifa_name,if_name) == 0 ) &&
        		( ifa->ifa_addr->sa_family == AF_INET )		)
        {

            if ( i != 0 )
            {
                handle_app_error("init_if_sockaddr_in: " \
                					"<getnameinfo> failed: %s\n"
                						, gai_strerror(i));
            }

            s = init_sockaddr_in(host, port);
            if_found = true;

        }

    }

    freeifaddrs(ifaddr);

    if ( if_found == false )
    {
    	handle_app_error("Could not get local interface, if_name = %s.\n"
    						, if_name);
    }

	return(s);

}

/* open_receiver_udp_socket */
int open_receiver_udp_socket(const int port)
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_receiver_udp_socket: " \
							"<socket> returns error. Description"); }

	// 2) local address for binding
	sockaddr_in_t* addr = init_any_sockaddr_in(port);
	if ( bind(fd, (sockaddr_t *)addr, LEN__SOCKADDR_IN) < 0 )
		{ handle_sys_error("open_receiver_udp_socket: " \
							"<bind> returns error. Description"); }

	// 3) for analyzing received message's headers
	if ( set_msghdrs_socket(fd) < 0 )
		{ handle_app_error("open_receiver_udp_socket: " \
							"<set_msghdrs_socket> returns error.\n"); }


	return(fd);

}



int open_receiver_raw_socket(const int port)
{

	int fd = -1;

	// 1) socket creation
	sockaddr_ll_t *s= init_sockaddr_ll((const char *) NULL, port);


	if ( ( fd = socket(PF_PACKET, SOCK_RAW,0 ) ) < 0 )// lsap é port
		{ handle_sys_error("open_receiver_RAW_socket: " \
							"<socket> returns error. Description"); }
	printf("what\n");
	// 2) local address for binding
	//sockaddr_ll_t* addr = init_any_sockaddr_ll(port);
	//if ( bind(fd, (sockaddr_t *)addr, LEN__SOCKADDR_LL) < 0 )
	//	{ handle_sys_error("open_receiver_RAW_socket: " \
		//					"<bind> returns error. Description"); }

	// 3) for analyzing received message's headers
	//if ( set_msghdrs_socket(fd) < 0 ) //PROBABLEMENTE BORRE ESTO
		//{ handle_app_error("open_receiver_RAW_socket: " \
							"<set_msghdrs_socket> returns error.\n"); }

	return(fd);

}

/* open_transmitter_udp_socket */
int open_transmitter_udp_socket(const int port, in_addr_t addr) //non está completo, non?
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }
/**
//	sockaddr_in_t server;
	//	 memset(&server, 0, sizeof server);
	//	    server.sin_family = AF_INET; // Use IPv4
	//	    server.sin_addr.s_addr = addr; // My IP
	//	    server.sin_port = port; // Server Port

		    // Bind socket
//		    if ((bind(fd, (struct sockaddr *) &server, sizeof(server))) == -1) {
//		    	close(fd);
//		    	perror("Can't bind");
	//	    }***/

	return(fd);

}


int open_transmitter_raw_socket(const int port) //non está completo, non?
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_PACKET, SOCK_RAW, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }




	return(fd);

}


/* open_broadcast_udp_socket */
int open_broadcast_udp_socket(const char *iface, const int port)
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }

	// 2) set broadcast socket options
	if ( set_broadcast_socket(fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_broadcast_socket> returns error.\n");
	}

	// 3) broadcast socket must be bound to a specific network interface
	if ( set_bindtodevice_socket(iface, fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_bindtodevice_socket> returns error.\n");
	}

	return(fd);

}


int open_broadcast_raw_socket(const char *iface, const int port) ///cambiar esto a raw
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_PACKET, SOCK_RAW, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }

	// 2) set broadcast socket options
	/*if ( set_broadcast_socket(fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_broadcast_socket> returns error.\n");
	}

	// 3) broadcast socket must be bound to a specific network interface
	if ( set_bindtodevice_socket(iface, fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_bindtodevice_socket> returns error.\n");
	}
*/
	return(fd);

}


/* set_broadcast_socket */
int set_broadcast_socket(const int socket_fd)
{

	int bcast = 1;

	if ( setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(int))
			< 0 )
		{ handle_sys_error("set_broadcast_socket: " \
							"<setsockopt> returns error."); }

	return(EX_OK);

}

/* set_bindtodevice_socket */
int set_bindtodevice_socket(const char *if_name, const int socket_fd)
{

	ifreq_t *ifr = init_ifreq(if_name);

	if ( setsockopt(socket_fd, SOL_SOCKET, SO_BINDTODEVICE, ifr, LEN__IFREQ)
			< 0 )
		{ handle_sys_error("set_bindtodevice_socket: " \
							"<setsockopt> returns error"); }

	return(EX_OK);

}

/* set_msghdrs_socket */
int set_msghdrs_socket(const int socket_fd)
{

	ifreq_t *ifr = new_ifreq();

	if ( setsockopt(socket_fd, IPPROTO_IP, IP_PKTINFO, &ifr, LEN__IFREQ)
			< 0 )
	{ handle_sys_error("set_generate_headers: " \
						"<setsockopt> returns error"); }

	return(EX_OK);



}

ieee80211_frame_t *new_ieee80211_frame()
	{

		ieee80211_frame_t *buffer = NULL;
		buffer = (ieee80211_frame_t *)malloc(LEN__IEEE80211_FRAME);
		memset(buffer, 0, LEN__IEEE80211_FRAME);
		return(buffer);

	}
int set_ll_frame
	(ll_frame_t *frame, const int frame_type, const int frame_len)
{

	frame->frame_type = frame_type;
	frame->frame_len = frame_len;

	if ( gettimeofday(&frame->timestamp, NULL) < 0 )
	{
		log_sys_error("Cannot get timestamp");
		return(EX_ERR);
	}

	return(EX_OK);

}

ieee80211_frame_t *init_ieee80211_frame	(	const int ll_sap, const unsigned char *h_dest, const unsigned char *h_source	)
{

	ieee80211_frame_t *f = new_ieee80211_frame();

	if ( set_ll_frame(&f->info, TYPE_IEEE_80211, ETH_FRAME_LEN) < 0 )
		{ log_app_msg("Could not set info adequately!\n"); }
	//f->buffer.header.h_proto = htons(ETH_P_ALL);//ll_sap;//cambio
	memcpy(f->buffer.header.dest_address, h_dest, ETH_ALEN);
	memcpy(f->buffer.header.src_address, h_source, ETH_ALEN);
	return(f);

}
/* send_message */
int send_message(	const sockaddr_t* dest_addr, const int socket_fd,
					const void *buffer, const int len	)
{

	int sent_bytes = 0;

//teño que engadir aqui a cabeceira


	if ( ( sent_bytes = sendto(socket_fd, buffer, len
								, 0, dest_addr, LEN__SOCKADDR_LL) ) < 0 ) // solo habería que cambiar esta liña
	{
		log_sys_error("cb_broadcast_sendto (fd=%d): <sendto> ERROR.\n"
						, socket_fd);
		getchar();
		return(EX_ERR);
	}

	if ( sent_bytes < len )
	{
		log_app_msg("send_message: sent %d bytes, requested %d.\n"
						, sent_bytes, len);
		return(EX_ERR);
	}

	return(sent_bytes);

}

/* recv_message */
int recv_message(const int socket_fd, void *data)
{

	int rx_bytes = 0;

	if ( ( rx_bytes = recvfrom(socket_fd, data, UDP_BUFFER_LEN, 0, NULL, NULL) ) < 0 )

		//if((rx_bytes= recvfrom(socket_fd, msg,IEEE_80211_FRAME_LEN, 0,NULL,NULL))<0)
	{
		log_sys_error("recv_message: wrong <recvfrom> call. ");
		return(EX_ERR);
	}

	return(rx_bytes);

}
#define IEEE_80211_FRAME_LEN	2343	/*!< IEEE 802.11 frame length (B). */
/* recv_msg */
int recv_msg(	const int socket_fd, msg_header_t *msg, //FACER TAMÉN UNHA VERSION NET (RAW) DO SEND
				const in_addr_t block_ip, bool *blocked	)
{

	int rx_bytes = 0;

	// 1) read UDP message from network level
	if ( ( rx_bytes = recvmsg(socket_fd, msg, 0) ) < 0 ) //ESTO HAI QUE CAMBIALO!!!

	{
		log_sys_error("recv_msg: wrong <recvmsg> call. ");
		return(EX_ERR);
	}

	in_addr_t src_addr = get_source_address(msg);

	if ( block_ip == src_addr )
		{ *blocked = true; }
	else
		{ *blocked = false; }

	return(rx_bytes);

}

/* get_source_address */
in_addr_t get_source_address(msg_header_t *msg)
{

	sockaddr_in_t *src = NULL;

	// iterate through all the control headers
	for	(
			struct cmsghdr *cmsg = CMSG_FIRSTHDR(msg);
			cmsg != NULL;
			cmsg = CMSG_NXTHDR(msg, cmsg)
		)
	{

		// ignore the control headers that don't match what we want
	    if (	( cmsg->cmsg_level 	!= IPPROTO_IP ) 	||
	    		( cmsg->cmsg_type 	!= IP_PKTINFO ) )
	    	{ continue; }

	    //struct in_pktinfo *pi = CMSG_DATA(cmsg);
	    src = (sockaddr_in_t *)msg->msg_name;
	    break;

	}

	return(src->sin_addr.s_addr);

}

/* print_hex_data */
int print_hex_data(const char *buffer, const int len)
{

	int last_byte = len - 1;
	if ( len < 0 ) { return(EX_WRONG_PARAM); }

	for ( int i = 0; i < len; i++ )
	{
		if ( ( i != 0 ) && ( ( i % BYTES_PER_LINE ) == 0 ) )
			{ log_app_msg("\n\t\t\t"); }
		log_app_msg("%02X", 0xFF & (unsigned int)buffer[i]);
		if ( i < last_byte ) { log_app_msg(":"); }
	}

	return(EX_OK);

}

/* print_eth_address */
void print_eth_address(const unsigned char *eth_address)
{

	printf("%02X:%02X:%02X:%02X:%02X:%02X",
  			(unsigned char) eth_address[0],
  			(unsigned char) eth_address[1],
  			(unsigned char) eth_address[2],
  			(unsigned char) eth_address[3],
  			(unsigned char) eth_address[4],
  			(unsigned char) eth_address[5]);

}
