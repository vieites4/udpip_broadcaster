//#include "main.c"

//2947 porto para gps, deixo correndo o sistema e
#include "netmanagement.h"
#include "/home/pc/Descargas/gpsd-3.9/gps.h"
//#include <gpsd_config.h>
//#include <gpsdclient.h>
//#include "novas/netmanagement.h"

List_locT * locT; //variable global
List_lsp * lsp;

//extern int SN_g;
extern itsnet_node_id GN_ADDR;
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


//gpsd_activate();
int af;

af=gps_open("localhost","2947",&gpsdata);

if (&gpsdata == NULL) {
		g_print("Could not connect to gpsd!\n");

	}

printf("an %d\n",af);

//if (gps_waiting(&gpsdata, 5000000)) {
//af=gps_read(&gpsdata);
//printf("an\n");
//}
//printf("o gps funciona %d\n",a);
//gps_set_raw_hook(gpsdata, (void *)updateGPSdata);
  //      gps_query(gpsdata, "w+x\n");


(void) gps_stream(&gpsdata, WATCH_ENABLE, NULL);
    /* Put this in a loop with a call to a high resolution sleep () in it. */
    if (gps_waiting (&gpsdata, 500)) {
        errno = 0;
        if (gps_read (&gpsdata) == -1) {
          } else {
            /* Display data from the GPS receiver. */
        	    char * datos;
        	  datos=gps_data(&gpsdata);
        	  printf("latitude %f\n",gpsdata.fix.latitude);
            printf("\n");
        }
    }

    /* When you are done... */
//   gps_stream(&gpsdata, WATCH_ENABLE, datos);

    gps_close (&gpsdata);






//gps_stream(&gpsdata, WATCH_ENABLE,NULL);
//printf("%d\n",gpsdata.PRN,);
  //      gps_close(&gpsdata);


//void * data;
//bool a=gps_waiting(gpsdata,af);
//printf("o gps funciona %d\n",a);
//gps_stream(gpsdata, WATCH_ENABLE, data);
//gps_close(gpsdata);



	locT = init_locT(); //probablemente teña que facer esto aquí para tódalas listas/buffers.



//if (itsGnLocalAddrConfMethod=AUTO(0))
//	autoaddrconf();
//else
//	mngdaddrconf(1);
printf("teño que enviar unha Beacon\n");
//Beacon_send();
//LPV_ini();//teñen que estar todos os elementos a 0-> páx 16
return (locT);
}

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


void LPV_update(){}
void Time_update(){}

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

































