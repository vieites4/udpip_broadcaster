#include "netmanagement.h"
#include <sys/time.h>
const unsigned char ETH_BROAD[ETH_ALEN] ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
const unsigned char ZEROS[ETH_ALEN] ={ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
const unsigned char TWOS[ETH_ALEN] ={ 0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
extern List_lsp * lsp_bc_g;
extern List_lsp * lsp_uc_g;
extern List_lsp *lsp_cbf_uc;
extern List_lsp *lsp_repetition;
extern List_lsp * ls_buffer;
extern itsnet_node_id GN_ADDR;
extern int PDR;
extern uint16_t SN_g;
int addtimernum=0;int gpsdatatnum=0;int LPV_oldnum=0;int LPVnum=0;int locTelementnum=0;int lspnum=0;
itsnet_position_vector * LPV;
itsnet_position_vector *LPV_old;
ev_timer t_Loct;
unsigned short dictionary[17]={0x00,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
mac_addr *mac_list[16];
List_locT *locT_general;
bool taken[17]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
List_timer *mpTimerList;
List_timer *mpTimerList_repetition_max;
List_timer *mpTimerList_repetition;
List_timer *mpTimerList_pending;
List_timer *mpTimerList_uc;
List_timer *mpTimerList_lsp;
List_timer *mpTimerList_ls_rtx;
List_timer *mpTimerList_ls_buff;
List_timer *mpTimerList_cbf_uc;
extern const unsigned char tsb0[1];
extern const unsigned char tsb1[1];
extern const unsigned char geobroad0[1];
extern const unsigned char geobroad1[1];
extern const unsigned char geobroad2[1];
extern const unsigned char geoanycast0[1];
extern const unsigned char geoanycast1[1];
extern const unsigned char geoanycast2[1];
extern const unsigned char geounicast[1];
extern const unsigned char ls0[1];
extern const unsigned char ls1[1];
//Global variable for reference
static unsigned short gTimer;
int i=0;
static int gTimer_lsp[1000];
static int gTimer_ls_rtx[1000];
static int gTimer_uc[100];
static int gTimer_pending[100];
static int gTimer_repetition_max[100];
static int gTimer_cbf_uc[100];
static int gTimer_repetition[100];
char t07[2]={0x07,0x07};
static int gTimer_ls_buff[1000];
#pragma inline SystemTickEvent,SystemTickEvent_lsp
extern  time_t PDR_ini;
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
int PDR_update(void * data)
{
	char source[6];
	memcpy(source, data, 6);
	time_t PDR_t_new = time(0);
	Element_locT * pos=locT_general->init;
	int same=0;
	while(same==0 && pos!=NULL){if(memcmp(source,pos->data.mac_id.address,6)){	same=1;}else pos=pos->next;}
	if(pos==NULL){PDR=PDR_t_new;return(0);}else{PDR=PDR_t_new;
		if ((PDR_t_new-pos->data.tpdr) ==0){if (pos->data.tpdr!=0){pos->data.pdr=(0.5*pos->data.pdr) + (0.5);}else{pos->data.pdr=1/(PDR_t_new- PDR_ini);}}else{
			if (pos->data.tpdr!=0){pos->data.pdr=(0.5*pos->data.pdr) + (0.5*(1/(PDR_t_new-pos->data.tpdr)));}else{pos->data.pdr=1/(PDR_t_new- PDR_ini);}}

		pos->data.tpdr=PDR_t_new;return(pos->data.pdr);
	}
}

tTimer * FindTimer(unsigned short TimerId,int type)
{
	List_timer * list= NULL;
	if (type==0){list= mpTimerList_lsp;}else if(type==1){list=mpTimerList;}else if(type==4){list=mpTimerList_uc;}else if(type==5){list=mpTimerList_pending;}else if (type==2){list=mpTimerList_ls_rtx;}else if (type==3){list=mpTimerList_ls_buff;}else if (type==6){list=mpTimerList_cbf_uc;}else if (type==7){list=mpTimerList_repetition_max;}else if (type==9){list=mpTimerList_repetition;}

	tTimer *pTimer=list->init;
	tTimer *pTimerC=NULL;
	//PRF("FINDTIMER %d\n",TimerId);
	if(pTimer != NULL)
	{        // Move through the list until the timer id is found
		while((pTimer!= NULL) )
		{            // Timer ids - not match, Move onto the next timer
			if (pTimer->TimerId == TimerId){//PRF("COINCIDENCIA!!!\n");
			pTimerC=pTimer;}
			pTimer = pTimer->pNext;        }    }
	//PRF("saio de find timer\n");
	return (pTimerC);}

bool AddTimer(unsigned short TimerId, int period, int type)

{
	List_timer * list= NULL;
	if (type==0){list= mpTimerList_lsp;}else if(type==1){list=mpTimerList;}else if(type==4){list=mpTimerList_uc;}else if (type==2){list=mpTimerList_ls_rtx;}else if (type==3){list=mpTimerList_ls_buff;}else if (type==6){list=mpTimerList_cbf_uc;}else if (type==7){list=mpTimerList_repetition_max;}else if (type==9){list=mpTimerList_repetition;}else if (type==5){list=mpTimerList_pending;}
	tTimer *pTimer;
	tTimer *new_element = NULL;
	bool ReturnValue = false;
	// Look for the timer – if already exists
	pTimer = FindTimer(TimerId,type);
	// Check if the timer was found
	if((pTimer == NULL) ){
		//PRF("esto si non atopa co find timer, crea un timer novo\n");
		new_element = (tTimer *) malloc(sizeof(tTimer));printf("addtimernum: %d\n",addtimernum);addtimernum++;
		new_element->TimerId = TimerId;
		new_element->pNext = NULL;
		new_element->Period = period;
		new_element->RTC=0;
		if(list->init==NULL){new_element->before=list->init; new_element->pNext=list->end; list->init=new_element;  }
		else {
			new_element->before=list->end;
			list->end->pNext = new_element;}
		list->end = new_element;
		list ->len++;
		// Check if the list is empty
	}else	{	// Set the timer interval
		pTimer->Period = period;
		ReturnValue = true;    }
	if (type==0){mpTimerList_lsp=list;}else if(type==1){mpTimerList=list;}else if (type==2){mpTimerList_ls_rtx=list;}else if (type==3){mpTimerList_ls_buff=list;}else if(type==4){mpTimerList_uc=list;} else if(type==6){mpTimerList_cbf_uc=list;}else if (type==7){mpTimerList_repetition_max=list;}else if (type==9){mpTimerList_repetition=list;}else if (type==5){mpTimerList_pending=list;}
	return ReturnValue;
}


void CheckTimerEvent(EV_P_ ev_timer *w, int revents)
{signal(SIGUSR2, CheckTimerEvent);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
nTimer = gTimer;gTimer = 0;
// Check for TimerId
mac_addr * pos;
if( nTimer != 0)
{ if((nTimer & TIMER_1)!=0)     {	sup_elem_locT(1,mac_list[1],locT_general);    }
if((nTimer & TIMER_2)!=0)       { 	sup_elem_locT(2,mac_list[2],locT_general);    }
if((nTimer & TIMER_3)!=0)       { 	sup_elem_locT(3,mac_list[3],locT_general);    }
if((nTimer & TIMER_4)!=0)       { 	sup_elem_locT(4,mac_list[4],locT_general);    }
if((nTimer & TIMER_5)!=0)     	{ 	sup_elem_locT(5,mac_list[5],locT_general);    }
if((nTimer & TIMER_6)!=0)     	{	sup_elem_locT(6,mac_list[6],locT_general);    }
if((nTimer & TIMER_7)!=0)     	{ 	sup_elem_locT(7,mac_list[7],locT_general);    }
if((nTimer & TIMER_8)!=0)     	{ 	sup_elem_locT(8,mac_list[8],locT_general);    }
if((nTimer & TIMER_9)!=0)     	{  	sup_elem_locT(9,mac_list[9],locT_general);    }
if((nTimer & TIMER_10)!=0)      {	sup_elem_locT(10,mac_list[10],locT_general);  }
if((nTimer & TIMER_11)!=0)      {	sup_elem_locT(11,mac_list[11],locT_general);  }
if((nTimer & TIMER_12)!=0)      {	sup_elem_locT(12,mac_list[12],locT_general);  }
if((nTimer & TIMER_13)!=0)     	{  	sup_elem_locT(13,mac_list[13],locT_general);  }
if((nTimer & TIMER_14)!=0)    	{	sup_elem_locT(14,mac_list[14],locT_general);  }
if((nTimer & TIMER_15)!=0)    	{ 	sup_elem_locT(15,mac_list[15],locT_general);  }
if((nTimer & TIMER_16)!=0)      {	sup_elem_locT(16,mac_list[16],locT_general);  }
}}
void CheckTimerEvent_lsp(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_lsp SIGUSR1\n");
signal(SIGUSR1, CheckTimerEvent_lsp);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_lsp[i]==0){ aa=1;} else {sup_elem_t_lsp(gTimer_lsp[i],0);i++; }
PRF("CheckTimerEvent_lsp SIGUSR1 fin\n");
}}

void CheckTimerEvent_cbf_uc(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_cbf_uc 48\n");
signal(48, CheckTimerEvent_cbf_uc);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
while (aa==0){  if(gTimer_cbf_uc[i]==0){ aa=1;} else {send_bcast_cbf_uc(gTimer_cbf_uc[i],arg);i++; }
PRF("CheckTimerEvent_cbf_uc 48 fin\n");
}}
void CheckTimerEvent_uc(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_uc 47\n");
signal(47, CheckTimerEvent_uc);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_uc[i]==0){ aa=1;} else {sup_elem_t_lsp(gTimer_uc[i],4);i++; }
PRF("CheckTimerEvent_uc 47 fin\n");
}}

void CheckTimerEvent_pending(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_pending 51\n");
signal(51, CheckTimerEvent_pending);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_pending[i]==0){ aa=1;} else {sup_timer(gTimer_pending[i],5);change_pending(gTimer_pending[i]);i++; }
PRF("CheckTimerEvent_pending 51 fin\n");
}}


void CheckTimerEvent_ls_rtx(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_ls_rtx 45\n");
signal(45, CheckTimerEvent_ls_rtx);
unsigned short nTimer;
public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_ls_rtx[i]==0){ aa=1;} else {rtx_ls(gTimer_ls_rtx[i],arg);i++; }
PRF("CheckTimerEvent_ls_rtx 45 fin\n");
}}
void CheckTimerEvent_ls_buff(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_ls_buff 46\n");
signal(46, CheckTimerEvent_ls_buff);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_ls_buff[i]==0){ aa=1;} else {sup_elem_t_lsp(gTimer_ls_buff[i],3);i++; }
PRF("CheckTimerEvent_ls_buff fin\n");
}}

void CheckTimerEvent_repetition_max(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_repetition_max 49\n");
signal(49, CheckTimerEvent_ls_buff);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;
while (aa==0){  if(gTimer_repetition_max[i]==0){ aa=1;} else {sup_elem_t_lsp(gTimer_repetition_max[i],9);sup_timer(gTimer_repetition_max[i],7);i++; }
PRF("CheckTimerEvent_repetition_max fin\n");
}}

void CheckTimerEvent_repetition(EV_P_ ev_timer *w, int revents)
{	PRF("CheckTimerEvent_repetition 50\n");
signal(50, CheckTimerEvent_repetition);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
int aa=0;i=0;public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
while (aa==0){  if(gTimer_repetition[i]==0){ aa=1;} else {rtx_repetition(gTimer_repetition[i],7,arg);i++; }
PRF("CheckTimerEvent_repetition fin\n");
}}

void change_pending(int TimeId){

	Element_locT *aux;	aux = locT_general->init;
	while(aux!=NULL)	{if(TimeId==aux->data.SN1) {aux->data.LS_PENDING=false;}else{ aux=aux->next;}}


}
void rtx_repetition(int num, int type,public_ev_arg_r * arg ){


	//buscar o pkt1 no lsp_repetition, q pasa se é shb??non ten id!!-> idea de solución no 10/1/14
	int aa=0;
	Element_lsp * pos=lsp_repetition->init;
	while(pos!=NULL && aa==0)
	{		if (pos->data.payload.itsnet_unicast.sequencenumber==num)aa=1;else pos=pos->next;	}
	itsnet_packet * pkt1=&pos->data;
	int lon_int =sprint_hex_data(pkt1->common_header.payload_lenght,2);
	char h_source2[ETH_ALEN];
	get_mac_address(arg->net_socket_fd, net_name,(unsigned char *) h_source2) ;
	ieee80211_frame_t *tx_frame1 = init_ieee80211_frame(arg->net_port, ETH_BROAD,h_source2);
	memcpy(tx_frame1->buffer.header.type,t07,2);
	int header_length=0;
	char HT[1];
	memcmp(HT,(char *)&pkt1->common_header.HT_HST,1);
	if(memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0 ||memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0){header_length=44;}
	else if(memcmp(HT,tsb0,1)==0){header_length=28;}else if(memcmp(HT,geounicast,1)==0){header_length=48;} else if(memcmp(HT,ls0,1)==0){header_length=36;}else if(memcmp(HT,ls0,1)==0){header_length=48;}
	memcpy(tx_frame1->buffer.data,(char *)  pkt1,  header_length +lon_int+4+8+4);
	sockaddr_ll_t * dir= init_sockaddr_ll(arg->port);
	send_message((sockaddr_t *)dir,arg->net_socket_fd,&tx_frame1->buffer, header_length +lon_int+14+4+8+4);
	free(pkt1);pkt1=NULL;free(tx_frame1);int e=0;free(dir);
	tTimer * pos_time= mpTimerList_repetition->init;
	while(pos_time!=NULL && e==0){
		if (pos_time->TimerId==num) {pos_time->Period=pos_time->Period_rep;e=1;} else{
			pos_time=pos_time->pNext;}	}
}
int rtx_ls(int num,public_ev_arg_r * arg){

	Element_lsp *position=ls_buffer->init;int e=0;
	tTimer * pos_time= mpTimerList_ls_rtx->init;
	while(pos_time!=NULL && e==0){
		if (pos_time->TimerId==num) e=1; else{
			pos_time=pos_time->pNext;}
	}
	e=0;	while(position!=NULL && e==0){
		if (position->data.payload.itsnet_ls_req.sequencenumber==pos_time->TimerId) e=1; else {position=position->next;}
	}
	Element_locT *pos_locT=locT_general->init;
	e=0;	while(pos_locT!=NULL && e==0){
		if (position->data.payload.itsnet_ls_req.sequencenumber==pos_locT->data.SN1) e=1; else {pos_locT=pos_locT->next;}
	}

	if (pos_locT!=NULL){if(pos_locT->data.LS_PENDING){
		char h_source[ETH_ALEN];
		get_mac_address(arg->socket_fd, net_name, (unsigned char *) h_source) ;
		ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
		memcpy(tx_frame->buffer.header.type,t07,2);
		position->data.payload.itsnet_ls_req.sequencenumber=SN_g++;
		position->data.payload.itsnet_ls_req.source_position_vector=*LPV;
		memcpy(tx_frame->buffer.data, (char *) &position->data,sprint_hex_data(position->data.common_header.payload_lenght,2)+52+4+8+4);
		send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, 52 +sprint_hex_data(position->data.common_header.payload_lenght,2)+14+4+8+4);//==-1){}
		//	ev_timer_again (l_Beacon,&t_Beacon);
		pos_time->RTC++;free(tx_frame);
		if (pos_time->RTC>=itsGnLocationServiceMaxRetrans){
			//sup_timer(num,3);//xa se fai mais adiante
			sup_timer(num,2);
			//search_in_locT_m(position->data.payload.itsnet_ls_req.GN_ADDR.mac,locT_general);
			Element_lsp * pos_lsp=ls_buffer->init;
			while(pos_lsp!=NULL){ //aqui
				if(memcmp(pos_lsp->data.payload.itsnet_ls_req.GN_ADDR.mac.address,position->data.payload.itsnet_ls_req.GN_ADDR.mac.address,6)==0)
				{sup_elem_t_lsp(pos_lsp->data.payload.itsnet_ls_req.sequencenumber,3);
				}
				pos_lsp=pos_lsp->next;
			}
			Element_locT * pos_locT=locT_general->init;
			while(pos_locT!=NULL){
				if(memcmp(pos_locT->data.mac_id.address,position->data.payload.itsnet_ls_req.GN_ADDR.mac.address,6)==0)
				{
					int i=1;
					bool a=taken[i];int aa=1;
					while(a && aa==1){if(memcmp((void *)mac_list[i],pos_locT->data.mac_id.address,6)==0){aa=0;}else {a=taken[i];i++;}}
					sup_elem_locT(i,&pos_locT->data.mac_id,locT_general);
				}
				pos_locT=pos_locT->next;
			}
		}else pos_time->Period=itsGnLocationServiceRetransmitTimer;
	}else{
		if(pos_time!=NULL){sup_timer(pos_time->TimerId,2);}
		if (position!=NULL)sup_elem_t_lsp(num,3);}
	}	return(e);
}

int send_bcast_cbf_uc(int num,public_ev_arg_r *arg){

	int aa=0;
	Element_lsp * pos=lsp_cbf_uc->init;
	while(pos!=NULL && aa==0)
	{		if (pos->data.payload.itsnet_unicast.sequencenumber==num)aa=1;else pos=pos->next;	}
	itsnet_packet * pkt=&pos->data;
	sup_elem_t_lsp(num,6);
	int lon_int =sprint_hex_data(pkt->common_header.payload_lenght,2);
	char h_source[ETH_ALEN];
	get_mac_address(arg->socket_fd, net_name, (unsigned char *) h_source) ;
	ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
	memcpy(tx_frame->buffer.header.type,t07,2);
	int length_header=0;
	if(pkt->common_header.HT_HST.HT==2){length_header=48;}else{length_header=44;} //comprobar
	memcpy(tx_frame->buffer.data, (char *) pkt,lon_int+length_header+4+8+4);
	send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer,length_header +lon_int+14+4+8+4);//==-1){}
	free(pkt);free(tx_frame);
	return(0);
}

void SystemTickEvent(void)
{alarm(1);
signal(SIGALRM, SystemTickEvent);
tTimer *pTimer;i=0;
while(i<1000){gTimer_lsp[i++]=0;	gTimer_ls_rtx[i++]=0;gTimer_ls_buff[i++]=0; if(i<100){gTimer_pending[i++]=0;gTimer_uc[i++]=0;	gTimer_repetition_max[i++]=0;gTimer_repetition[i++]=0;}}
// Update the timers
pTimer = mpTimerList->init;
while(pTimer != NULL)
{ 	if(pTimer->Period != 0){pTimer->Period--;
if(pTimer->Period == 0){ 		// Set the corresponding bit when the timer reaches zero
	gTimer = gTimer | pTimer->TimerId;
}        }
// Move to the next timer in the list
pTimer = pTimer->pNext;
}
if (gTimer!=0){raise(SIGUSR2);}
tTimer *pTimer1;
// Update the timers
pTimer1 = mpTimerList_lsp->init;
int count=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_lsp[count] = pTimer1->TimerId;	count++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;int aa=0;
while(i<1000 && aa==0){if (gTimer_lsp[i++]!=0) aa=1;}
if (aa!=0){raise(SIGUSR1);
}
// Update the timers
pTimer1 = mpTimerList_ls_rtx->init;
int count1=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_ls_rtx[count1] = pTimer1->TimerId;	count1++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;int aa1=0;
while(i<1000 && aa1==0){if (gTimer_ls_rtx[i++]!=0) aa1=1;}
if (aa1!=0){raise(45);}

// Update the timers
pTimer1 = mpTimerList_ls_buff->init;
int count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_ls_buff[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;int aa2=0;
while(i<1000 && aa2==0){if (gTimer_ls_buff[i++]!=0) aa2=1;}
if (aa2!=0){raise(46);}

// Update the timers
pTimer1 = mpTimerList_uc->init;
count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_uc[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;int aa3=0;
while(i<100 && aa3==0){if (gTimer_uc[i++]!=0) aa3=1;}
if (aa3!=0){raise(47);}
// Update the timers
pTimer1 = mpTimerList_cbf_uc->init;
count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_cbf_uc[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;aa3=0;
while(i<100 && aa3==0){if (gTimer_cbf_uc[i++]!=0) aa3=1;}
if (aa3!=0){raise(47);}
// Update the timers


pTimer1 = mpTimerList_repetition_max->init;
count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_repetition_max[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;aa3=0;
while(i<100 && aa3==0){if (gTimer_repetition_max[i++]!=0) aa3=1;}
if (aa3!=0){raise(49);}

// Update the timers
pTimer1 = mpTimerList_repetition->init;
count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_repetition[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;aa3=0;
while(i<100 && aa3==0){if (gTimer_repetition[i++]!=0) aa3=1;}
if (aa3!=0){raise(50);}


// Update the timers
pTimer1 = mpTimerList_pending->init;
count2=0;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_pending[count2] = pTimer1->TimerId;	count2++;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
i=0;aa3=0;
while(i<100 && aa3==0){if (gTimer_pending[i++]!=0) aa3=1;}
if (aa3!=0){raise(51);}
}

void thr_h2(void *arg){
	alarm(1);
	signal(SIGALRM,SystemTickEvent);
	signal(SIGUSR2, CheckTimerEvent);
	signal(SIGUSR1, CheckTimerEvent_lsp);
	signal(45, CheckTimerEvent_ls_rtx);
	signal(46, CheckTimerEvent_ls_buff);
	signal(47, CheckTimerEvent_uc);
	signal(48, CheckTimerEvent_cbf_uc);
	signal(49, CheckTimerEvent_repetition_max);
	signal(50, CheckTimerEvent_repetition);
}

List_locT * startup1(configuration_t *cfg)
{
	if (itsGnLocalAddrConfMethod==0){
		PRF("AUTOADDRESS CONFIGURATION\n");
		memset(&GN_ADDR,0,8);
		if (cfg->manually==true) GN_ADDR.manually=1; else GN_ADDR.manually=0;
		GN_ADDR.itss_type=cfg->itss_type;
		if (cfg->scc>255){GN_ADDR.scc2=floor(cfg->scc/256);GN_ADDR.scc=cfg->scc-(256*GN_ADDR.scc2);
		}else {GN_ADDR.scc=cfg->scc;GN_ADDR.scc2=0;}
	}else{
		PRF("MANAGED ADDRESS CONFIGURATION, communication with lateral layer\n");
	}
	locT_general = init_locT();
	ls_buffer= init_lsp();
	mpTimerList= init_timer_list();
	mpTimerList_lsp= init_timer_list();
	mpTimerList_pending= init_timer_list();
	mpTimerList_ls_rtx= init_timer_list();
	mpTimerList_ls_buff=init_timer_list();
	mpTimerList_uc=init_timer_list();
	mpTimerList_cbf_uc=init_timer_list();
	mpTimerList_repetition=init_timer_list();
	mpTimerList_repetition_max=init_timer_list();
	return (locT_general);
}
int first=0;


itsnet_position_vector * LPV_update(EV_P_ ev_timer *w, int revents){
	char * h_source= (char *)w->data;
	gps_data_t * gpsdata_p=NULL;
	if (first==1) {free(gpsdata_p);gpsdatatnum--;}
	gps_data_t gpsdata;
	if (first==0){printf("entro no first==0\n");}
		gpsdata_p=(gps_data_t*)malloc(sizeof(gps_data_t));gpsdatatnum=gpsdatatnum+1;printf("gps_data_t %d \n",gpsdatatnum);
	gpsdata=*gpsdata_p;
	PRF ("ENTRO NO DO LPV_update\n");
	int timer;
	if (first==0){LPV_old=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	timer=50000;
	printf("LPV_old %d",LPV_oldnum);LPV_oldnum++;

	}else timer=5000;
	if (first==1) {memcpy((void *)LPV_old,(void *)LPV,24);}
	if (first==1){free(LPV);LPVnum--;}

	first=1;
	LPV=NULL;
	LPV=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));printf("LPV %d",LPVnum);LPVnum++;
	if( gps_open("localhost","2947",&gpsdata)!=0){
		return(LPV);
	}
	if (&gpsdata == NULL) {		PRF("Could not connect to gpsd!\n");	}
	(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);//gpsdata.dev);
	/* Put this in a loop with a call to a high resolution sleep () in it. */
	int i=0;PRF ("ENTRO NO DO LPV_update\n");
	for(i = 0; i < 100; i++){
		if (gps_waiting (&gpsdata, timer)) //wait 5 seconds
		{
			if (gps_read (&gpsdata) == -1) {PRF ("ENTRO NO -1\n");
			} else {
				PRF ("ENTRO NO gpsdata\n");
				/* Display data from the GPS receiver. */
				if (gpsdata.fix.mode>=2 && 	(gpsdata.fix.epx>=0 && gpsdata.fix.epx<366) && 	(gpsdata.fix.epy>=0 && gpsdata.fix.epy<366) && 	(gpsdata.fix.epv>=0 && gpsdata.fix.epv<366)){

					memcpy((void *)&LPV->node_id,(void *)&GN_ADDR,8);PRF ("ENTRO NO DO LPV_update\n");
					LPV->heading=gpsdata.fix.track *10; //necesitoo en 1/10 degrees e danmo en degrees.
					char str6[9] = {'\0'};	char str7[9] = {'\0'};	char str8[9] = {'\0'};	char str10[5] = {'\0'};
					if(gpsdata.fix.latitude<0){sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * 10000000));}
					if(gpsdata.fix.longitude<0){sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * -10000000));}else{sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * 10000000));}
					long double num0=fmod(((long double)gpsdata.fix.time*1000),4294967296);
					double num00= (double) num0;
					sprintf(str8, "%08X",(uint32_t)  num00);
					sprintf(str10, "%04X",(int)ceil(gpsdata.fix.speed *100));
					int num1=0;int num6=0;signed int num7=0;signed int num8=0;int num9=0;int num10=0;
				//	print_hex_data(str7,4);printf(" longitud\n");
					num6=strtol(str6,NULL,16);PRF ("ENTRO NO DO LPV_update\n");
					num7=strtol(str7,NULL,16);
					num10=strtol(str10,NULL,16);
					if(gpsdata.fix.latitude<0){LPV->latitude=0xffff-num6;}else{LPV->latitude=num6;}
					if(gpsdata.fix.longitude<0){LPV->longitude=0xffff-num7;}else{LPV->longitude=num7;}
					LPV->time_stamp=num00;// recibe Unix time in seconds with fractional part  e quere miliseconds
					int speed1=floor(num10/128); //división entera
					int speed2=num10%128;
					LPV->speed1=speed1;
					LPV->speed2=speed2; //recibe metros por segundo e quere 1/100 m/s
					print_hex_data(h_source,6);PRF( "MAC \n");
					memcpy(LPV->node_id.mac.address,h_source,6);
					i=100;				} } }else {if (first==1) memcpy(LPV,LPV_old,24);    }}
	gps_close (&gpsdata);PRF("saio do lpv_update\n");
	return(LPV);
}

itsnet_position_vector * LPV_ini(){ //non se está executando nunca
	itsnet_position_vector * LPV;
	LPV->longitude=0;	LPV->latitude=0;	LPV->time_stamp=0;	LPV->speed1=0;LPV->speed2=0;	LPV->heading=0;	memcpy((void *)&LPV->node_id,(void *)&GN_ADDR,8);
	return(LPV);}

void Beacon_send(EV_P_ ev_timer *w, int revents) {
	PRF("teño que enviar unha Beacon\n");
	char h_source[ETH_ALEN];
	public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
	get_mac_address(arg->socket_fd, net_name,(unsigned char *) h_source) ;
	ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
	memcpy(tx_frame->buffer.header.type,t07,2);
	version_nh * res=NULL;
	itsnet_packet * pkt = NULL;
	trafficclass_t *tc=NULL;
	tc=(trafficclass_t *)malloc(1);
	res= (version_nh *)malloc(1);
	flags_t * flags=NULL;
	flags= (flags_t *)malloc(1);
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));//printf("pkt %d",pktnum);pktnum++;
	pkt->basic_header.version_nh.HST=1;
	pkt->basic_header.version_nh.HT=itsGnProtocolVersion;
	char res_char[1];
	//res_char=(char *)res;PRF("beacon enviada\n");
	//memcpy(pkt->basic_header.version_nh,(void *)res,1);	PRF("beacon enviada\n");
	memset(pkt->basic_header.reserved,0,1);
	memset(pkt->basic_header.lt,itsGnMaxPacketLifeTime,1);
	memset(pkt->basic_header.rhl,1,1);
	pkt->common_header.HT_HST.HT=1;
	pkt->common_header.HT_HST.HST=0;
	memset(flags,0,1);
	flags->itsStation=itsGnStationType;
	memcpy(pkt->common_header.flags,(void *)flags,1);
	memset(pkt->common_header.payload_lenght,0,1);
	memset(pkt->common_header.reserved,0,1);
	memset(pkt->common_header.mhl,1,1);
	pkt->payload.itsnet_beacon.source_position_vector=*LPV;
	tc->channel_offload=0;
	tc->scf=0;
	tc->tc_id=itsGnDefaultTrafficClass;
	memset(&pkt->common_header.traffic_class,tc,1);
	char* num1[2];char* num2[2];// hai que manter estes asteriscos
	sprintf(num1,"%02X",(unsigned int)arg->port);	sprintf(num2,"%02X",(unsigned int)arg->forwarding_port);
	memcpy(pkt->payload.itsnet_beacon.payload.btp1,num1,2);	memcpy(pkt->payload.itsnet_beacon.payload.btp2,num2,2);
	memcpy(tx_frame->buffer.data, (char *) pkt,8+4+24);
	// 2) broadcast application level UDP message to network level
	//while(

	send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer,24+8+4+14);//==-1){}
	free(tc);free(flags);free(res);free(pkt);free(tx_frame);
	PRF("beacon enviada\n");

}
List_locT * init_locT ()
{
	List_locT * locTi=NULL;
	locTi = (List_locT *) malloc (sizeof (List_locT));//printf("locTi %d",loctinum);loctinum++;
	locTi->init = NULL;
	locTi->end = NULL;
	locTi ->len = 0;
	return(locTi);
}

List_timer * init_timer_list ()
{
	List_timer * list=NULL;
	list = (List_timer *) malloc (sizeof (List_timer));//printf("init_timer_list %d",init_timer_listnum);init_timer_listnum++;
	list->init= NULL;
	list->end=NULL;
	list->len=0;
	return(list);
}

/*add at list end  */
List_locT * add_end_locT ( List_locT * locT, itsnet_node data){
	Element_locT *new_element=NULL;
	new_element = (Element_locT *) malloc (sizeof (Element_locT));printf("locTelement %d\n",locTelementnum);locTelementnum++;

	new_element->data= data;
	new_element->next = NULL;
	new_element->before = NULL;
	if (locT->init==NULL) {
		//new_element->before=locT->init;
		locT->init=new_element; //new_element->next=locT->end;
	} else {
		new_element->before=locT->end;
		locT->end->next = new_element;	}
	locT->end = new_element;
	locT ->len++;
	int num2=0;int a=0;
	unsigned short num=0;
	while(a==0 && num2<16){
		num2=num2 +1;
		if (taken[num2]==false){num=dictionary[num2]; mac_list[num2]=&(new_element->data.mac_id); a=1;taken[num2]=true;}
	}
	AddTimer(num,itsGnLifetimeLocTE,1);
	locT_general=locT;
	PRF("fin add_en_loct\n");
	return locT;
}

/*modificate locT  */
List_locT * mod_t_locT (int val, List_locT * locT, itsnet_node *data,int num,itsnet_time_stamp tst){
	Element_locT *pos=locT->init;	int aa=1;
	while (pos!=NULL && aa<val) {aa++;
		pos=pos->next;
	}//PRF("%d\n",data->pdr);
	if (pos!=NULL){	if(data->tpdr==pos->data.tpdr)pos->data.pdr= pos->data.pdr++;else pos->data.pdr= 1/((data->tpdr)-pos->data.tpdr); }
	//else
	//{PRF("one\n");
	//PRF("%d\n",data->tpdr);	PRF("%d %d\n",data->tpdr-PDR_ini,PDR_ini);
	//PRF("%d \n",1/(data->tpdr-PDR_ini));
	//pos->data.pdr=1/((int)(data->tpdr)-(int)(PDR_ini));}
	if(((pos->data.tst.tv_sec < tst ) &&(tst- pos->data.tst.tv_sec<=65536/2))||((pos->data.tst.tv_sec > tst)&&(-tst + pos->data.tst.tv_sec >65536/2)))
	{	pos->data.pos_vector=data->pos_vector;}
	if (num!=0)pos->data.Sequence_number=num;
	pos->data.tst.tv_sec=tst;
	locT_general=locT;
	return locT;
}

int sup_timer (unsigned short TimerId, int num)
{
	tTimer * position=NULL;
	tTimer * to_erase=NULL;
	List_timer *list;
	if (num==0){list= mpTimerList_lsp;}else if(num==1){list=mpTimerList;}else if(num==2){list=mpTimerList_ls_rtx;}else if (num==3){list=mpTimerList_ls_buff;}else if (num==5){list=mpTimerList_pending;}else if (num==6){list=mpTimerList_cbf_uc;}else if (num==7){list=mpTimerList_repetition_max;}else if (num==9){list=mpTimerList_repetition;}
	position = FindTimer(TimerId,num);
	if (position==NULL){
		PRF("null en sup_timer\n");	}
	else{
		if(position->before==NULL){
			to_erase=list->init;
			list->init=list->init->pNext;
			if(list->len==1){list->end=NULL;  // PRF("eliminamos o unico timer \n");
			}else{list->init->before=NULL;}
			free(to_erase);list->len--;addtimernum--;printf("addtimernum: %d \n",addtimernum);
		}else if (position->pNext==NULL){
			to_erase=list->end;
			list->end->before->pNext=NULL;
			list->end=list->end->before;
			free(to_erase);list->len--;addtimernum--;printf("addtimernum: %d \n",addtimernum);
		}else{
			to_erase=position;
			position->before->pNext=position->pNext;
			position->pNext->before=position->before;
			free(to_erase);list->len--;addtimernum--;printf("addtimernum: %d \n",addtimernum);
		}
		if (num==0){ mpTimerList_lsp=list;}else if(num==1){mpTimerList=list;}else if (num==2){mpTimerList_ls_rtx=list;}else if (num==3){mpTimerList_ls_buff=list;}else if (num==5){mpTimerList_pending=list;}else if (num==6){mpTimerList_cbf_uc=list;}else if (num==7){mpTimerList_repetition_max=list;}else if (num==9){mpTimerList_repetition=list;}}
	return 0;
}
/* erase after a position */
int sup_elem_locT (int num,mac_addr *pos,List_locT *locT)
{
	Element_locT * position=locT->init;
	Element_locT *aux;
	Element_locT *to_erase;
	aux = locT->init;
	while (aux != NULL){aux = aux->next;}
	int in=1;	itsnet_node *data;
	data=(itsnet_node *)malloc(sizeof(itsnet_node));
	data->mac_id=*pos;
	int a=search_in_locT(data,locT);
	free(data);data=NULL;
	if (position==NULL) PRF("a posición inicial é  null\n");
	while (in<(a) && position!=NULL)
	{in++;position = position->next;//print_hex_data(position->data.mac_id.address,6);PRF(" \n");
	}
	if (position==NULL) PRF("a posición é null\n"); else{//print_hex_data(position->data.mac_id.address,6);PRF(" elimino este en loct\n");
		if(position->before==NULL){
			PRF("eliminamos o primeiro de loct\n");
			to_erase=locT->init;
			locT->init=locT->init->next;
			free(to_erase);locTelementnum--;printf("locTelementnum: %d \n",locTelementnum);
			if(locT->len==1){locT->end=NULL;// PRF("eliminamos o unico de loct\n");
			}else{locT->init->before=NULL;}locT->len--;
		}else if (position->next==NULL){ //   PRF("eliminamos o ultimo de loct \n");
		to_erase=locT->end;
		locT->end->before->next=NULL;
		locT->end=locT->end->before;
		free((void *)to_erase);locT->len--;locTelementnum--;printf("locTelementnum: %d \n",locTelementnum);
		}else{
			to_erase=position;
			PRF("eliminamos outro de loct\n");
			position->before->next=position->next;
			position->next->before=position->before;
			free(to_erase);locT->len--;locTelementnum--;printf("locTelementnum: %d \n",locTelementnum);
		}
		sup_timer(dictionary[num],1);
		locT_general=locT;
		memcpy(mac_list[num]->address,ZEROS,6);
		taken[num]=false;}
	return 0;
}

List_lsp * sup_elem_t_lsp (int num,int type)
{
	List_lsp *lsp;
	if (type==0)lsp=lsp_bc_g; else if (type==4)lsp=lsp_uc_g;else if (type==3)lsp=ls_buffer;else if (type==6)lsp=lsp_cbf_uc;else if (type==9)lsp=lsp_repetition;
	Element_lsp * position=lsp->init;
	Element_lsp *to_erase;
	if (position!=NULL){
		int buf_size=sprint_hex_data(position->data.common_header.payload_lenght,2);
		PRF("entrei no sup_elem_t_lsp \n");
		itsnet_node *data;
		int in=0;
		char SN[2];char HT[1];
		int head=0;
		memcpy(HT,(void *)&position->data.common_header.HT_HST,1);
		if (memcmp(HT,tsb0,1)==0 || memcmp(HT,tsb1,1)==0)head=28;else if (memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0||memcmp(HT,geoanycast0,1)==0||memcmp(HT,geoanycast1,1)==0||memcmp(HT,geoanycast2,1)==0)head=48;
		while (in==0 && position!=NULL)
		{
			memcpy(SN,(char*)(&position->data.payload),2);
			int num0=sprint_hex_data(SN,2);
			if(num0==num)in=1; else position = position->next;}
		if (position==NULL) {PRF("son null, no sup_elem_t_lsp\n");}else{
			if(position->before==NULL){
				to_erase=lsp->init;
				//PRF("eliminamos o primeiro de lsp\n");
				lsp->init=lsp->init->next;
				if(lsp->len==1){lsp->end=NULL;    //	PRF("eliminamos o unico \n");
				}else{lsp_bc_g->init->before=NULL;}
				free(to_erase);lsp->len--;lspnum--;printf("locTelementnum: %d \n",lspnum);
			}else if (position->next==NULL){    	//PRF("eliminamos o ultimo de lsp \n");
				to_erase=lsp->end;
				lsp->end->before->next=NULL;
				lsp->end=lsp->end->before;free(to_erase);lsp->len--;lspnum--;printf("lspnum: %d \n",lspnum);
			}else{
				//PRF("eliminamos outro de lsp\n");
				to_erase=position;
				position->before->next=position->next;
				position->next->before=position->before;
				free(to_erase);lsp->len--;lspnum--;printf("lspnum: %d \n",lspnum);
			}
			//free(SN);SN=NULL;
			sup_timer(num,type);

			lsp->size =lsp->size- 12-head-buf_size-4;}}
	if (type==0)lsp_bc_g=lsp; else if (type==4)lsp_uc_g=lsp;else if (type==3)ls_buffer=lsp;else if (type==6)lsp_cbf_uc=lsp;else if (type==9)lsp_repetition=lsp;
	return lsp;
}

/* view List */
void view_locT (){
	Element_locT *aux;	aux = locT_general->init;
	while (aux != NULL){//	print_hex_data(aux->data.mac_id.address,6);
		PRF(" lista loct\n");
		aux = aux->next;}
}
void view_lsp (){
	Element_lsp *aux;	aux = lsp_bc_g->init;
	char LEN[2];	int num0;
	while (aux != NULL){ //print_hex_data(&aux->data.payload,2);	PRF(" lista lsp\n");
		aux = aux->next;
	}
}
int any_neighbours (){
	Element_locT *aux;	aux = locT_general->init;
	int a=0;	int num0;
	while (aux != NULL && a==0){ //print_hex_data(&aux->data.payload,2);	PRF(" lista lsp\n");
		if(aux->data.IS_NEIGHBOUR)a=1;
		aux = aux->next;
	}	return(a);
}

int any_ls_proccess (){
	Element_locT *aux;	aux = locT_general->init;
	int a=0;	int num0;
	while (aux != NULL && a==0){ //print_hex_data(&aux->data.payload,2);	PRF(" lista lsp\n");
		if(aux->data.LS_PENDING)a=1;
		aux = aux->next;
	}
	return(a);
}


int search_in_locT (itsnet_node * data, List_locT * locT){
	Element_locT *aux;	aux = locT->init;
	int i=0;
	while (aux != NULL){
		if (memcmp(aux->data.mac_id.address,data->mac_id.address,6)==0){
			i=1;
			aux->data.pdr=PDR;
			if(((aux->data.pos_vector.time_stamp < data->pos_vector.time_stamp ) &&(data->pos_vector.time_stamp - aux->data.pos_vector.time_stamp <=4294967296/2))||((aux->data.pos_vector.time_stamp > data->pos_vector.time_stamp)&&(-data->pos_vector.time_stamp + aux->data.pos_vector.time_stamp >4294967296/2)))
			{				aux->data.pos_vector=data->pos_vector;
			aux->data.tst.tv_sec= data->tst.tv_sec;
			aux->data.IS_NEIGHBOUR=true;			}	}
		aux = aux->next;	}
	int a=0; int e=i;
	if(i==1){
		while ((i<17) &&(a==0)&& (e<17))
		{			if (taken[i]==true){
			//print_hex_data(mac_list[i]->address,6);PRF("\n");print_hex_data(data->mac_id.address,6);	PRF(" esta é a que busco\n");
			if(memcmp(data->mac_id.address,mac_list[i]->address,6)==0) {a=1;}else{ e++;i++;}}
		else i++;		}	}
	return(e);
}


int search_in_locT_m (mac_addr data, List_locT * locT){
	Element_locT *aux;	aux = locT->init;
	int e=1;i=0;int a=0;
	while ((i<17) &&(aux!=NULL) &&(a==0)&& (e<17))
	{
		if (taken[i]==true){
			if(memcmp(data.address,aux->data.mac_id.address,6)==0 && aux->data.IS_NEIGHBOUR ) {a=1;}else{ e++;i++;aux=aux->next;}}
		else {aux=aux->next;	i++;}	}
	if (a==0) return (0); else return(e);
}

int search_in_locT_m_wo_n (mac_addr data, List_locT * locT){
	Element_locT *aux;
	aux = locT->init;
	int e=1;i=0;int a=0;
	while ((i<17) &&(aux!=NULL) &&(a==0)&& (e<17))
	{
		if (taken[i]==true){
			if(memcmp(data.address,aux->data.mac_id.address,6)==0 ) {a=1;}else{ e++;i++;aux=aux->next;}}
		else {aux=aux->next;	i++;}	}
	if (a==0) return (0); else return(e);
}


int search_in_locT_m_pending (mac_addr data, List_locT * locT){
	Element_locT *aux;
	aux = locT->init;
	int e=1;
	int aa=0;i=0;
	while ((i<17) &&(aux!=NULL) &&(aa==0)&& (e<17))
	{
		if (taken[i]==true){
			//print_hex_data(mac_list[i]->address,6);PRF("\n");print_hex_data(data->mac_id.address,6);	PRF(" esta é a que busco\n");
			if(memcmp(data.address,aux->data.mac_id.address,6)==0 && aux->data.LS_PENDING) {aa=1;}else{ e++;i++;aux=aux->next;}}
		else {aux=aux->next;	i++;}	}
	if (aa==0) return (0); else return(e);
}
List_lsp * init_lsp ()
{List_lsp * lsp=NULL;
lsp = (List_lsp *) malloc (sizeof (List_lsp));
lsp ->init = NULL;lsp ->end = NULL;lsp ->len = 0;
return(lsp);}

/*add at list end  */
int add_end_lsp ( List_lsp * lsp, itsnet_packet data, int type){

	Element_lsp *new_element=NULL;
	new_element = (Element_lsp *) malloc (sizeof (Element_lsp));lspnum++;printf("lspnum: %d \n",lspnum);
	new_element->data= data;
	new_element->next = NULL;
	new_element->before = NULL;
	if (lsp->init==NULL) {
		lsp->init=new_element;  } else {
			new_element->before=lsp->end;
			lsp->end->next = new_element;}
	lsp->end = new_element;
	lsp ->len++;
	lsp->size=lsp->size+8+4+4+sprint_hex_data(data.common_header.payload_lenght,2);
	PRF("AQUI en engadir en lsp! %d \n",lsp->len);
	uint16_t sn;	LT_s *temp;
	char HT[1];	char HL[1];
	memcpy(HL,data.common_header.mhl,1);
	int lon_int=sprint_hex_data( HL, 1);
	memcpy(HT,(void *)&data.common_header.HT_HST,1);
	if((memcmp(HT,tsb0,1)==0) && (lon_int>1)){
		temp = (LT_s *) data.basic_header.lt;		sn = data.payload.itsnet_tsb.sequencenumber;
	}else if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0||memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0 ||memcmp(HT,geoanycast2,1)==0){
		sn = data.payload.itsnet_geobroadcast.sequencenumber;		temp = (LT_s *) data.basic_header.lt;}
	else if(memcmp(HT,geounicast,1)==0 ){
		sn = data.payload.itsnet_unicast.sequencenumber;		temp = (LT_s *) data.basic_header.lt;}
	else if(memcmp(HT,ls0,1)==0 ){
		sn = data.payload.itsnet_ls_req.sequencenumber;		temp = (LT_s *) data.basic_header.lt;
	}					else{sn=0;}
	if(type!=6){printf("type!=6\n");
	if (memcmp(HT,tsb0,1)==0 && lon_int<=1){printf("non paso!\n");}
		if(memcmp(HT,tsb1,1)==0 ||(memcmp(HT,tsb0,1)==0 && lon_int>1)||memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0||memcmp(HT,ls0,1)==0||memcmp(HT,geounicast,1)==0||memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0 ||memcmp(HT,geoanycast2,1)==0){
			printf("paso!\n");
			int str2=temp->multiple;	int str1=temp->base;int num0;
			if (str1==0){num0=50;}else if(str1==1){num0=100;}else if(str1==2){num0=1000;}else num0=10000;
			int lt=num0*str2 /100; //lt in seconds
			PRF("SEQUENCE NUMBER! %d\n",sn);
			if (lt>0) {printf("paso!\n");AddTimer(sn,lt,type);}
		}
	}
	if (type==0)lsp_bc_g=lsp; else if (type==4)lsp_uc_g=lsp; else if (type==3)ls_buffer=lsp;else if (type==6)lsp_cbf_uc=lsp;else if (type==9)lsp_repetition=lsp;
	return 0;
}


/* erase after a position */
List_lsp * sup_elem_lsp (int num, int type){
	int a=0;
	List_lsp * lsp;
	if (type==0)lsp=lsp_bc_g; else if (type==4)lsp=lsp_uc_g;else if (type==3)lsp=ls_buffer;else if (type==9)lsp=lsp_repetition;
	Element_lsp *pos=lsp->init;
	Element_lsp *to_erase=lsp->init;
	int buf_size=sprint_hex_data(pos->data.common_header.payload_lenght,2);
	PRF("eliminamos o primeiro de lsp \n");
	if (lsp->init!=NULL){
		lsp->init=lsp->init->next;
		if(lsp->len==1){lsp->end=NULL;  //  PRF("eliminamos o unico de lsp\n");
		}else{lsp->init->before=NULL;}
		free(to_erase);		lsp->len--;lspnum--;printf("lspnum: %d \n",lspnum);
		char HT[1];uint16_t sn;char HL[1];
		memcpy(HT,(void *)&pos->data.common_header.HT_HST,1);
		memcpy(HL,pos->data.common_header.mhl,1);
		int head=28;
		int lon_int=sprint_hex_data( HL, 1);
		if((memcmp(HT,tsb0,1)==0 && lon_int>1) ||memcmp(HT,tsb1,1)==0 || memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0||memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0 ||memcmp(HT,geoanycast2,1)==0){a=1;}
		if (a==1){ if (num==0xffff){
			if((memcmp(HT,tsb0,1)==0 && lon_int>1) ||memcmp(HT,tsb1,1)==0){
				sn = pos->data.payload.itsnet_tsb.sequencenumber;head=28;
			}else
				if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0||memcmp(HT,geoanycast0,1)==0 ||memcmp(HT,geoanycast1,1)==0 ||memcmp(HT,geoanycast2,1)==0){
					sn = pos->data.payload.itsnet_geobroadcast.sequencenumber;head=48;
				}
			sup_timer(sn,type);
		}else sup_timer(num,2);}
		lsp->size =lsp->size- 12-buf_size-head-4;
		PRF("saimos de suprimir elemento da lsp\n");
		if (type==0)lsp_bc_g=lsp; else if (type==4)lsp_uc_g=lsp;else if (type==3)ls_buffer=lsp;else if (type==9)lsp_repetition=lsp;}
	return (lsp);
}
/* view List */
void view_timers(){
	tTimer *aux;	aux = mpTimerList->init;
	while (aux != NULL){		PRF("timerid %d \n",aux->TimerId);aux = aux->pNext;	}
	PRF("pinto lista timers loct \n");
	aux = mpTimerList_lsp->init;
	while (aux != NULL){		PRF("timerid %d \n",aux->TimerId);aux = aux->pNext;	}
}

int duplicate_control(void * data,List_locT * locT){

	Element_locT *aux;
	aux = locT->init;
	int i=1;
	PRF("entro en duplicate control\n");
	itsnet_node_id * buffer=NULL;//
	buffer= (itsnet_node_id *)malloc(sizeof(itsnet_node_id));
	memcpy(buffer,data +8+4+4,8);
	char SN[2];
	memcpy(SN,data+8+4,2);
	int lon_int=sprint_hex_data( SN, 2);
	char TST_[4];
	memcpy(TST_,data+8+4+4+8,4);
	int lon_int2=sprint_hex_data( TST_, 4);
	while (aux != NULL && i==1){
		if (memcmp(aux->data.node_id.mac.address,buffer->mac.address,6)==0){
			if(((aux->data.tst.tv_sec < lon_int2 ) &&(lon_int2- aux->data.tst.tv_sec<=65536/2))||((aux->data.tst.tv_sec > lon_int2)&&(-lon_int2 + aux->data.tst.tv_sec >65536/2)))
			{	aux->data.tst.tv_sec=lon_int2;aux->data.Sequence_number=lon_int; }else
				if(aux->data.tst.tv_sec == lon_int2){
					if(((aux->data.Sequence_number < lon_int ) &&(lon_int- aux->data.Sequence_number <=65536/2))||((aux->data.Sequence_number > lon_int)&&(-lon_int + aux->data.Sequence_number >65536/2)))
					{	aux->data.tst.tv_sec=lon_int2;aux->data.Sequence_number=lon_int;	i=0	;}else{
						PRF("**************************DUPLICADO*************************** \n");	i=0;}
				}}
		else			aux = aux->next;
	}
	free(buffer);	PRF("saio de duplicate control\n");	return(i);
}

int duplicate_control2(void * data,List_locT * locT){

	Element_locT *aux;
	aux = locT->init;
	int i=1;
	PRF("entro en duplicate control2\n");
	itsnet_node_id * buffer=NULL;//
	buffer= (itsnet_node_id *)malloc(sizeof(itsnet_node_id));
	memcpy(buffer,data +8+4+4,8);
	char TST_[4];
	memcpy(TST_,data+8+4+4+8,4);
	int lon_int=sprint_hex_data( TST_, 4);
	while (aux != NULL && i==1){
		if (memcmp(aux->data.node_id.mac.address,buffer->mac.address,6)==0){
			if(((aux->data.tst.tv_sec < lon_int ) &&(lon_int- aux->data.tst.tv_sec<=65536/2))||((aux->data.tst.tv_sec > lon_int)&&(-lon_int + aux->data.tst.tv_sec >65536/2)))
			{	aux->data.tst.tv_sec=lon_int;i=0	;}else
				PRF("**************************DUPLICADO*************************** \n");i=0	;		  }
		else		aux = aux->next;
	}
	free(buffer);
	PRF("saio de duplicate control2\n");
	return(i);
}


