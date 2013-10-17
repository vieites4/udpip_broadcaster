//#include "main.c"
#include "netmanagement.h"
#include <sys/time.h>
const unsigned char ETH_BROAD[ETH_ALEN] ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
//#include <gpsd_config.h>
//#include <gpsdclient.h>
//#include "novas/netmanagement.h"
//List_locT * locT; //variable global
List_lsp * lsp;
extern itsnet_node_id GN_ADDR;
itsnet_position_vector * LPV;
ev_timer t_Loct;
unsigned short variables[16]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
Element_locT * mac_list[16];
List_locT *locT_general;
bool taken_rep[16],taken_lsp[16], taken[16]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
/*void *thr_h2(void * arg){
	struct ev_loop *l_Loct;

	l_Loct=EV_DEFAULT;
t_Loct.data=(void *)&arg;
struct parametros *p;
  p=(struct parametros *) arg;
int num=p->num;
//*addr[num]=l_Loct;
variables[num]=1;
		ev_timer_init(&t_Loct,sup_elem_locT,8.0,0.0); //itsGnLifetimeLocTE
		ev_timer_start(l_Loct,&t_Loct);
		// ev_loop (l_Loct, 0);
		printf("entro aqui\n");
	return NULL;
}*/
tTimer *mpTimerList;
tTimer *mpTimerList_lsp;
//Global variable for reference
static unsigned short gTimer,gTimer_lsp;
#pragma inline SystemTickEvent,SystemTickEvent_lsp
int cont=0;int cont1=0;

void Timer2Function(){}

static tTimer * FindTimer(unsigned short TimerId,int num)
{
	tTimer * list= NULL;
	if (num==itsGnMaxPacketLifeTime){list= mpTimerList_lsp;}else{list=mpTimerList;}
	tTimer *pTimer;
	// Move to the start of the list
	pTimer = list;
	// Check if list is empty
	if(pTimer != NULL)
	{        // Move through the list until the timer id is found
		while((pTimer->pNext != NULL) && (pTimer->TimerId != TimerId))
		{            // Timer ids - not match, Move onto the next timer
			if (pTimer->TimerId != TimerId)printf("COINCIDENCIA!!!\n");
			pTimer = pTimer->pNext;        }    }

	return (pTimer);
}

bool AddTimer(unsigned short TimerId, int num)

{
	tTimer * list= NULL;
	if (num==itsGnMaxPacketLifeTime){list= mpTimerList_lsp;}else{list=mpTimerList;}

	tTimer *pTimer;
	tTimer *pNewTimer = NULL;
	bool ReturnValue = false;
	// Look for the timer – if already exists
	pTimer = FindTimer(TimerId,num);
	// Check if the timer was found
	if((pTimer == NULL) || (pTimer->TimerId != TimerId))
	{		// Create a new timer
		pNewTimer = malloc(sizeof(tTimer));
		if(pNewTimer != NULL)
		{     	pNewTimer->TimerId = TimerId;
		pNewTimer->pNext = NULL;
		// Check if the list is empty
		if(pTimer == NULL)
		{   // Store the address of this timer as a first element in the list
			list = pNewTimer;
		}	else
		{        		// Add the new timer to the end of the list
			pTimer = pNewTimer;            }        }
		// Select the new timer
		pTimer = pNewTimer;    }
	if(pTimer != NULL)
	{	// Set the timer interval
		pTimer->Period = num;
		ReturnValue = true;    }
	return ReturnValue;
}

void CheckTimerEvent(EV_P_ ev_timer *w, int revents)
{	printf("5555555\n");
signal(SIGINT, CheckTimerEvent);
unsigned short nTimer;
// Read the global variable gTimer and reset the value
nTimer = gTimer;
gTimer = 0;
// Check for TimerId
struct parametros *p;
//p->
if( nTimer != 0)
{ printf("CHEGA A ENTRAR AQUI!!\n");if(nTimer & TIMER_1)       {	p->num=1;	p->locT=locT_general;    p->pos=mac_list[1];	sup_elem_locT(p);    }
if(nTimer & TIMER_2)        {  	p->num=2;	p->locT=locT_general;    p->pos=mac_list[2];	sup_elem_locT(p);    }
if(nTimer & TIMER_3)        {  	p->num=3;	p->locT=locT_general;    p->pos=mac_list[3];	sup_elem_locT(p);    }
if(nTimer & TIMER_4)        {  	p->num=4;	p->locT=locT_general;    p->pos=mac_list[4];	sup_elem_locT(p);    }
if(nTimer & TIMER_5)    	{  	p->num=5;	p->locT=locT_general;    p->pos=mac_list[5];	sup_elem_locT(p);    }
if(nTimer & TIMER_6)    	{  	p->num=6;	p->locT=locT_general;    p->pos=mac_list[6];	sup_elem_locT(p);    }
if(nTimer & TIMER_7)    	{  	p->num=7;	p->locT=locT_general;    p->pos=mac_list[7];	sup_elem_locT(p);    }
if(nTimer & TIMER_8)    	{  	p->num=8;	p->locT=locT_general;    p->pos=mac_list[8];	sup_elem_locT(p);    }
if(nTimer & TIMER_9)    	{  	p->num=9;	p->locT=locT_general;    p->pos=mac_list[9];	sup_elem_locT(p);    }
if(nTimer & TIMER_10)    	{  	p->num=10;	p->locT=locT_general;    p->pos=mac_list[10];	sup_elem_locT(p);    }
if(nTimer & TIMER_11)       {   p->num=11;	p->locT=locT_general;    p->pos=mac_list[11];	sup_elem_locT(p);    }
if(nTimer & TIMER_12)       {   p->num=12;	p->locT=locT_general;    p->pos=mac_list[12];	sup_elem_locT(p);    }
if(nTimer & TIMER_13)    	{   p->num=13;	p->locT=locT_general;    p->pos=mac_list[13];	sup_elem_locT(p);    }
if(nTimer & TIMER_14)    	{ 	p->num=14;	p->locT=locT_general;    p->pos=mac_list[14];	sup_elem_locT(p);    }
if(nTimer & TIMER_15)  	    {   p->num=15;	p->locT=locT_general;    p->pos=mac_list[15];	sup_elem_locT(p);    }
if(nTimer & TIMER_16)    	{   p->num=16;	p->locT=locT_general;    p->pos=mac_list[16];	sup_elem_locT(p);    }
}
}
void CheckTimerEvent_lsp()
{	printf("6666666\n");
signal(SIGUSR1, CheckTimerEvent_lsp);
unsigned short nTimer;

// Read the global variable gTimer and reset the value
nTimer = gTimer_lsp;
gTimer_lsp = 0;
// Check for TimerId
if( nTimer != 0)
{   if(nTimer & TIMER_1)        {     	Timer2Function();       }
if(nTimer & TIMER_2)        {     	Timer2Function();        }
if(nTimer & TIMER_3)        {       Timer2Function();        }
if(nTimer & TIMER_4)        {        	Timer2Function();        }
if(nTimer & TIMER_5)    	{      	Timer2Function();        }
if(nTimer & TIMER_6)    	{     	Timer2Function();        }
if(nTimer & TIMER_7)    	{        	Timer2Function();        }
if(nTimer & TIMER_8)    	{        	Timer2Function();        }
if(nTimer & TIMER_9)    	{      	Timer2Function();        }
if(nTimer & TIMER_10)    	{     	Timer2Function();        }
if(nTimer & TIMER_11)       {        	Timer2Function();        }
if(nTimer & TIMER_12)       {        	Timer2Function();        }
if(nTimer & TIMER_13)    	{      	Timer2Function();        }
if(nTimer & TIMER_14)    	{     	Timer2Function();        }
if(nTimer & TIMER_15)  	    {        	Timer2Function();        }
if(nTimer & TIMER_16)    	{        	Timer2Function();        }
}
}
void SystemTickEvent(void) //facer un fio que repita todo o tempo (bucle while(1)) esta temporizacion, cada segundo, poñer un sleep(1) xusto antes desta funcion
{alarm(1);
signal(SIGALRM, SystemTickEvent);
tTimer *pTimer;
// Update the timers
pTimer = mpTimerList;
while(pTimer != NULL)
{ 	if(pTimer->Period != 0){  	pTimer->Period--;printf("%d timer %d\n",pTimer->TimerId,pTimer->Period);
if(pTimer->Period == 0){ 		// Set the corresponding bit when the timer reaches zero
	gTimer = gTimer | pTimer->TimerId;
}        }
// Move to the next timer in the list
pTimer = pTimer->pNext;
}
//	printf("ENTRA O SIGNAL\n");
cont++;
//if (cont==5){cont=0;raise(SIGINT);}//eliminaríase esto, que está como un exemplo
if (gTimer!=0){printf("8888888888888888888888888888888888\n");raise(SIGINT);
}
tTimer *pTimer1;
// Update the timers
pTimer1 = mpTimerList_lsp;
while(pTimer1 != NULL)
{ 	if(pTimer1->Period != 0)
{  	pTimer1->Period--;
if(pTimer1->Period == 0)
{ 		// Set the corresponding bit when the timer reaches zero
	gTimer_lsp = gTimer_lsp | pTimer1->TimerId;
}        }
// Move to the next timer in the list
pTimer1 = pTimer1->pNext;
}
//	printf("ENTRA O SIGNAL\n");
cont1++;
if (cont1==5){cont1=0;raise(SIGUSR1);}//eliminaríase esto, que está como un exemplo
if (gTimer_lsp!=0){//raise(SIGINT);
}}



void handler_tempo(int sig){
	//printf("ENTRA O SIGNAL\n");

}
void thr_h2(void *arg){
	alarm(1);
	signal(SIGALRM,SystemTickEvent);
	while(1){
		//	SystemTickEvent();
		signal(SIGINT, CheckTimerEvent);
		signal(SIGUSR1, CheckTimerEvent_lsp);
	}
}

List_locT * startup1()
{
	if (itsGnLocalAddrConfMethod==0){
		printf("AUTOADDRESS CONFIGURATION\n");
		//GN_ADDR = (itsnet_node_id)malloc(8);
		GN_ADDR.id[0]=0x14;//itsGnLocalGnAddr(0);
		GN_ADDR.id[1]=0x00;
		GN_ADDR.id[2]=0x00;
		GN_ADDR.id[3]=0x00;
		GN_ADDR.id[4]=0x00;
		GN_ADDR.id[5]=0x00;
		GN_ADDR.id[6]=0x00;
		GN_ADDR.id[7]=0x00;
		//itsGnLocalGnAddr
	}else{
		printf("MANAGED ADDRESS CONFIGURATION, communication with lateral layer\n");
	}
	//List_locT *
	locT_general = init_locT(); //probablemente teña que facer esto aquí para tódalas listas/buffers.
	//LPV= LPV_update(l_LPV,&t_LPV, revents );
	struct parametros * argumento;
	/**argumento = (struct parametros *) malloc (sizeof (struct parametros));
	argumento->num=num;
	argumento->locT=locT;
	 argumento->pos=locT->end;**/ //(void *)argumento
	mpTimerList= init_locT_timer ();
	mpTimerList_lsp= init_locT_timer ();
	//ev_timer_again(*l_LPV,t_LPV);
	//ev_run(l_LPV,0);
	return (locT_general);
}
int a;

itsnet_position_vector * LPV_update(EV_P_ ev_timer *w, int revents){
	char * h_source= (char *)w->data;
	gps_data_t * gpsdata_p=NULL;
	if (a==1)free(gpsdata_p);
	gps_data_t gpsdata;
	gpsdata_p=(gps_data_t*)malloc(sizeof(gps_data_t));
	gpsdata=*gpsdata_p;
	//itsnet_position_vector * LPV;
	printf ("ENTRO NO DO LPV\n");
	if (a==1)free(LPV);
	a=1;
	LPV=NULL;
	LPV=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	if( gps_open("localhost","2947",&gpsdata)!=0){
		printf("sain despois do gps_open\n");
		return(LPV);
	}
	if (&gpsdata == NULL) {		printf("Could not connect to gpsd!\n");	}
	(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);//gpsdata.dev);
	/* Put this in a loop with a call to a high resolution sleep () in it. */
	int i;
	for(i = 0; i < 100; i++){
		if (gps_waiting (&gpsdata, 50000000))
		{
			if (gps_read (&gpsdata) == -1) {
			} else {
				/* Display data from the GPS receiver. */
				if (gpsdata.fix.mode>=2 && 	(gpsdata.fix.epx>=0 && gpsdata.fix.epx<366) && 	(gpsdata.fix.epy>=0 && gpsdata.fix.epy<366) && 	(gpsdata.fix.epv>=0 && gpsdata.fix.epv<366)){
					LPV->node_id=GN_ADDR;
					LPV->heading=gpsdata.fix.track *10; //necesitoo en 1/10 degrees e danmo en degrees.
					char str1[2] = {'\0'};	char str2[2] = {'\0'};	char str3[2] = {'\0'};	char str4[2] = {'\0'};	char str5[2] = {'\0'};	char str6[9] = {'\0'};	char str7[9] = {'\0'};
					char str8[9] = {'\0'};	char str9[5] = {'\0'};	char str10[5] = {'\0'};
					if(gpsdata.fix.ept==0){memcpy(str1,"0",1);}else {sprintf(str1, "%01X",(int) ceil(log2(1000*gpsdata.fix.ept)));}//ceil(log2(1000*gpsdata.fix.ept)));
					if(gpsdata.fix.epx==0 && gpsdata.fix.epx==0){memcpy(str2,"0",1);}else {sprintf(str2, "%01X",(int) ceil(log2(sqrt(pow(gpsdata.fix.epx,2)+(pow(gpsdata.fix.epy,2)))/4)));}
					if(gpsdata.fix.eps==0){memcpy(str3,"0",1);}else {sprintf(str3, "%01X",(int) ceil(log2(100*gpsdata.fix.eps)));}
					if(gpsdata.fix.ept==0){memcpy(str4,"0",1);}else {sprintf(str4, "%01X",(int) ceil(log2((1/0.005493247)*gpsdata.fix.ept)));}
					if(gpsdata.fix.epv==0){memcpy(str5,"0",1);}else {sprintf(str5, "%01X",(int) ceil(log2(gpsdata.fix.epv)));}
					if(gpsdata.fix.latitude<0){sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * 10000000));}
					if(gpsdata.fix.longitude<0){sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.longitude * 10000000));}
					long double num0=fmod(((long double)gpsdata.fix.time*1000),4294967296);
					double num00= (double) num0;
					sprintf(str8, "%08X",(uint32_t)  num00);
					if(gpsdata.fix.altitude<0){sprintf(str9, "%04X",(signed int)floor(gpsdata.fix.altitude*-1));}else {sprintf(str9, "%04X",(signed int)floor(gpsdata.fix.altitude));}
					sprintf(str10, "%04X",(int)ceil(gpsdata.fix.speed *100));
					int num1=0;int num2=0;int num3=0;int num4=0;int num5=0;int num6=0;signed int num7=0;signed int num8=0;int num9=0;int num10=0;
					num1=strtol(str1,NULL,16);
					num2=strtol(str2,NULL,16);
					num3=strtol(str3,NULL,16);
					num4=strtol(str4,NULL,16);
					num5=strtol(str5,NULL,16);
					num6=strtol(str6,NULL,16);
					num7=strtol(str7,NULL,16);
					num9=strtol(str9,NULL,16);
					num10=strtol(str10,NULL,16);
					if(gpsdata.fix.latitude<0){LPV->latitude=0xffff-num6;}else{LPV->latitude=num6;}
					if(gpsdata.fix.longitude<0){LPV->longitude=0xffff-num7;}else{LPV->longitude=num7;}
					if(gpsdata.fix.altitude<0){LPV->altitude=0xffff-num9;}else{LPV->altitude=num9;}
					LPV->time_stamp=num00;// recibe Unix time in seconds with fractional part  e quere miliseconds
					LPV->speed=num10; //recibe metros por segundo e quere 1/100 m/s
					LPV->accuracy.alt_ac=num5;
					LPV->accuracy.pos_ac=num2;
					LPV->accuracy.speed_ac=num3;
					LPV->accuracy.head_ac=num4;
					LPV->accuracy.time_ac=num1;
					memcpy(LPV->node_id.mac,h_source,6);
					i=100;
				} } }    }
	gps_close (&gpsdata);
	return(LPV);
}

itsnet_position_vector * LPV_ini(){
	itsnet_position_vector * LPV;
	LPV->altitude=0;
	LPV->longitude=0;
	LPV->latitude=0;
	LPV->time_stamp=0;
	LPV->speed=0;
	LPV->heading=0;
	LPV->node_id=GN_ADDR;
	LPV->accuracy.alt_ac=0;
	LPV->accuracy.pos_ac=0;
	LPV->accuracy.speed_ac=0;
	LPV->accuracy.head_ac=0;
	LPV->accuracy.time_ac=0;
	return(LPV);}

mac_addr autoaddrconf(){

	mac_addr addr;
	return(addr);
}

bool mngdaddrconf(int option) //1: initial, 2:update,3:duplicate detection
{
	bool correct=false;
	switch(option)
	{
	case 1: //initial

		printf("envío unha primitiva GN-MGMT.request\n");		break;
	case 2://update
		printf("envío primitiva GN-MGMT.request");		break;
	default://duplicados
		printf("fago a comparación SO e SE para detectar duplicados\n");//devolver true ou false
		//sería a opcion 3
		break;
	}
	return(correct);

}

void Beacon_send(EV_P_ ev_timer *w, int revents) {//public_ev_arg_r *arg){
	printf("teño que enviar unha Beacon\n");
	char h_source[ETH_ALEN];
	public_ev_arg_r * arg=(public_ev_arg_r *)w->data;
	get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
	//		tx_frame->buffer.frame_len = sizeof(arg->data) +42;
	char tipo[2]={0x07,0x07};
	memcpy(tx_frame->buffer.header.type,tipo,2);
	const unsigned char beaco[1]={0x01};
	version_nh * res=NULL;
	itsnet_packet * pkt = NULL;
	trafficclass_t *tc=NULL;
	tc=(trafficclass_t *)malloc(1);
	res= (version_nh *)malloc(1);
	flags_t * flags=NULL;
	flags= (flags_t *)malloc(1);
	pkt=(itsnet_packet *)malloc(sizeof(itsnet_packet));
	res->version=itsGnProtocolVersion;
	res->nh=0;
	memcpy(pkt->common_header.version_nh,res,1);
	pkt->common_header.HT_HST.HT=1;
	pkt->common_header.HT_HST.HST=0;
	memset(flags,0,1);
	flags->itsStation=itsGnStationType;
	memcpy(pkt->common_header.flags,flags,1);
	memset(pkt->common_header.payload_lenght,0,1);
	memset(pkt->common_header.hop_limit,1,1);
	pkt->common_header.forwarder_position_vector=*LPV;
	tc->reserved=0;
	tc->relevance=itsGnTrafficClassRelevance;
	tc->reliability=itsGnTrafficClassReliability;
	tc->latency=itsGnTrafficClassLatency;
	memset(pkt->common_header.traffic_class,tc,1);
	char *num1[3];char *num2[3];
	sprintf(num1,"%04X",(unsigned int)arg->port);	sprintf(num2,"%04X",(unsigned int)arg->forwarding_port);
	memcpy(pkt->payload.itsnet_beacon.payload.btp1,num1,4);	memcpy(pkt->payload.itsnet_beacon.payload.btp2,num2,4);
	memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );
	// 2) broadcast application level UDP message to network level
	int fwd_bytes = send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, sizeof(itsnet_common_header)+sizeof(itsnet_btp_wo_payload_t)+14);
	printf("beacon enviada\n");

}
List_locT * init_locT ()
{
	List_locT * locTi=NULL;
	locTi = (List_locT *) malloc (sizeof (List_locT));
	locTi->init = NULL;
	locTi->end = NULL;
	locTi ->len = 0;
	printf("AQUI en init ! %d \n",locTi->len);

	return(locTi);
}

tTimer * init_locT_timer ()
{
	tTimer * list=NULL;
	list = (tTimer *) malloc (sizeof (tTimer));
	list->Period=0;
	list->TimerId=0;
	list->pNext= NULL;
	return(list);
}

/*add at list end  */
int add_end_locT ( List_locT * locT, itsnet_node data){
	Element_locT *new_element=NULL;
	new_element = (Element_locT *) malloc (sizeof (Element_locT));
	if (new_element==NULL) printf( "No hay memoria disponible!\n");
	new_element->data= data;
	new_element->next = NULL;
	if (locT->init==NULL) {
		new_element->before=locT->init; new_element->next=locT->end; locT->init=new_element;
	} else {
		new_element->before=locT->end;
		locT->end->next = new_element;}

	locT->end = new_element;
	locT ->len++;
	int num2=0;a=0;
	unsigned short num=0;
	while(a==0 && num2<16){
		num2=num2 +1;
		if (taken[num2]==false){num=variables[num2]; taken[num2]=true;mac_list[num2]=locT->end; a=1;}
printf("%d é o num\n",num);
	}
	AddTimer(num,itsGnLifetimeLocTE);
	return 0;
}

/* erase after a position */
int sup_elem_locT (struct parametros *p)//(EV_P_ ev_timer *w, int revents)// void * arg){//EV_P_ ev_timer *w,
{
	// if (locT->len <= 1 || pos < 1 || pos >= locT ->len)
	//   return -1;
	int i;
	//struct mac_addr *pos=p->pos;
	Element_locT *position=p->pos;
	List_locT * locT=p->locT;
	//Element_locT *actual,*position;
	//	actual = locT->init;
	/**while (actual != NULL){
			if (memcmp(actual->data.mac_id.address,pos.address,6)==0){
				position=actual;
				}
			actual = actual->next;
		}**/

	printf("AQUI en suprimir ! %d \n",locT->len);
	if(position->before==NULL){
		printf("eliminamos o primeiro \n");
		locT->init=locT->init->next;
		if(locT->len==1){locT->end=NULL;    	printf("eliminamos o unico \n");}else{locT->init->before=NULL;}
	}else if (position->next==NULL){    	printf("eliminamos o ultimo \n");
	locT->end->before->next=NULL;
	locT->end=locT->end->before;
	}else{
		printf("eliminamos outro \n");
		position->before->next=position->next;
		position->next->before=position->before;
	}
	locT->len--;
	taken[p->num]=0;
	mac_list[p->num]=NULL;
	return 0;
}

/* view List */
void view_locT (List_locT * locT){
	Element_locT *actual;
	actual = locT->init;
	while (actual != NULL){	print_hex_data(actual->data.mac_id.address,6);printf("\n");actual = actual->next;}
}

int search_in_locT (itsnet_node * data, List_locT * locT){
	Element_locT *actual;
	actual = locT->init;
	int i=0;
	while (actual != NULL){
		if (memcmp(actual->data.mac_id.address,data->mac_id.address,6)==0){
			i=1;
			if(((actual->data.pos_vector.time_stamp < data->pos_vector.time_stamp ) &&(data->pos_vector.time_stamp - actual->data.pos_vector.time_stamp <=4294967296/2))||((actual->data.pos_vector.time_stamp > data->pos_vector.time_stamp)&&(-data->pos_vector.time_stamp + actual->data.pos_vector.time_stamp >4294967296/2)))
			{
				actual->data.pos_vector=data->pos_vector;
				actual->data.expires.tv_sec= itsGnLifetimeLocTE;
				actual->data.IS_NEIGHBOUR=true;
			}	}
		actual = actual->next;
	}
	return(i);
}

bool exist_neighbours(List_locT * locT){

	Element_locT *actual;
	actual = locT->init;
	while (actual != NULL){
		if (actual->data.IS_NEIGHBOUR){return(true);}else{ actual = actual->next;}
	}
	return(false);
}

List_lsp * init_lsp ()
{
	List_lsp * lsp=NULL;
	lsp = (List_lsp *) malloc (sizeof (List_lsp));
	lsp ->init = NULL;
	lsp ->end = NULL;
	lsp ->len = 0;
	return(lsp);
}

/*add at list end  */
int add_end_lsp ( List_lsp * lsp, itsnet_packet data){

	Element_lsp *new_element=NULL;
	new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
	if (new_element==NULL) printf( "No hay memoria disponible!\n");
	new_element->data= data;
	new_element->next = NULL;
	if (lsp->init==NULL) {
		new_element->before=lsp->init; new_element->next=lsp->end; lsp->init=new_element;  } else {
			new_element->before=lsp->end;
			lsp->end->next = new_element;}
	lsp->end = new_element;
	lsp ->len++;
	printf("AQUI en engadir ! %d \n",lsp->len);
	int num2,a=0;
	unsigned short num=0;
	while(a==0){
		num2=(num2 +1) % 16;
		if (taken_lsp[num2]==false){num=variables[num2]; taken_lsp[num2]=true; a=1;}
	}
	AddTimer(num,itsGnMaxPacketLifeTime);
	return 0;
}

int add_end_rep ( List_lsp * rep, itsnet_packet data){

	Element_lsp *new_element=NULL;
	new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
	new_element->data= data;
	new_element->next = NULL;
	if (rep->init==NULL) {
		new_element->before=rep->init; new_element->next=rep->end; rep->init=new_element;  } else {
			//printf( "Non é primeiro elemento\n");
			new_element->before=rep->end;
			rep->end->next = new_element;}
	rep->end = new_element;
	rep ->len++;
	printf("AQUI en engadir ! %d \n",rep->len);
	int num2,a=0;
	unsigned short num=0;
	while(a==0){
		num2=(num2 +1) % 16;
		if (taken_rep[num2]==false){num=variables[num2]; taken_rep[num2]=true; a=1;}
	}
	AddTimer(num,itsGnMaxPacketLifeTime);
	return 0;
}

/* erase after a position */
int sup_elem_lsp (struct parametros_lsp *p){
	// if (locT->len <= 1 || pos < 1 || pos >= locT ->len)
	//   return -1;
	Element_lsp *pos=p->pos;
	List_lsp * lsp=p->lsp;
	printf("AQUI en suprimir ! %d \n",lsp->len);
	if(pos->before==NULL){
		printf("eliminamos o primeiro \n");
		lsp->init=lsp->init->next;
		if(lsp->len==1){lsp->end=NULL;    	printf("eliminamos o unico \n");}else{lsp->init->before=NULL;}
	}else if (pos->next==NULL){    	printf("eliminamos o ultimo \n");
	lsp->end->before->next=NULL;
	lsp->end=lsp->end->before;
	}else{
		printf("eliminamos outro \n");
		pos->before->next=pos->next;
		pos->next->before=pos->before;
	}
	lsp->len--;
	taken_lsp[p->num]=0;
	printf("saimos de suprimir elemento da loct1\n");
	return 0;
}

/* view List */
void view_lsp(){
	Element_lsp *actual;
	actual = lsp->init;
	while (actual != NULL){		print_hex_data(actual->data.payload,12);printf("\n");actual = actual->next;	}
}

int duplicate_control(void * data,List_locT * locT){

	Element_locT *actual;
	actual = locT->init;
	int i=1;
	itsnet_node_id * dato=NULL;//
	dato= (itsnet_node_id *)malloc(sizeof(itsnet_node_id));
	memcpy(dato,data +8,8);
	char SN[2];
	memcpy(SN,data+36,2);
	int lon_int=sprint_hex_data( SN, 2);
	while (actual != NULL){
		//printf ("%p - %s\n", actual, actual->data);
		if (actual->data.node_id.id==dato->id){
			i=0;
			if(((actual->data.Sequence_number < lon_int ) &&(lon_int- actual->data.Sequence_number <=65536/2))||((actual->data.Sequence_number > lon_int)&&(-lon_int + actual->data.Sequence_number >65536/2)))
			{	actual->data.Sequence_number=lon_int;		  }		  }
		actual = actual->next;
	}
	return(i);}
