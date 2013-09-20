//#include "main.c"

//2947 porto para gps, deixo correndo o sistema e
#include "netmanagement.h"
#include "/home/pc/Descargas/gpsd-3.9/gps.h"
#include "/home/pc/Descargas/gpsd-3.9/gpsdclient.h"
//#include <gpsd_config.h>
//#include <gpsdclient.h>
//#include "novas/netmanagement.h"

List_locT * locT; //variable global
List_lsp * lsp;

//extern int SN_g;
extern itsnet_node_id GN_ADDR;

//static struct fixsource_t source;
itsnet_position_vector * LPV;
struct gps_data_t gpsdata;

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
locT = init_locT(); //probablemente teña que facer esto aquí para tódalas listas/buffers.
printf("teño que enviar unha Beacon\n");
//Beacon_send();
//LPV=LPV_ini();//teñen que estar todos os elementos a 0-> páx 16
LPV= LPV_update( );

return (locT);
}

itsnet_position_vector * LPV_update(){
	itsnet_position_vector * LPV;
	LPV=(itsnet_position_vector *)malloc(sizeof(itsnet_position_vector));
	gps_open("localhost","2947",&gpsdata);

	if (&gpsdata == NULL) {
			g_print("Could not connect to gpsd!\n");

		}
	(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);//gpsdata.dev);
	    /* Put this in a loop with a call to a high resolution sleep () in it. */
	   int i;
	for(i = 0; i < 10; i++){
	if (gps_waiting (&gpsdata, 50000000))
	    {
	        if (gps_read (&gpsdata) == -1) {
	          } else {


	        //	  gps_poll(&gpsdata);
	            /* Display data from the GPS receiver. */
	if (gpsdata.fix.mode>=2){
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
	printf("mode %d \n",gpsdata.fix.mode);
	i=10;


	LPV->altitude=gpsdata.fix.altitude; //metros en ambos casos

	LPV->longitude=gpsdata.fix.longitude * 10000000;//recibe en degrees e quereo en 1/10 micro degrees
	LPV->latitude=gpsdata.fix.latitude * 10000000;
	LPV->time_stamp=gpsdata.fix.time *1000;// recibe Unix time in seconds with fractional part  e quere miliseconds
	LPV->speed=gpsdata.fix.speed /100 ; //recibe metros por segundo e quere 1/100 m/s
	LPV->node_id=GN_ADDR;
	LPV->heading=gpsdata.fix.track *10; //necesitoo en 1/10 degrees e danmo en degrees.
	LPV->accuracy.alt_ac=ceil(log2(gpsdata.fix.epv));
	LPV->accuracy.pos_ac=ceil(log2(sqrt(pow(gpsdata.fix.epx,2)+(pow(gpsdata.fix.epy,2)))/4));
	LPV->accuracy.speed_ac=ceil(log2(100*gpsdata.fix.eps));

	char* cadena1,entero1_b;
	cadena1=(char *) malloc (3*sizeof(char));
	printf("an\n");
	int entero1=ceil(log2((1/0.005493247)*gpsdata.fix.ept));
	printf("an\n");
	sprintf(cadena1, "%d", entero1);
	printf("an1\n");
	int entero_bin=strtol(cadena1,NULL,2);
	printf("an2\n");
	sprintf( entero1_b, "%d", entero_bin);
	printf("an3\n");
	printf(cadena1);
	printf("an\n");
	memcpy(LPV->accuracy.head_ac,entero1_b,3);
	printf("an\n");
	LPV->accuracy.time_ac=ceil(log2(1000*gpsdata.fix.ept));
	printf("an\n");

	printf("latitude %f\n",LPV->latitude);
	printf("longitude %f\n",LPV->longitude);
	printf("altitude %f\n",LPV->altitude);
	printf("TST %f\n",LPV->time_stamp);
	printf("speed %f\n",LPV->speed);
	printf("TAcc %f\n",LPV->accuracy.time_ac);
	printf("PosAcc %f\n",LPV->accuracy.pos_ac);
	printf("SAcc %f\n",LPV->accuracy.speed_ac);
	printf("HAcc %f\n",LPV->accuracy.head_ac);
	printf("AltAcc %f\n",ceil(log2(1000*gpsdata.fix.ept)));






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

void Beacon_send(){

	//crear unha Beacon. é SO A COMMONHEADER

	//algo dunha request?!
	//enviar cara LL a beacon con destino broadcast
	//inicializar o timer para a transmission periodica de beacons Tbeacons

	//recorda que hai que controlar sempre o Tbeacon!! //crear timer que fagan un evento ó terminar


}


void LS(int option){
	switch(option)
				{
					case 1: //initial transmission of LS Request
					{
						printf("initial LS Request\n");
//						if (LS_pending){
//							pkt2buffer();
//							break;
//										}
			//			else{printf("envío o paquete LS request");
					     //start timer Tls_gnaddr= itsGnLocationServiceRetransmitTimer"
						//initialize the retransmit counter RTCls_gnaddr==0;
						//add the packet in LocTE and set LS_pending=true
				//
				//		}
						break;
					}
					case 2://LS Request re-transmission
					{
						//aquí entraríamos no caso de que o timer Tls_gnaddr expirara


//			if( RTCls_gnaddr<itsGnLocationServiceMaxRetrans){
//
			//reenviar o packete LS Request como un TSB.
			//restart Tls_gnaddr=itsGnLocationServiceRetransmitTimer
			//RTCls_gnaddr++;

//			}
//			else{
				//flush the LS packet buffer for the sought GN_ADDR and discard the stored packets;
				//remove the LocTE for the sought(desexado) GN_ADDR

//			}

			printf("LS Request re-transmission");


						break;

					}
					case(3)://receive LS Reply //sería a opcion 3
					{
						//if the source receives a LS Reply packet for the sought GN_ADDR

						//------------------------x-----------------////inicio da parte común
				//Common Header processing
				if( mngdaddrconf(3)){

					//discard the packet
				//	break;
				}else{

//update SO PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
	//				if (so/=se){ IS_NEIGHBOUR=FALSE}

				//	if (SO LS_pending){
						//flush packet buffers. SO LS packet buffer
						//forward the stored packets
						// SO LS_pending=false;
						}

			//		if(UC_forwarding_pkt_buffer!=empty){//flush the UC_forwarding_pkt_buffer
						//forward the stored packets

			//		}
					//------------------------x-----------------////fin da parte común

					//flush LS_pkt_buffer for the sought GN_ADDR & forward the stored packets
					//LS_pending=false of GN_ADDR
					//stop Tls_gnaddr
					//reset RTCls_gnaddr


					//}


					printf("LS Reply\n");

										break;



				}


					default:{ //recepción da LS Request
						//despois de comprobar na recepcion  que ten o GN_ADDR correcto

						//Common Header processing
						if( mngdaddrconf(3)){

						//discard the packet
						break;
					}else{
						//B.2
						//IF(SO/=SE)SO IS_NEIGHBOUR=FALSE
						//

					}

					break;
					}
				}



}

List_locT * init_locT ()
{
	List_locT * locT;
	locT = (List_locT *) malloc (sizeof (List_locT));
	locT->init = NULL;
	locT->end = NULL;
	locT ->len = 0;
	return(locT);
}

/*add at list end  */
int add_end_locT ( List_locT * locT, itsnet_node data){
  Element_locT *new_element;
 new_element = (Element_locT *) malloc (sizeof (Element_locT));
 if (new_element==NULL) printf( "No hay memoria disponible!\n");
 new_element->data= data;
 new_element->next = NULL;
 Element_locT *inicio = (Element_locT *) malloc (sizeof (Element_locT));

 if (locT->init==NULL) {
     printf( "Primeiro elemento\n");
     locT->init =  new_element;  } else {
	 printf( "Non é primeiro elemento\n");
	 locT->end->next = new_element;}

 locT->end = new_element;
  locT ->len++;

  return 0;
}

/* erase after a position */
int sup_elem_locT ( int pos){
	 // if (locT->len <= 1 || pos < 1 || pos >= locT ->len)
	  //   return -1;
  int i;
  Element_locT *actual;
  Element_locT *sup_elemento;
  actual = locT ->init;

  for (i = 1; i < pos; ++i)
    actual = actual->next;

  sup_elemento = actual->next;
  actual->next = actual->next->next;
  if(actual->next == NULL)
	  locT->end = actual;
 // free (sup_elemento->data);
  free (sup_elemento);
  locT->len--;
  return 0;
}

/* view List */
void view_locT (){
  Element_locT *actual;
  actual = locT->init;
  while (actual != NULL){
      //printf ("%p - %s\n", actual, actual->data);
      actual = actual->next;
  }
}


bool exist_neighbours(){

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
  	     printf( "Primeiro elemento\n");
  	     lsp->init =  new_element;  } else {
  		 printf( "Non é primeiro elemento\n");
  		 lsp->end->next = new_element;}
   lsp ->end = new_element;
  lsp ->len++;
  return 0;
}

/* erase after a position */
int sup_elem_lsp (int pos){
	 // if (locT->len <= 1 || pos < 1 || pos >= locT ->len)
	  //   return -1;
  int i;
  Element_lsp *actual;
  Element_lsp *sup_elemento;
  actual = lsp ->init;

  for (i = 1; i < pos; ++i)
    actual = actual->next;

  sup_elemento = actual->next;
  actual->next = actual->next->next;
  if(actual->next == NULL)
	  lsp->end = actual;
 // free (sup_elemento->data);
  free (sup_elemento);
  lsp->len--;
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

































