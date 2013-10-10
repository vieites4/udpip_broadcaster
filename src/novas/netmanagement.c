//#include "main.c"

//2947 porto para gps, deixo correndo o sistema e
#include "netmanagement.h"
#include <sys/time.h>
const unsigned char ETH_BROAD[ETH_ALEN] ={ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFf };
//#include <gpsd_config.h>
//#include <gpsdclient.h>
//#include "novas/netmanagement.h"

//List_locT * locT; //variable global
List_lsp * lsp;
//extern struct ev_loop * l_LPV;
//extern ev_timer t_LPV;
//extern int SN_g;
extern itsnet_node_id GN_ADDR;

itsnet_position_vector * LPV;

int revents;

ev_timer t_Loct;

//struct ev_loop  *addr[10];
unsigned short variables[16]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
bool taken[16];bool taken_rep[16];bool taken_lsp[16];
//struct ev_loop * b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10;



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
static tTimer *mpTimerList = NULL;
static tTimer *mpTimerList_lsp = NULL;
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

void CheckTimerEvent()
{	printf("5555555\n");
	signal(SIGINT, CheckTimerEvent);
	unsigned short nTimer;

    // Read the global variable gTimer and reset the value
	nTimer = gTimer;
	gTimer = 0;
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
		//printf("ALARMA 11111111\n");
	tTimer *pTimer;
	// Update the timers
	pTimer = mpTimerList;
	while(pTimer != NULL)
    { 	if(pTimer->Period != 0)
        {  	pTimer->Period--;
      	if(pTimer->Period == 0)
            { 		// Set the corresponding bit when the timer reaches zero
            	gTimer = gTimer | pTimer->TimerId;
            }        }
    	// Move to the next timer in the list
    	pTimer = pTimer->pNext;
    }
//	printf("ENTRA O SIGNAL\n");
	cont++;
	if (cont==5){cont=0;raise(SIGINT);}//eliminaríase esto, que está como un exemplo
	if (gTimer!=0){//raise(SIGINT);
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


	}
}



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

	//addr[0] = &b0;	addr[1] = &b1;	addr[2] = &b2;	addr[3] = &b3;	addr[4] = &b4;	addr[5] = &b5;	addr[6] = &b6;
	//addr[7] = &b7;addr[8] = &b8;addr[9] = &b9;addr[10] = &b10;
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
List_locT * locT = init_locT(); //probablemente teña que facer esto aquí para tódalas listas/buffers.
//LPV= LPV_update(l_LPV,&t_LPV, revents );
struct parametros * argumento;

/**argumento = (struct parametros *) malloc (sizeof (struct parametros));

	argumento->num=num;
	argumento->locT=locT;
	 argumento->pos=locT->end;**/ //(void *)argumento

//ev_timer_again(*l_LPV,t_LPV);
//ev_run(l_LPV,0);

printf("aqui chego1\n");
return (locT);
}
static 	gps_data_t gpsdata;

itsnet_position_vector * LPV_update(){
	//itsnet_position_vector * LPV;
	printf ("ENTRO NO DO LPV\n");
	LPV=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	printf("e tamén saio \n");
	gps_open("localhost","2947",&gpsdata);
	printf("e tamén saio \n");
	if (&gpsdata == NULL) {
			printf("Could not connect to gpsd!\n");

		}


	(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);//gpsdata.dev);
	    /* Put this in a loop with a call to a high resolution sleep () in it. */
	   int i;
	for(i = 0; i < 100; i++){
	if (gps_waiting (&gpsdata, 50000000))
	    {
	        if (gps_read (&gpsdata) == -1) {//printf ("ENTRO MAL NO read\n");
	          } else {//printf ("ENTRO NO read\n");
	            /* Display data from the GPS receiver. */
	if (gpsdata.fix.mode>=2 && 	(gpsdata.fix.epx>=0 && gpsdata.fix.epx<366) && 	(gpsdata.fix.epy>=0 && gpsdata.fix.epy<366) && 	(gpsdata.fix.epv>=0 && gpsdata.fix.epv<366)){

	/**	printf ("ENTRO NO if do read\n");
	printf("latitude %f\n",gpsdata.fix.latitude);
	printf("longitude %f\n",gpsdata.fix.longitude);
	printf("altitude %f\n",gpsdata.fix.altitude);
	printf("TST %f\n",gpsdata.fix.time);
	printf("speed %f\n",gpsdata.fix.speed);
	printf("heading %f\n",gpsdata.fix.track);
	printf("TAcc %f\n",gpsdata.fix.ept);
	printf("PosAccX %f\n",gpsdata.fix.epx);
	printf("PosAccY %f\n",gpsdata.fix.epy);
	printf("SAcc %f\n",gpsdata.fix.eps);
	printf("HAcc %f\n",gpsdata.fix.ept);
	printf("AltAcc %f\n",gpsdata.fix.epv);
	printf("mode %d \n",gpsdata.fix.mode);**/
	LPV->node_id=GN_ADDR;
	LPV->heading=gpsdata.fix.track *10; //necesitoo en 1/10 degrees e danmo en degrees.
	char str1[2] = {'\0'};	char str2[2] = {'\0'};	char str3[2] = {'\0'};	char str4[2] = {'\0'};	char str5[2] = {'\0'};	char str6[9] = {'\0'};	char str7[9] = {'\0'};
	char str8[9] = {'\0'};	char str9[5] = {'\0'};	char str10[5] = {'\0'};
	//if (memcmp(gpsdata.fix.altitude,nan(sizeof(sou)))){printf("É UNHA NAAAAAN\n");}
	if(gpsdata.fix.ept==0){memcpy(str1,"0",1);}else {sprintf(str1, "%01X",(int) ceil(log2(1000*gpsdata.fix.ept)));}//ceil(log2(1000*gpsdata.fix.ept)));
	if(gpsdata.fix.epx==0 && gpsdata.fix.epx==0){memcpy(str2,"0",1);}else {sprintf(str2, "%01X",(int) ceil(log2(sqrt(pow(gpsdata.fix.epx,2)+(pow(gpsdata.fix.epy,2)))/4)));}
	if(gpsdata.fix.eps==0){memcpy(str3,"0",1);}else {sprintf(str3, "%01X",(int) ceil(log2(100*gpsdata.fix.eps)));}
	if(gpsdata.fix.ept==0){memcpy(str4,"0",1);}else {sprintf(str4, "%01X",(int) ceil(log2((1/0.005493247)*gpsdata.fix.ept)));}
	if(gpsdata.fix.epv==0){memcpy(str5,"0",1);}else {sprintf(str5, "%01X",(int) ceil(log2(gpsdata.fix.epv)));}
	if(gpsdata.fix.latitude<0){sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.latitude * 10000000));}
	if(gpsdata.fix.longitude<0){sprintf(str7, "%08X",(signed int) ceil(gpsdata.fix.longitude * -10000000));}else{sprintf(str6, "%08X",(signed int) ceil(gpsdata.fix.longitude * 10000000));}
	int num0=(int)gpsdata.fix.time *1000;
	sprintf(str8, "%08X",(int) ceil(num0%(4294967296)));
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
	num8=strtol(str8,NULL,16);
	num9=strtol(str9,NULL,16);
	num10=strtol(str10,NULL,16);
	if(gpsdata.fix.latitude<0){LPV->latitude=0xffff-num6;}else{LPV->latitude=num6;}
	if(gpsdata.fix.longitude<0){LPV->longitude=0xffff-num7;}else{LPV->longitude=num7;}
	if(gpsdata.fix.altitude<0){LPV->altitude=0xffff-num9;}else{LPV->altitude=num9;}
	LPV->time_stamp=num8;// recibe Unix time in seconds with fractional part  e quere miliseconds
	//LPV->altitude=num9; //metros en ambos casos
    LPV->speed=num10; //recibe metros por segundo e quere 1/100 m/s
    LPV->accuracy.alt_ac=num5;
    LPV->accuracy.pos_ac=num2;
    LPV->accuracy.speed_ac=num3;
    LPV->accuracy.head_ac=num4;
    LPV->accuracy.time_ac=num1;
   /** printf(str2);printf("\n");
    printf(str3);printf("\n");
    printf(str4);printf("\n");
    printf(str5);printf("\n");
    printf(str6);printf("\n");
    printf("longitude ");printf(str7);printf("\n");
    printf("tst ");printf(str8);printf("\n");
    printf(str9);printf("\n");
    printf(str10);printf("\n");
    //printf("%d\n",num1);    printf("%d\n",num2);    printf("%d\n",num3);    printf("%d\n",num4);    printf("%d\n",num5);
	printf("latitude %d\n",LPV->latitude);
	printf("longitude %d\n",LPV->longitude);
	printf("altitude %d\n",LPV->altitude);//2
	printf("TST %d\n",LPV->time_stamp);//4
	printf("speed %d\n",LPV->speed);//2
	printf("TAcc %d ",LPV->accuracy.time_ac," \n");
	printf("PosAcc %d",LPV->accuracy.pos_ac," \n");
	printf("SAcc %d",LPV->accuracy.speed_ac," \n");
	printf("HAcc %d",LPV->accuracy.head_ac," \n");
	printf("AltAcc %d",LPV->accuracy.alt_ac," \n");**/

	i=100;
	} } }    }
	gps_close (&gpsdata);
	//free(gpsdata);

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
	//int i;
	//for (i=0;i<8;i++ ) addr[i]= rand() % 0xFF;//0x00;//buscar o modo de que sexa random
	return(addr);
}

bool mngdaddrconf(int option) //1: initial, 2:update,3:duplicate detection
{
bool correct=false;
	switch(option)
			{
				case 1: //initial

					printf("envío unha primitiva GN-MGMT.request\n");
					break;
				case 2://update

					printf("envío primitiva GN-MGMT.request");
					break;
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
	//printf("porto %d \n",arg->socket_fd);
			get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	//get_mac_address(arg->socket_fd, "wlan0",(unsigned char *) h_source) ;
	ieee80211_frame_t *tx_frame = init_ieee80211_frame(arg->forwarding_port, ETH_BROAD,h_source);
	//tx_frame->buffer.frame_type=0xdc05;
	//		tx_frame->buffer.frame_len = sizeof(arg->data) +42;
	char tipo[2]={0x07,0x07};
	memcpy(tx_frame->buffer.header.type,tipo,2);
	//memcpy(tx_frame->buffer.header.type,tipo,2);
	const unsigned char beaco[1]={0x01};
	version_nh * res=NULL;
	itsnet_packet * pkt = NULL;
	trafficclass_t *tc;
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
//	pkt->common_header.txpower="0"
	memset(pkt->common_header.hop_limit,1,1);
	pkt->common_header.forwarder_position_vector=*LPV;
	tc->reserved=0;
	tc->relevance=itsGnTrafficClassRelevance;
	tc->reliability=itsGnTrafficClassReliability; //non caben esos valores nese espacio
	tc->latency=itsGnTrafficClassLatency;
	memset(pkt->common_header.traffic_class,tc,1);
	//memcpy(portoO,arg->port,2);
	//memcpy(portoD,arg->forwarding_port,2);
	char *num1[3];char *num2[3];
	sprintf(num1,"%04X",(unsigned int)arg->port);	sprintf(num2,"%04X",(unsigned int)arg->forwarding_port);
	memcpy(pkt->payload.itsnet_beacon.payload.btp1,num1,4);	memcpy(pkt->payload.itsnet_beacon.payload.btp2,num2,4);
memcpy(tx_frame->buffer.data, (char *) pkt, sizeof(itsnet_packet) );
	// 2) broadcast application level UDP message to network level
int fwd_bytes = send_message((sockaddr_t *)arg->forwarding_addr,arg->forwarding_socket_fd,&tx_frame->buffer, sizeof(itsnet_common_header)+sizeof(itsnet_btp_wo_payload_t)+14);
//printf(" %d \n",sizeof(itsnet_btp_wo_payload_t )+sizeof(itsnet_common_header)+14);
 //valor do PV
	//inicializar o timer para a transmission periodica de beacons Tbeacons
	//recorda que hai que controlar sempre o Tbeacon!! //crear timer que fagan un evento ó terminar
printf("beacon enviada\n");
//print_hex_data(&tx_frame->buffer,  sizeof(itsnet_common_header)+sizeof(itsnet_btp_wo_payload_t)+14);

//free(res); free(tc); free(flags); free(pkt);
}



List_locT * init_locT ()
{
	List_locT * locT;
	locT = (List_locT *) malloc (sizeof (List_locT));
	locT->init = NULL;
	locT->end = NULL;
	locT ->len = 0;
    printf("AQUI en init ! %d \n",locT->len);

	return(locT);
}

/*add at list end  */
int add_end_locT ( List_locT * locT, itsnet_node data){
//	pthread_t h1;

  Element_locT *new_element;
 new_element = (Element_locT *) malloc (sizeof (Element_locT));
 if (new_element==NULL) printf( "No hay memoria disponible!\n");
 new_element->data= data;
 new_element->next = NULL;
 //lement_locT *inicio = (Element_locT *) malloc (sizeof (Element_locT));

 if (locT->init==NULL) {
      new_element->before=locT->init; new_element->next=locT->end; locT->init=new_element;  } else {
	 //printf( "Non é primeiro elemento\n");
new_element->before=locT->end;
	 locT->end->next = new_element;}

 locT->end = new_element;

  locT ->len++;
//argumento->thread=h1;
int num2,a=0;
unsigned short num=0;
while(a==0){
num2=(num2 +1) % 16;
if (taken[num2]==false){num=variables[num2]; taken[num2]=true; a=1;}
}


//pthread_create(&h1,NULL,thr_h2,(void *)argumento);
AddTimer(num,itsGnLifetimeLocTE);
printf("saio de add\n");
  return 0;
}

/* erase after a position */
int sup_elem_locT (struct parametros *p)//(EV_P_ ev_timer *w, int revents)// void * arg){//EV_P_ ev_timer *w,
{
	 // if (locT->len <= 1 || pos < 1 || pos >= locT ->len)
	  //   return -1;
  int i;
  printf("entro en cb\n");
 // struct parametros *p;
  //   p=(struct parametros *) w->data;

  Element_locT *pos=p->pos;
 // pthread_t h2=p->thread;

    List_locT * locT=p->locT;
    printf("entro en cb\n");
   printf("AQUI en suprimir ! %d \n",locT->len);
    printf("entro en cb\n");
    if(pos->before==NULL){
    	printf("eliminamos o primeiro \n");
    	locT->init=locT->init->next;
    	if(locT->len==1){locT->end=NULL;    	printf("eliminamos o unico \n");}else{locT->init->before=NULL;}
    }else if (pos->next==NULL){    	printf("eliminamos o ultimo \n");
    	locT->end->before->next=NULL;
    	locT->end=locT->end->before;
    }else{
    	printf("eliminamos outro \n");
    	pos->before->next=pos->next;
    	pos->next->before=pos->before;
    }

  printf("chego aqui \n"); //  free (pos);printf("chego aquitamén \n");
     // free(&pos->data);
     locT->len--;
     printf("saimos de suprimir elemento da loct1\n");
  //   ev_timer_stop (EV_A_ w);
     variables[p->num]=0;
  //   pthread_join(h2, NULL);
  printf("saimos de suprimir elemento da loct1\n");
  return 0;
}

/* view List */
void view_locT (List_locT * locT){
  Element_locT *actual;
  actual = locT->init;
  while (actual != NULL){
      //printf ("%p - %s\n", actual, actual->data);
      actual = actual->next;
  }
}

int search_in_locT (itsnet_node * data, List_locT * locT){
  Element_locT *actual;
  actual = locT->init;
  int i=0;
  while (actual != NULL){
      //printf ("%p - %s\n", actual, actual->data);
	  if (strcmp(actual->data.mac_id.address,data->mac_id.address)==0){
		  i=1;
if(((actual->data.pos_vector.time_stamp < data->pos_vector.time_stamp ) &&(data->pos_vector.time_stamp - actual->data.pos_vector.time_stamp <=4294967296/2))||((actual->data.pos_vector.time_stamp > data->pos_vector.time_stamp)&&(-data->pos_vector.time_stamp + actual->data.pos_vector.time_stamp >4294967296/2)))
{
actual->data.pos_vector=data->pos_vector;
actual->data.expires.tv_sec= itsGnLifetimeLocTE;
		  actual->data.IS_NEIGHBOUR=true;
	  }
	  }
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

	List_lsp * lsp;
	lsp = (List_lsp *) malloc (sizeof (List_lsp));
	lsp ->init = NULL;
	lsp ->end = NULL;
	lsp ->len = 0;
	return(lsp);
}

/*add at list end  */
int add_end_lsp ( List_lsp * lsp, itsnet_packet data){

  Element_lsp *new_element;
  new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
  if (new_element==NULL) printf( "No hay memoria disponible!\n");
 new_element->data= data;
 new_element->next = NULL;
 if (lsp->init==NULL) {
	  new_element->before=lsp->init; new_element->next=lsp->end; lsp->init=new_element;  } else {
			 //printf( "Non é primeiro elemento\n");
		new_element->before=lsp->end;
		lsp->end->next = new_element;}

 lsp->end = new_element;

 lsp ->len++;
		  printf("AQUI en engadir ! %d \n",lsp->len);
		//argumento->thread=h1;
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

  Element_lsp *new_element;
  new_element = (Element_lsp *) malloc (sizeof (Element_lsp));
  if (new_element==NULL) printf( "No hay memoria disponible!\n");
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
		//argumento->thread=h1;
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
	 // pthread_t h2=p->thread;

	    List_lsp * lsp=p->lsp;
	    printf("entro en cb\n");
	   printf("AQUI en suprimir ! %d \n",lsp->len);
	    printf("entro en cb\n");
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

	  printf("chego aqui \n"); //  free (pos);printf("chego aquitamén \n");
	     // free(&pos->data);
	  lsp->len--;
	     printf("saimos de suprimir elemento da loct1\n");
	  //   ev_timer_stop (EV_A_ w);
	     variables[p->num]=0;
	  //   pthread_join(h2, NULL);
	  printf("saimos de suprimir elemento da loct1\n");
	  return 0;
}

/* view List */
void view_lsp(){
  Element_lsp *actual;
  actual = lsp->init;
  while (actual != NULL){
   //   printf ("%p - %s\n", actual, actual->data);
      actual = actual->next;
  }
}



int duplicate_control(void * data,List_locT * locT){

	Element_locT *actual;
	  actual = locT->init;
	  int i=1;
	  itsnet_node_id * dato=NULL;//
	 dato= (itsnet_node_id *)malloc(sizeof(itsnet_node_id));
	  memcpy(dato,data +8,8);
	  char *SN= (char *)malloc(2);
	  memcpy(SN,data+36,2);
		int lon_int=sprint_hex_data( SN, 2);
	  while (actual != NULL){
	      //printf ("%p - %s\n", actual, actual->data);
		  if (actual->data.node_id.id==dato->id){
			  i=0;
	if(((actual->data.Sequence_number < lon_int ) &&(lon_int- actual->data.Sequence_number <=65536/2))||((actual->data.Sequence_number > lon_int)&&(-lon_int + actual->data.Sequence_number >65536/2)))
	{	actual->data.Sequence_number=lon_int;		  }		  }
	      actual = actual->next;
	  }//free(SN);
	  return(i);}
/**



**/






