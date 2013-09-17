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
#include "itsnet_header.h"


struct tq_elem
{
	struct list_head list;
	struct timespec expires;       	/* expire time for task */
	pthread_t thread;		/* who queued this task */
	void (*task)(struct tq_elem *);	/* pointer to task      */
};

struct itsnet_node_
{

	struct mac_addr mac_id;
	struct itsnet_node_id node_id;			/** node identity		*/
	struct tq_elem tqe;                    /** Timer queue entry */
	struct timespec expires;       		/** expire time for message 	*/
	struct itsnet_position_vector pos_vector;	/** position vector		*/
	bool LS_PENDING;
	bool IS_NEIGHBOUR;
	int Sequence_number;
	//struct list_head list;   			/** list head 			*/
};
typedef struct itsnet_node_ itsnet_node;

struct ElementList_locT
{
  itsnet_node data;
  struct ElementList_locT *next;
};

typedef struct ElementList_locT Element_locT;

struct List_locT
{
  Element_locT *init;
  Element_locT *end;
  int len;
};

typedef struct List_locT List_locT;

struct ElementList_lsp
{
  itsnet_node data;
  struct ElementList_lsp *next;
};
typedef struct ElementList_lsp Element_lsp;

struct ListIdent_lsp
{
  Element_lsp *init;
  Element_lsp *end;
  int len;
} ;

typedef struct ListIdent_lsp List_lsp;

bool exist_neighbours();

int sup_elem_locT ( int pos);
int add_end_locT ( List_locT * locT, itsnet_node data);

List_locT * init_locT ();
void view_locT ();


int sup_elem_lsp ( int pos);
int add_end_lsp ( List_lsp * locT, itsnet_node data); //hai que cambiar o itsnet_node
List_lsp * init_lsp ();

void view_lsp ();

List_locT * startup1();


void LPV_update();
void Time_update();

void Beacon_send();
void LS(int option);

#endif /* NETMANAGEMENT_H_ */
