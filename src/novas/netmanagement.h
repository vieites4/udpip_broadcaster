/*
 * netmanagement.h
 *
 *  Created on: 04/09/2013
 *      Author: pc
 */

#ifndef NETMANAGEMENT_H_
#define NETMANAGEMENT_H_
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>

#include "/home/pc/Descargas/gpsd-3.9/gps.h"
#include "/home/pc/Descargas/gpsd-3.9/gpsdclient.h"
#include <ev.h>
#include "itsnet_header.h"
#include "../udpev/define.h"

#define TIMER_1	            (unsigned short)0x0001
#define TIMER_2	            (unsigned short)0x0002
#define TIMER_3	            (unsigned short)0x0004
#define TIMER_4	            (unsigned short)0x0008
#define TIMER_5	            (unsigned short)0x0010
#define TIMER_6	            (unsigned short)0x0020
#define TIMER_7	            (unsigned short)0x0040
#define TIMER_8	            (unsigned short)0x0080
#define TIMER_9	            (unsigned short)0x0100
#define TIMER_10	        (unsigned short)0x0200
#define TIMER_11	        (unsigned short)0x0400
#define TIMER_12	        (unsigned short)0x0800
#define TIMER_13	        (unsigned short)0x1000
#define TIMER_14	        (unsigned short)0x2000
#define TIMER_15	        (unsigned short)0x4000
#define TIMER_16	        (unsigned short)0x8000
//#include "udpev/cb_udp_events.h"
typedef struct List_locT List_locT;
typedef struct msghdr msg_header_t;
typedef struct sockaddr_in sockaddr_in_t;		/*!< Type for sockaddr_in. */
typedef struct gps_data_t gps_data_t;
 struct public_ev_arg
{

	int socket_fd;					/**< Socket file descriptor. */
	int port;						/**< Port to be used. */

	int forwarding_socket_fd;		/**< Socket for message forwarding. */
	int forwarding_port;			/**< Port for message forwarding. */

	sockaddr_in_t *forwarding_addr;	/**< Forwarding address. */
	sockaddr_in_t *local_addr;		/**< Local address (NOT localhost) */

	bool print_forwarding_message;	/**< Flag that enables verbose. */

	void *data;						/**< Buffer for frames reception. */
	int len;						/**< Data length within the buffer. */

	msg_header_t *msg_header;		/**< Buffer for msg_header reception. */

	bool nec_mode;					/**< Flag that indicates NEC mode. */

	int __test_number;				/**< For testing, counts no tests. */
	List_locT *locT;
};


typedef struct public_ev_arg public_ev_arg_r;
struct tq_elem
{
	struct list_head list;
	struct timespec expires;       	/* expire time for task */
	pthread_t thread;		/* who queued this task */
	void (*task)(struct tq_elem *);	/* pointer to task      */
};

struct itsnet_node_
{
	struct itsnet_node_id node_id;			/** node identity		*/
	struct mac_addr mac_id;
	unsigned char tstation:1;
	struct itsnet_position_vector pos_vector;	/** position vector		*/
	bool LS_PENDING;
	bool IS_NEIGHBOUR;
	int Sequence_number;
	struct tq_elem tqe;                    /** Timer queue entry */
	struct timespec expires;       		/** expire time for message 	*/

	//struct list_head list;   			/** list head 			*/
};
typedef struct itsnet_node_ itsnet_node;

struct ElementList_locT
{
  itsnet_node data;
  struct ElementList_locT *next;
  struct ElementList_locT *before;
};

typedef struct ElementList_locT Element_locT;

struct List_locT
{
  Element_locT *init;
  Element_locT *end;
  int len;
};



struct ElementList_lsp
{
	itsnet_packet data;
  struct ElementList_lsp *next;
};
typedef struct ElementList_lsp Element_lsp;

struct ListIdent_lsp
{
  Element_lsp *init;
  Element_lsp *end;
  int len;
} ;
struct parametros{
Element_locT * pos;
unsigned short num;
//pthread_t thread;
List_locT * locT;
};
typedef struct sockaddr sockaddr_t;			/*!< Type for sockaddr. */
typedef struct ieee80211_header
{

	unsigned char dest_address[ETH_ALEN]; //é importante a orde na que coloco os compoñentes do struct
	unsigned char src_address[ETH_ALEN];	/*!< Source MAC address. */
    unsigned char type[2];

//	unsigned char recheo[28];
} ieee80211_header_t ;
typedef struct ieee80211_frame_buffer
{

	ieee80211_header_t header;	/*!< IEEE 802.11 header. */
	//int frame_type;				/*!< Type of the frame. */
		//int frame_len;
	//	unsigned char recheo[4];

	//ll_frame_t info;
#define IEEE_80211_BLEN 		2312	/*!< IEEE 802.11 body length (B). */
	char data[IEEE_80211_BLEN];	/*!< Data body of the IEEE 802.11 frame. */

} ieee80211_buffer_t;
typedef struct ll_frame
{

	int frame_type;				/*!< Type of the frame. */
	int frame_len;				/*!< Length of the total bytes read. */
//unsigned char recheo[26];
	struct timeval timestamp;	/*!< Frame creation timestamp (usecs). */

} ll_frame_t;

typedef struct ieee80211_frame
{

	ll_frame_t info;			/*!< Info relative to frame management. */
	ieee80211_buffer_t buffer;	/*!< Buffer with frame contents. */

} ieee80211_frame_t;

typedef struct ListIdent_lsp List_lsp;

bool exist_neighbours(List_locT * locT);

int sup_elem_locT (struct parametros *p);//EV_P_ ev_timer *w, int revents);//void * arg);//EV_P_ ev_timer *w,
int add_end_locT ( List_locT * locT, itsnet_node data);

List_locT * init_locT ();
void view_locT (List_locT * locT);
int search_in_locT (itsnet_node * data, List_locT * locT);

int sup_elem_lsp ( int pos);
int add_end_lsp ( List_lsp * lsp, itsnet_packet data); //hai que cambiar o itsnet_node
List_lsp * init_lsp ();

void view_lsp ();

List_locT * startup1();

itsnet_position_vector * LPV_ini();
itsnet_position_vector * LPV_update(EV_P_ ev_timer *w, int revents);
void Beacon_send(EV_P_ ev_timer *w, int revents) ;//void Beacon_send(public_ev_arg_r *arg);
int duplicate_control(void * data,List_locT * locT);

//void *thr_h2(void * arg);
void LS(int option);



/* Timer record for each timer */
typedef struct Timer
{
	unsigned short TimerId;
	int Period;
	struct Timer *pNext;
}tTimer;

bool AddTimer(unsigned short TimerId);
static tTimer * FindTimer(unsigned short TimerId);
void handler_tempo(int sig);
void Timer2Function();

#endif /* NETMANAGEMENT_H_ */
