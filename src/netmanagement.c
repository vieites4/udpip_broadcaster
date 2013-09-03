
#include "main.h"
#include "itsnet_header.h"
void startup()
{
printf("comprobo o valor de itsGnLocalAddrConfMethod\n");
//if (itsGnLocalAddrConfMethod=AUTO(0))
//	autoaddrconf();
//else
//	mngdaddrconf(1);
printf("teño que enviar unha Beacon\n");
Beacon_send();
LPV_ini();//teñen que estar todos os elementos a 0-> páx 16
}


itsnet_node_id autoaddrconf(){

	//teño que definir o struct das GN_ADDR

	itsnet_node_id addr;
	int i;
	for (i=0;i<8;i++)	addr[0]=rand() % 0xFF;//0x00;//buscar o modo de que sexa random
	return(addr);

}

bool mngdaddrconf(int opcion) //1: initial, 2:update,3:duplicate detection
{
bool correcto=false;
	switch(opcion)
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
return(correcto);

}


void LPV_update(){}
void Time_update(){}

void Beacon_send(){

	//crear unha Beacon.
	//algo dunha request?!
	//enviar cara LL a beacon con destino broadcast
	//inicializar o timer para a transmission periodica de beacons Tbeacons

	//recorda que hai que controlar sempre o Tbeacon!! //crear timer que fagan un evento ó terminar


}


void LS(int opcion){
	switch(opcion)
				{
					case 1: //initial LS Request
					{
						printf("initial LS Request\n");
//						if (LS_pending){
//							paquete_a_buffer();
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


			if( RTCls_gnaddr<itsGnLocationServiceMaxRetrans){
//
			//reenviar o packete LS Request como un TSB.
			//restart Tls_gnaddr=itsGnLocationServiceRetransmitTimer
			//RTCls_gnaddr++;

			}
//			else{
				//flush the LS packet buffer for the sought GN_ADDR and discard the stored packets;
				//remove the LocTE for the sought(desexado) GN_ADDR

//			}

			printf("LS Request re-transmission");


						break;

					}
					default://LS Reply //sería a opcion 3
					{
						//if the source receives a LS Reply packet for the sought GN_ADDR


				//Common Header processing
				if( mngdaddrconf(3)){

					//discard the packet
					break;
				}else{

//update SO PVlocT=PV do LS Reply Extended Header [algoritmo B.2]
	//				if (so/=se){ IS_NEIGHBOUR=FALSE}


				}


						printf("LS Reply\n");

					break;
				}}



}
