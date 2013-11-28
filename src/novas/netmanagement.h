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
#include "../gps.h"
#include "../gpsdclient.h"
#include "configuration.h"
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
 #define IEEE_80211_BLEN 		2312	/*!< IEEE 802.11 body length (B). */
typedef struct List_locT List_locT;
typedef struct List_lsp List_lsp;
typedef struct msghdr msg_header_t;
typedef struct sockaddr_in sockaddr_in_t;		/*!< Type for sockaddr_in. */
typedef struct sockaddr_ll sockaddr_ll_t;       /*!< Type for sockaddr_ll. */
typedef struct gps_data_t gps_data_t;           /*!< Type for gps_data_t. */
typedef struct sockaddr sockaddr_t;			/*!< Type for sockaddr. */
struct public_ev_arg
{

	int socket_fd;                                        /**< Socket file descriptor. */
	        int port;                                                /**< Port to be used. */
	        int forwarding_socket_fd;                /**< Socket for message forwarding. */
	        int net_socket_fd;
	        int forwarding_port;                        /**< Port for message forwarding. */
	        int net_port;
	        sockaddr_in_t *forwarding_addr;        /**< Forwarding address. */
	        sockaddr_ll_t *forwarding_addr_ll;        /**< Forwarding address. */
	        sockaddr_in_t *local_addr;                /**< Local address (NOT localhost) */
	        sockaddr_ll_t *local_addr_ll;                /**< Local address (NOT localhost) */
	        bool print_forwarding_message;        /**< Flag that enables verbose. */
	        void *data;                                                /**< Buffer for frames reception. */
	        int len;                                                /**< Data length within the buffer. */
	        msg_header_t *msg_header;                /**< Buffer for msg_header reception. */
	        bool nec_mode;                                        /**< Flag that indicates NEC mode. */
	        int __test_number;                                /**< For testing, counts no tests. */
	        List_locT *locT;
	        List_lsp *lsp;
	        List_lsp *rep;
	        bool gn;
};


typedef struct public_ev_arg public_ev_arg_r;
/**struct tq_elem
{
	struct list_head list;
	struct timespec expires;       	// expire time for task
	pthread_t thread;		// who queued this task
	void (*task)(struct tq_elem *);	// pointer to task
};
**/
struct itsnet_node_//loct entry
{
	struct itsnet_node_id node_id;			/** node identity		*/
	struct mac_addr mac_id;
//	unsigned char tstation:1;
	 int itss_type;
	int version;
	 struct itsnet_position_vector pos_vector;	/** position vector		*/
	bool LS_PENDING;
	bool IS_NEIGHBOUR;
	uint16_t Sequence_number;
	//struct tq_elem tqe;                    /** Timer queue entry */
	struct timespec expires;       		/** expire time for message 	*/
	int pdr; // packet data rate
	};
typedef struct itsnet_node_ itsnet_node;
/**
 * @struct ElementList_locT
 * @brief Structure for ElementList_locT, elements of locT.
 */
struct ElementList_locT
{	itsnet_node data;
	struct ElementList_locT *next;
	struct ElementList_locT *before;};
typedef struct ElementList_locT Element_locT;
/**
 * @struct LT
 * @brief Structure for lifetime. There are two parts in the coded way.
 */
struct LT
{unsigned char base:2;
unsigned char multiple:6;
};
typedef struct LT LT_s;
/**
 * @struct List_locT
 * @brief Structure for loct, marks init and end of it.
 */
struct List_locT
{	Element_locT *init;
	Element_locT *end;
	int len;};
/**
 * @struct ElementList_lsp
 * @brief Structure for ElementList_lsp, elements of lsp.
 */
struct ElementList_lsp
{	itsnet_packet data;
	struct ElementList_lsp *next;
	struct ElementList_lsp *before;};
typedef struct ElementList_lsp Element_lsp;
/**
 * @struct List_lsp
 * @brief Structure for lsp, marks init and end of it. Maintain also it size to satisfy the limitation
 */
struct List_lsp
{	Element_lsp *init;
	Element_lsp *end;
	int size;
	int len;} ;
/**
 * @struct ieee80211_header
 * @brief Structure for ieee80211_header, macs and type of packet
 */

typedef struct ieee80211_header
{	unsigned char dest_address[ETH_ALEN];   /*!< Dest MAC address. */
	unsigned char src_address[ETH_ALEN];	/*!< Source MAC address. */
	unsigned char type[2];					/*!<0x0707 of GN. */
} ieee80211_header_t ;
/**
 * @struct ieee80211_frame_buffer
 * @brief Structure for ieee80211_frame_buffer, header and data
 */
typedef struct ieee80211_frame_buffer
{
	ieee80211_header_t header;	/*!< IEEE 802.11 header. */
    	char data[IEEE_80211_BLEN];	/*!< Data body of the IEEE 802.11 frame. */
} ieee80211_buffer_t;

typedef struct ll_frame
{	int frame_type;				/*!< Type of the frame. */
	int frame_len;				/*!< Length of the total bytes read. */
	struct timeval timestamp;	/*!< Frame creation timestamp (usecs). */
} ll_frame_t;

typedef struct ieee80211_frame
{	ll_frame_t info;			/*!< Info relative to frame management. */
	ieee80211_buffer_t buffer;	/*!< Buffer with frame contents. */
} ieee80211_frame_t;
/**
 * @fn sup_elem_locT
 * @brief This function erase an element of loct list.
 * @param num, we use this to know what element of timer list related with loct list we have to erase
 * @param mac of the element we want to erase
 * @param list where we erase the element
  */
int sup_elem_locT (int num, struct mac_addr *pos,List_locT *locT);
/**
 * @fn search_in_locT
 * @brief This function search an element of loct list.
 * @param data, we use this information to make the search
 * @param list where we look for the element
 * @return the number of position in loct list
  */
int search_in_locT (itsnet_node * data, List_locT * locT);

/**
 * @fn sup_elem_lsp
 * @brief This function erase last element of lsp list.
 * @param num, not necessary
  */
int sup_elem_lsp (int num);
/**
 * @fn add_end_locT
 * @brief This function add an element of loct list.
 * @param list where we put the new element
 * @param data is the information of the new element
  */
int add_end_locT ( List_locT * locT, itsnet_node data);
/**
 * @fn add_end_lsp
 * @brief This function add an element of lsp list.
 * @param list where we put the new element
 * @param data is the information of the new element
  */
int add_end_lsp ( List_lsp * lsp, itsnet_packet data);
/**
 * @fn add_end_rep
 * @brief This function add an element of rep list.
 * @param list where we put the new element
 * @param data is the information of the new element
  */
int add_end_rep ( List_lsp *rep, itsnet_packet data);
/**
 * @fn view_lsp
 * @brief This function permits view lsp list of TimeId.
 */
void view_lsp ();
/**
 * @fn view_locT
 * @brief This function permits view loct list of mac.
 */
void view_locT ();
List_locT * startup1(configuration_t *cfg);
/**
 * @fn LPV_update
 * @brief LPV_update This function is the Callback when LPV timer breaks! use gps information to create
 * new LPV.
  */
itsnet_position_vector * LPV_update(EV_P_ ev_timer *w, int revents);
/**
 * @fn Beacon_send
 * @brief Beacon_send This function is the Callback when Beacon timer breaks! we need to send a beacon to
 * announce our mac to the other hosts,so as they can annotate it on their loct.
  */

void Beacon_send(EV_P_ ev_timer *w, int revents) ;
int duplicate_control(void * data,List_locT * locT);
/**
 * @fn LS
 * @brief LS hasn't been implemented yet
  */
void LS(int option);
/**
 * @struct Timer
 * @brief Structure for elements in timer list. Here we can annotate Period we will timer,
 *  next and before pointer and TimerId(sn or number unsigned short of loct)
 * .
 */
typedef struct Timer
{
	unsigned short TimerId;
	int Period;
	struct Timer *pNext;
	struct Timer * before;
}tTimer;
/**
 * @struct List_timer
 * @brief Structure for timer list, marks init and end of it.
 */
struct List_timer
{
	tTimer *init;
	tTimer *end;
	int len;
};
typedef struct List_timer List_timer;
/**
 * @fn sup_timer
 * @brief This function erase an element of lsp or loct timer list.
 * @param TimerId is the name of timer, sn in case of lsp or number unsigned short in loct case
 * @param type tell us if the element is lsp (2) or loct (1)
 * @return the list pointer
  */
int sup_timer (unsigned short TimerId, int type);
/**
 * @fn init_lsp
 * @brief This function create a new list of lsp, it's the list where we maintain frames we
 *  didn't send because there aren't neighbours.
 * @return the list pointer
  */
List_lsp * init_lsp ();
/**
 * @fn init_locT
 * @brief This function create a new list of loct, it's the list where we maintain macs we receive
 * @return the list pointer
  */
List_locT * init_locT ();
itsnet_position_vector * LPV_ini();
/**
 * @fn init_timer_list
 * @brief This function create a new list of timers.
 * @return the list pointer
  */
List_timer * init_timer_list ();
/**
 * @fn AddTimer
 * @brief This function create a new element in the list of timers.
 * @param TimerId is the name of timer, sn in case of lsp or number unsigned short in loct case
 * @param time tell us lifetime of element we will timer
 * @param type tell us if the element is lsp (2) or loct (1)
 * @return if the addition was done correctly
  */
bool AddTimer(unsigned short TimerId, int time, int type);
/**
 * @fn FindTimer
 * @brief This function find a timer of an element of lsp or loct to know if it exist and update
 * it or create a new timer in case it don't exist.
 * @param TimerId is the name of timer, sn in case of lsp or number unsigned short in loct case
 * @param type tell us if the element is lsp (2) or loct (1)
 * @return position of the searched element or NULL in case it doesn't exist
  */
tTimer * FindTimer(unsigned short TimerId, int type);
/**
 * @fn SystemTickEvent
 * @brief When a second passed this function is called by alarm signal to
 * substract 1 to timers of elements of loct and lsp. If one of them arrive to zero a SIGUSR2
 * or SIGUSR1 is rised to call another function (CheckTimerEvent_lsp or CheckTimerEvent)

 */
void SystemTickEvent(void);
/**
 * @fn CheckTimerEvent_lsp
 * @brief When one of the elements of lsp finishes its timer a signal SIGUSR2 is rised and
 * this function is called to call sup_elem_t_lsp,
 * to erase the timer of the element we erase.

 */
void CheckTimerEvent_lsp(EV_P_ ev_timer *w, int revents);
/**
 * @fn CheckTimerEvent_lsp
 * @brief When one of the elements of loct finishes its timer a signal SIGUSR1 is rised and
 * this function is called to call sup_elem_loct
 */

void CheckTimerEvent(EV_P_ ev_timer *w, int revents);
/**
 * @fn thr_h2
 * @brief This is the thread we use to timer lifetime of locT elements and lsp elements.
 * @param public_arg Public arguments for this callback function.

 */
void thr_h2(void *arg);
#endif /* NETMANAGEMENT_H_ */
