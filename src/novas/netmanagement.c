#include "netmanagement.h"
#include <sys/time.h>
const unsigned char ETH_BROAD[ETH_ALEN] ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
const unsigned char ZEROS[ETH_ALEN] ={ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
extern List_lsp * lsp_bc_g;
extern itsnet_node_id GN_ADDR;
extern int PDR;
itsnet_position_vector * LPV;
itsnet_position_vector *LPV_old;
//itsnet_position_vector *LPV_trans;
ev_timer t_Loct;
unsigned short dictionary[17]={0x00,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
mac_addr *mac_list[16];
List_locT *locT_general;
bool taken_rep[17]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
bool taken[17]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
List_timer *mpTimerList;
List_timer *mpTimerList_lsp;
List_timer *mpTimerList_rep;
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
extern const unsigned char single[1];
//Global variable for reference
static unsigned short gTimer;
int i=0;
int PDR_sum=0;
static int gTimer_lsp[1000];
#pragma inline SystemTickEvent,SystemTickEvent_lsp

#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
int PDR_update(int Pdr_n)
{
	int PDR_out;
	// time_t PDR_t = time(0);
	i++;
	PDR_sum= PDR_sum + Pdr_n;
	printf("PDR_sum %d %d\n",Pdr_n,PDR_sum);
	PDR_out=PDR_sum/i;
	//   struct tm *tlocal = localtime(&PDR_t);

	return(PDR_out);
}
tTimer * FindTimer(unsigned short TimerId,int num)
{
	List_timer * list= NULL;
	if (num==2){list= mpTimerList_lsp;
	}else{list=mpTimerList;}
	tTimer *pTimer=list->init;
	tTimer *pTimerC=NULL;
	PRF("FINDTIMER %d\n",TimerId);
	if(pTimer != NULL)
	{        // Move through the list until the timer id is found
		while((pTimer!= NULL) )
		{            // Timer ids - not match, Move onto the next timer
			if (pTimer->TimerId == TimerId){PRF("COINCIDENCIA!!!\n");
			pTimerC=pTimer;}
			pTimer = pTimer->pNext;        }    }
	PRF("saio de find timer\n");
	return (pTimerC);}

bool AddTimer(unsigned short TimerId, int num, int type)

{
	List_timer * list= NULL;
	if (type==2){list= mpTimerList_lsp;}else if(type==1){list=mpTimerList;}else {list=mpTimerList_rep;}
	tTimer *pTimer;
	tTimer *new_element = NULL;
	bool ReturnValue = false;
	// Look for the timer – if already exists
	pTimer = FindTimer(TimerId,type);
	// Check if the timer was found
	if((pTimer == NULL) )
	{PRF("esto si non atopa co find timer, crea un timer novo\n");
	new_element = (tTimer *) malloc(sizeof(tTimer));
	new_element->TimerId = TimerId;
	new_element->pNext = NULL;
	new_element->Period = num;
	if(list->init==NULL){new_element->before=list->init; new_element->pNext=list->end; list->init=new_element;  }
	else {
		new_element->before=list->end;
		list->end->pNext = new_element;}
	list->end = new_element;
	list ->len++;
	// Check if the list is empty
	}
	if(pTimer != NULL)
	{	// Set the timer interval
		pTimer->Period = num;
		ReturnValue = true;    }
	if (type==2){mpTimerList_lsp=list;}else if(type==1){mpTimerList=list;}else {mpTimerList_rep=list;}
	return ReturnValue;
}

void CheckTimerEvent(EV_P_ ev_timer *w, int revents)
{signal(SIGUSR2, CheckTimerEvent);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
nTimer = gTimer;gTimer = 0;
// Check for TimerId
mac_addr * pos;
//pos=(mac_addr *)malloc(6);
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
int aa=0;int i=0;
while (aa==0){  if(gTimer_lsp[i]==0){ aa=1;PRF("entro na primeira\n");} else {PRF("entro na segunda\n");sup_elem_t_lsp(gTimer_lsp[i]);i++;PRF("entro na segunda\n"); }
PRF("CheckTimerEvent_lsp SIGUSR1 fin\n");
}}

void SystemTickEvent(void)
{alarm(1);
signal(SIGALRM, SystemTickEvent);
tTimer *pTimer;
int i=0;
while(i<1000){	gTimer_lsp[i++]=0;	}
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
}}

void thr_h2(void *arg){
	alarm(1);
	signal(SIGALRM,SystemTickEvent);
	signal(SIGUSR2, CheckTimerEvent);
	signal(SIGUSR1, CheckTimerEvent_lsp);	}



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
	mpTimerList= init_timer_list ();
	mpTimerList_lsp= init_timer_list ();
	return (locT_general);}
int a;

gps_data_t * gpsdata_p=NULL;
itsnet_position_vector * LPV_update(EV_P_ ev_timer *w, int revents){
	char * h_source= (char *)w->data;

	//if (a==1)free(gpsdata_p);
	gps_data_t gpsdata;
	if (a==0)gpsdata_p=(gps_data_t*)malloc(sizeof(gps_data_t));
	gpsdata=*gpsdata_p;
	PRF ("ENTRO NO DO LPV_update\n");
	int timer;
	if (a==0){LPV_old=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	//LPV_trans=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	timer=50000;}else timer=5000;
	if (a==1) {memcpy(LPV_old,LPV,24);}
	if (a==0)	LPV=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	//if (a==1)free(LPV);
	a=1;
	//LPV=NULL;

	if( gps_open("localhost","2947",&gpsdata)!=0){
		return(LPV);
	}
	if (&gpsdata == NULL) {		PRF("Could not connect to gpsd!\n");	}
	(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);//gpsdata.dev);
	/* Put this in a loop with a call to a high resolution sleep () in it. */
	int i=0;
	for(i = 0; i < 100; i++){
		if (gps_waiting (&gpsdata, timer)) //wait 5 seconds
		{
			if (gps_read (&gpsdata) == -1) {
			} else {
				/* Display data from the GPS receiver. */
				if (gpsdata.fix.mode>=2 && 	(gpsdata.fix.epx>=0 && gpsdata.fix.epx<366) && 	(gpsdata.fix.epy>=0 && gpsdata.fix.epy<366) && 	(gpsdata.fix.epv>=0 && gpsdata.fix.epv<366)){
					LPV->node_id=GN_ADDR;
					LPV->heading=gpsdata.fix.track *10; //necesitoo en 1/10 degrees e danmo en degrees.
					char str6[9] = {'\0'};	char str7[9] = {'\0'};	char str8[9] = {'\0'};	char str10[5] = {'\0'};
					if(gpsdata.fix.latitude<0){sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * 10000000));}
					if(gpsdata.fix.longitude<0){sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * -10000000));}else{sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * 10000000));}
					long double num0=fmod(((long double)gpsdata.fix.time*1000),4294967296);
					double num00= (double) num0;
					sprintf(str8, "%08X",(uint32_t)  num00);
					sprintf(str10, "%04X",(int)ceil(gpsdata.fix.speed *100));
					int num1=0;int num6=0;signed int num7=0;signed int num8=0;int num9=0;int num10=0;
					print_hex_data(str7,4);printf(" longitud\n");
					num6=strtol(str6,NULL,16);
					num7=strtol(str7,NULL,16);
					num10=strtol(str10,NULL,16);
					if(gpsdata.fix.latitude<0){LPV->latitude=0xffff-num6;}else{LPV->latitude=num6;}
					if(gpsdata.fix.longitude<0){LPV->longitude=0xffff-num7;}else{LPV->longitude=num7;}
					LPV->time_stamp=num00;// recibe Unix time in seconds with fractional part  e quere miliseconds
					int speed1=floor(num10/128); //división entera
					int speed2=num10%128;
					LPV->speed1=speed1;
					LPV->speed2=speed2; //recibe metros por segundo e quere 1/100 m/s
					memcpy(LPV->node_id.mac,h_source,6);
					i=100;				} } }else {if (a==1)memcpy(LPV,LPV_old,24);    }}
	gps_close (&gpsdata);
	//print_hex_data(LPV,24);PRF(" é o lpv\n");
	return(LPV);
}

itsnet_position_vector * LPV_ini(){ //non se está executando nunca
	itsnet_position_vector * LPV;
	LPV->longitude=0;	LPV->latitude=0;	LPV->time_stamp=0;	LPV->speed1=0;LPV->speed2=0;	LPV->heading=0;	memcpy(&LPV->node_id,&GN_ADDR,8);
	return(LPV);}

void Beacon_send(EV_P_ ev_timer *w, int revents) {
	PRF("teño que enviar unha Beacon\n");
	char h_source[ETH_ALEN];
	public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
	get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
	char tipo[2]={0x07,0x07};
	memcpy(tx_frame->buffer.header.type,tipo,2);
	version_nh * res=NULL;
	itsnet_packet * pkt = NULL;
	trafficclass_t *tc=NULL;
	tc=(trafficclass_t *)malloc(1);
	res= (version_nh *)malloc(1);
	flags_t * flags=NULL;
	flags= (flags_t *)malloc(1);
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
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
	memcpy(pkt->common_header.flags,flags,1);
	memset(pkt->common_header.payload_lenght,0,1);
	memset(pkt->common_header.reserved,0,1);
	memset(pkt->common_header.mhl,1,1);
	pkt->payload.itsnet_beacon.source_position_vector=*LPV;
	tc->channel_offload=0;
	tc->scf=0;
	tc->tc_id=itsGnDefaultTrafficClass;
	memset(pkt->common_header.traffic_class,tc,1);
	char* num1[2];char* num2[2];
	sprintf(num1,"%02X",(unsigned int)arg->port);	sprintf(num2,"%02X",(unsigned int)arg->forwarding_port);
	memcpy(pkt->payload.itsnet_beacon.payload.btp1,num1,2);	memcpy(pkt->payload.itsnet_beacon.payload.btp2,num2,2);
	memcpy(tx_frame->buffer.data, (char *) pkt,8+4+24);
	// 2) broadcast application level UDP message to network level
	//while(
	free(tc);free(flags);free(res);free(pkt);
	send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer,24+8+4+14);//==-1){}
	free(tx_frame);
	PRF("beacon enviada\n");

}
List_locT * init_locT ()
{
	List_locT * locTi=NULL;
	locTi = (List_locT *) malloc (sizeof (List_locT));
	locTi->init = NULL;
	locTi->end = NULL;
	locTi ->len = 0;
	return(locTi);
}

List_timer * init_timer_list ()
{
	tTimer * list=NULL;
	list = (List_timer *) malloc (sizeof (List_timer));
	list->Period=0;
	list->TimerId=0;
	list->pNext= NULL;
	return(list);
}

/*add at list end  */
int add_end_locT ( List_locT * locT, itsnet_node data){
	Element_locT *new_element=NULL;

	new_element = (Element_locT *) malloc (sizeof (Element_locT));
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
	int num2=0;a=0;
	unsigned short num=0;
	while(a==0 && num2<16){
		num2=num2 +1;
		if (taken[num2]==false){num=dictionary[num2]; mac_list[num2]=&(new_element->data.mac_id); a=1;taken[num2]=true;}
	}
	AddTimer(num,itsGnLifetimeLocTE,1);
	locT_general=locT;
	PRF("fin add_en_loct\n");
	return 0;
}

int sup_timer (unsigned short TimerId, int num)
{
	tTimer * position=NULL;
	tTimer * to_erase=NULL;
	List_timer *list;
	if (num==2){list= mpTimerList_lsp;}else{list=mpTimerList;}
	position = FindTimer(TimerId,num);
	if (position==NULL){
		PRF("null en sup_timer\n");	}
	else{
		if(position->before==NULL){
			PRF("eliminamos o primeiro de timer\n");
			to_erase=list->init;
			list->init=list->init->pNext;
			if(list->len==1){list->end=NULL;   PRF("eliminamos o unico timer \n");
			}else{list->init->before=NULL;}
			free(to_erase);list->len--;
		}else if (position->pNext==NULL){
			PRF("eliminamos o ultimo de timer \n");
			to_erase=list->end;
			list->end->before->pNext=NULL;
			list->end=list->end->before;
		free(to_erase);list->len--;
		}else{
		PRF("eliminamos outro de timer \n");
		to_erase=position;
		position->before->pNext=position->pNext;
		position->pNext->before=position->before;
		free(to_erase);list->len--;
		}
		list->len--;
	if (num==2){ mpTimerList_lsp=list;}else{mpTimerList=list;}}
	return 0;
}
/* erase after a position */
int sup_elem_locT (int num,mac_addr *pos,List_locT *locT)
{
	Element_locT * position=locT->init;
	Element_locT *aux;
	Element_locT *to_erase;
	aux = locT->init;
	while (aux != NULL){//	print_hex_data(aux->data.mac_id.address,6);PRF(" lista loct en sup_elem\n");
		aux = aux->next;}
	int in=1;
	itsnet_node *data;
	data=(itsnet_node *)malloc(sizeof(itsnet_node));
	data->mac_id=*pos;
	int a=search_in_locT(data,locT);
	free(data);data=NULL;
	if (position==NULL) PRF("a posición inicial é  null\n");
	while (in<(a))
	{in++;position = position->next;//print_hex_data(position->data.mac_id.address,6);PRF(" \n");
	}
	if (position==NULL) PRF("a posición é null\n"); //print_hex_data(position->data.mac_id.address,6);PRF(" elimino este en loct\n");
	if(position->before==NULL){
		PRF("eliminamos o primeiro de loct\n");
		to_erase=locT->init;
		locT->init=locT->init->next;
		free(to_erase);locT->len--;
		if(locT->len==1){locT->end=NULL; PRF("eliminamos o unico de loct\n");
		}else{locT->init->before=NULL;}
	}else if (position->next==NULL){    PRF("eliminamos o ultimo de loct \n");
	to_erase=locT->end;
	locT->end->before->next=NULL;
	locT->end=locT->end->before;
	free((void *)to_erase);locT->len--;
	}else{
		to_erase=position;
		PRF("eliminamos outro de loct\n");
		position->before->next=position->next;
		position->next->before=position->before;
		free(to_erase);locT->len--;
	}
	sup_timer(dictionary[num],1);

	locT_general=locT;
	memcpy(mac_list[num],ZEROS,6);
	taken[num]=false;
	return 0;
}

int sup_elem_t_lsp (int num)
{
	Element_lsp * position=lsp_bc_g->init;
	Element_lsp *to_erase;
	PRF("entrei no sup_elem_t_lsp\n");
	int buf_size=sprint_hex_data(position->data.common_header.payload_lenght,2);
	PRF("entrei no sup_elem_t_lsp %d\n",buf_size);
	itsnet_node *data;
	int in=0;
	PRF("entrei no sup_elem_t_lsp\n");
	char SN[2];char HT[1];
	int head=0;
	memcpy(HT,&position->data.common_header.HT_HST,1);
	if (memcmp(HT,tsb0,1)==0 || memcmp(HT,tsb1,1)==0)head=28;else if (memcmp(HT,geobroad0,1)==0||memcmp(HT,geobroad1,1)==0||memcmp(HT,geobroad2,1)==0)head=48;
	while (in==0 && position!=NULL)
	{
		memcpy(SN,(char*)(&position->data.payload),2);
		int num0=sprint_hex_data(SN,2);
		if(num0==num)in=1; else position = position->next;}
	if (position==NULL) {printf("son null, no sup_elem_t_lsp\n");}else{
	if(position->before==NULL){
		to_erase=lsp_bc_g->init;
		//PRF("eliminamos o primeiro de lsp\n");
		lsp_bc_g->init=lsp_bc_g->init->next;
		if(lsp_bc_g->len==1){lsp_bc_g->end=NULL;    //	PRF("eliminamos o unico \n");
		}else{lsp_bc_g->init->before=NULL;}
		free(to_erase);lsp_bc_g->len--;
	}else if (position->next==NULL){    	//PRF("eliminamos o ultimo de lsp \n");
		to_erase=lsp_bc_g->end;
		lsp_bc_g->end->before->next=NULL;
		lsp_bc_g->end=lsp_bc_g->end->before;free(to_erase);lsp_bc_g->len--;
	}else{
		//PRF("eliminamos outro de lsp\n");
		to_erase=position;
		position->before->next=position->next;
		position->next->before=position->before;
		free(to_erase);lsp_bc_g->len--;
	}
	//free(SN);SN=NULL;
	sup_timer(num,2);

	lsp_bc_g->size =lsp_bc_g->size- 12-head-buf_size-4;}
	return 0;
}

/* view List */
void view_locT (){
	Element_locT *aux;
	aux = locT_general->init;
	while (aux != NULL){//	print_hex_data(aux->data.mac_id.address,6);
		PRF(" lista loct\n");
		aux = aux->next;}
}
void view_lsp (){
	Element_lsp *aux;
	aux = lsp_bc_g->init;
	char LEN[2];
	int num0;
	while (aux != NULL){ //print_hex_data(&aux->data.payload,2);	PRF(" lista lsp\n");
		aux = aux->next;
	}
}

int search_in_locT (itsnet_node * data, List_locT * locT){
	Element_locT *aux;
	aux = locT->init;
	int i=0;
	while (aux != NULL){
		if (memcmp(aux->data.mac_id.address,data->mac_id.address,6)==0){
			i=1;
			aux->data.pdr=PDR;
			if(((aux->data.pos_vector.time_stamp < data->pos_vector.time_stamp ) &&(data->pos_vector.time_stamp - aux->data.pos_vector.time_stamp <=4294967296/2))||((aux->data.pos_vector.time_stamp > data->pos_vector.time_stamp)&&(-data->pos_vector.time_stamp + aux->data.pos_vector.time_stamp >4294967296/2)))
			{				aux->data.pos_vector=data->pos_vector;
			aux->data.expires.tv_sec= itsGnLifetimeLocTE;
			aux->data.IS_NEIGHBOUR=true;			}	}
		aux = aux->next;	}
	int a=0; int e=i;
	if(i==1){
		while ((i<17) &&(a==0)&& (e<17))
		{
			if (taken[i]==true){
				//print_hex_data(mac_list[i]->address,6);PRF("\n");print_hex_data(data->mac_id.address,6);	PRF(" esta é a que busco\n");
				if(memcmp(data->mac_id.address,mac_list[i]->address,6)==0) {a=1;}else{ e++;i++;}}
			else i++;		}	}
	return(e);
}

List_lsp * init_lsp ()
{List_lsp * lsp=NULL;
lsp = (List_lsp *) malloc (sizeof (List_lsp));
lsp ->init = NULL;
lsp ->end = NULL;
lsp ->len = 0;
return(lsp);}

/*add at list end  */
int add_end_lsp ( List_lsp * lsp, itsnet_packet data){

	Element_lsp *new_element=NULL;
	new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
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
	memcpy(HL,&data.common_header.mhl,1);
	int lon_int=sprint_hex_data( HL, 1);
	memcpy(HT,&data.common_header.HT_HST,1);
	if((memcmp(HT,tsb0,1)==0) && (lon_int>1)){
		temp = (LT_s *) data.basic_header.lt;
		sn = data.payload.itsnet_tsb.sequencenumber;
	}else if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){
		sn = data.payload.itsnet_geobroadcast.sequencenumber;
		temp = (LT_s *) data.basic_header.lt;}else{sn=0;}
	if(memcmp(HT,tsb1,1)==0 ||(memcmp(HT,tsb0,1)==0 && lon_int>1)||memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){
		int str2=temp->multiple;	int str1=temp->base;int num0;
		if (str1==0){num0=50;}else if(str1==1){num0=100;}else if(str1==2){num0=1000;}else num0=10000;
		int lt=num0*str2 /100; //lt in seconds
		PRF("SEQUENCE NUMBER! %d\n",sn);
		if (lt>0) AddTimer(sn,lt,2);
	}
	lsp_bc_g=lsp;
	return 0;
}

int add_end_rep ( List_lsp * rep, itsnet_packet data){

	Element_lsp *new_element=NULL;
	new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
	new_element->data= data;
	new_element->next = NULL;
	if (rep->init==NULL) {
		new_element->before=rep->init; new_element->next=rep->end; rep->init=new_element;  } else {
			new_element->before=rep->end;
			rep->end->next = new_element;}
	rep->end = new_element;
	rep ->len++;
	int num2,a=0;
	unsigned short num=0;
	while(a==0){
		num2=(num2 +1) % 16;
		if (taken_rep[num2]==false){num=dictionary[num2]; taken_rep[num2]=true; a=1;}
	}
	AddTimer(num,itsGnMaxPacketLifeTime,3);
	return 0;
}

/* erase after a position */
List_lsp * sup_elem_lsp (int num){
	int a=0;
	Element_lsp *pos=lsp_bc_g->init;
	Element_lsp *to_erase=lsp_bc_g->init;
	int buf_size=sprint_hex_data(pos->data.common_header.payload_lenght,2);
	PRF("eliminamos o primeiro de lsp \n");
	lsp_bc_g->init=lsp_bc_g->init->next;
	if(lsp_bc_g->len==1){lsp_bc_g->end=NULL;    PRF("eliminamos o unico de lsp\n");
	}else{lsp_bc_g->init->before=NULL;}
	free(to_erase);
	lsp_bc_g->len--;

	char HT[1];uint16_t sn;char HL[1];
	memcpy(HT,&pos->data.common_header.HT_HST,1);
	memcpy(HL,pos->data.common_header.mhl,1);
	int head=28;
	int lon_int=sprint_hex_data( HL, 1);
	if((memcmp(HT,tsb0,1)==0 && lon_int>1) ||memcmp(HT,tsb1,1)==0 || memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){a=1;}
	if (a==1){ if (num==0xffff){
		if((memcmp(HT,tsb0,1)==0 && lon_int>1) ||memcmp(HT,tsb1,1)==0){
			sn = pos->data.payload.itsnet_tsb.sequencenumber;head=28;
		}else
			if(memcmp(HT,geobroad0,1)==0 ||memcmp(HT,geobroad1,1)==0 ||memcmp(HT,geobroad2,1)==0){
				sn = pos->data.payload.itsnet_geobroadcast.sequencenumber;head=48;
			}
		sup_timer(sn,2);
	}else sup_timer(num,2);}
	lsp_bc_g->size =lsp_bc_g->size- 12-buf_size-head-4;
	PRF("saimos de suprimir elemento da lsp\n");
	return (lsp_bc_g);
}
/* view List */
void view_timers(){
	tTimer *aux;
	aux = mpTimerList->init;
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
	memcpy(buffer,data +8,8); //cambiar
	char SN[2];
	memcpy(SN,data+36,2); //cambiar
	int lon_int=sprint_hex_data( SN, 2);
	while (aux != NULL){
		if (memcmp(aux->data.node_id.mac,buffer->mac,6)==0){
			if(((aux->data.Sequence_number < lon_int ) &&(lon_int- aux->data.Sequence_number <=65536/2))||((aux->data.Sequence_number > lon_int)&&(-lon_int + aux->data.Sequence_number >65536/2)))
			{	aux->data.Sequence_number=lon_int;	i=0	;
			PRF("**************************DUPLICADO*************************** \n");
			}		  }
		aux = aux->next;
	}
	free(buffer);
	PRF("saio de duplicate control\n");

	return(i);
}
