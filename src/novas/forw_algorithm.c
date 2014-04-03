/*
 * forw_algorithm.c
 *
 *  Created on: 19/12/2013
 *      Author: pc
 */

#include <sys/time.h>
#include "forw_algorithm.h"
extern itsnet_position_vector * LPV;
extern const unsigned char ZEROS[ETH_ALEN];
extern const unsigned char TWOS[ETH_ALEN];
extern const unsigned char geobroad0[1];
extern const unsigned char geobroad1[1];
extern const unsigned char geobroad2[1];
extern const unsigned char ETH_ADDR_BROADCAST[6];
extern List_locT *locT_general;
extern List_lsp * lsp_cbf_uc;
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif

int Distance(itsnet_position_vector *lpv_a,itsnet_position_vector *lpv_b){
	int dist=0;
	int base=0; int high=0;
	int aa=lpv_a->latitude;int ab=lpv_b->latitude;
	int oa=lpv_a->longitude;int ob=lpv_b->longitude;

	if (aa>ab){base=aa-ab;}else{base=ab-aa;}
	if (oa>ob){high=oa-ob;}else{high=ob-oa;}

	dist=sqrt(pow(base,2)+pow(high,2));
	return(dist);
}

mac_addr Greedy_Forwarding_UC(itsnet_position_vector *lpv_p){
	mac_addr NH;
	memset(&NH,0,6);
	int dist=0;
	int MFR= Distance(LPV,lpv_p);
	Element_locT * pos=locT_general->init;
	while(pos!=NULL){

		if(pos->data.IS_NEIGHBOUR){
			dist=Distance(lpv_p, &pos->data.pos_vector);
			if (dist<MFR){
				NH=pos->data.mac_id;
				MFR=dist;
				pos=pos->next;
			}
		}else pos=pos->next;	}
	return(NH);
}


int CBF_UC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se){

	Element_lsp * pos=lsp_cbf_uc->init;
	while(pos!=NULL){
		if (memcmp(pkt,&pos->data,len)==0) {
			//remove p from buffer
			sup_elem_t_lsp(pkt->payload.itsnet_unicast.sequencenumber,6);
			//stop timer
			return(-1);}else pos=pos->next;	}
	int pos_locT=search_in_locT(lpv_se,locT_general);
	Element_locT * p_locT=locT_general->init;
	int aa=0;int cont=0;
	while(p_locT!=NULL && aa==0 ){
		if(cont==pos_locT){aa=1;}else{cont++;p_locT=p_locT->next;}	}
	int prog=0;int timeout=0;
	if(pos_locT>0 && p_locT->data.pos_vector.pai==1){
		prog=0;
		//prog=distance(pkt->payload.itsnet_unicast.dest_position_vector,p_locT->data.pos_vector)-distance(pkt->payload.itsnet_unicast.dest_position_vector,LPV);
		if (prog>0){
			add_end_lsp(lsp_cbf_uc,*pkt,6);
			//set time out
			if (prog<=itsGnDefaultMaxCommunicationRange){
				timeout=itsGnGeoUnicastCbfMaxTime +(((itsGnGeoUnicastCbfMinTime-itsGnGeoUnicastCbfMaxTime)/itsGnDefaultMaxCommunicationRange)*prog);
			}else{timeout=itsGnGeoUnicastCbfMinTime;}
			//start timer timeout
			AddTimer(pkt->payload.itsnet_unicast.sequencenumber,timeout,6);


			return(0);
		}else{			//discard p
			return(-1);		}
	}else{
		add_end_lsp(lsp_cbf_uc,*pkt,6);//engadir AddTimer();
		timeout=itsGnGeoUnicastCbfMaxTime;//set timeout  to_cbf_max
		AddTimer(pkt->payload.itsnet_unicast.sequencenumber,timeout,6);
		return(0);
	}}

mac_addr CBF_BC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se){

	Element_lsp * pos=lsp_cbf_uc->init;mac_addr nh;
	while(pos!=NULL){
		if (memcmp(pkt,&pos->data,len)==0) {			//remove p from buffer
			sup_elem_t_lsp(pkt->payload.itsnet_geobroadcast.sequencenumber,6);			//stop timer
			memcpy(nh.address,TWOS,6);
			return(nh);}else pos=pos->next;	}
	int pos_locT=search_in_locT(lpv_se,locT_general);
	Element_locT * p_locT=locT_general->init;
	int aa=0;int cont=0;
	while(p_locT!=NULL && aa==0 ){
		if(cont==pos_locT){aa=1;}else{cont++;p_locT=p_locT->next;}	}
	int dist=0;int timeout=0;
	int y=geo_limit_ll(pkt,LPV); // non teño claro que aqui sexa pkt o que vaia dentro.. revisar o de F() aka distance

	if(y>=0){
		add_end_lsp(lsp_cbf_uc,*pkt,6);
		if(pos_locT!=0 && p_locT->data.pos_vector.pai==1 ){
			dist=Distance(lpv_se, LPV);
			if (dist<=itsGnGeoUnicastCbfMaxTime){
				timeout=itsGnGeoUnicastCbfMaxTime +(((itsGnGeoUnicastCbfMinTime-itsGnGeoUnicastCbfMaxTime)/itsGnDefaultMaxCommunicationRange)*dist);
			}else{timeout=itsGnGeoUnicastCbfMinTime;}
		} else timeout=itsGnGeoUnicastCbfMaxTime;
		AddTimer(pkt->payload.itsnet_geobroadcast.sequencenumber,timeout,6);
		memcpy(nh.address,ZEROS,6);
		return(nh);
	}else
	{		if(pos_locT!=0 && p_locT->data.pos_vector.pai==1 ){
		y=geo_limit_ll(pkt,lpv_se);
		if(y<0){
			itsnet_position_vector A;
			A.latitude=pkt->payload.itsnet_geobroadcast.dest_latitude;
			A.longitude=pkt->payload.itsnet_geobroadcast.dest_longitude;
			mac_addr NH=Greedy_Forwarding_UC(&A);
			return(NH);
		}else{memcpy(nh.address,TWOS,6);return(nh);}
	}else {memcpy(nh.address,ETH_ADDR_BROADCAST,6);return(nh);}	}
}

int G(itsnet_position_vector * lpv_se,itsnet_position_vector * lpv_f){
	int DIST_R=Distance(LPV,lpv_se);
	int DIST_F=Distance(lpv_se,lpv_f);
	int FSR=asin(Distance(lpv_f,LPV)/Distance(lpv_se,lpv_f));
	if((DIST_R<DIST_F)&&(DIST_F<itsGnDefaultMaxCommunicationRange)&&(FSR<=itsGnBroadcastCBFDefSectorAngle)){return(1);}else return(-1);
}

mac_addr Advanced_BC(itsnet_packet * pkt,int len,itsnet_position_vector * lpv_se,mac_addr *PKT_mac,char h_source[ETH_ALEN]){
	mac_addr nh;int counter; int max_counter;int timeout=0;
	memcpy(nh.address,TWOS,6);
	itsnet_position_vector A;
	A.latitude=pkt->payload.itsnet_geobroadcast.dest_latitude;
	A.longitude=pkt->payload.itsnet_geobroadcast.dest_longitude;
	int INOUT1=geo_limit(&pkt->common_header.HT_HST,pkt,LPV);int aa=0;
	if(INOUT1>=0){
		Element_lsp * pos=lsp_cbf_uc->init;
		while(pos!=NULL && aa==0){
			if (memcmp(pkt,&pos->data,len)==0) {aa=1;}	else pos=pos->next;}
		if(aa==1){//P IN B
			if(counter>=max_counter){
				//remove p from buffer
				sup_elem_t_lsp(pkt->payload.itsnet_geobroadcast.sequencenumber,6);
				//stop timer
				memcpy(nh.address,TWOS,6);}
			else{
				itsnet_position_vector *lpv_f=NULL;
				lpv_f->latitude=sprint_hex_data(pkt->payload.itsnet_geobroadcast.dest_latitude,4);
				lpv_f->longitude=sprint_hex_data(pkt->payload.itsnet_geobroadcast.dest_longitude,4);
				int INOUT2=G(lpv_se,lpv_f);
				if(INOUT2>=0){
					while(pos!=NULL){
						if (memcmp(pkt,&pos->data,len)==0) {
							//remove p from buffer
							sup_elem_t_lsp(pkt->payload.itsnet_geobroadcast.sequencenumber,6);
							//stop timer
							memcpy(nh.address,TWOS,6);
							return(nh);}else pos=pos->next;}
				}else{
					int dist=Distance(lpv_se, LPV);
					if (dist<=itsGnGeoUnicastCbfMaxTime){
						timeout=itsGnGeoUnicastCbfMaxTime +(((itsGnGeoUnicastCbfMinTime-itsGnGeoUnicastCbfMaxTime)/itsGnDefaultMaxCommunicationRange)*dist);
					}else{timeout=itsGnGeoUnicastCbfMaxTime;}
					AddTimer(pkt->payload.itsnet_geobroadcast.sequencenumber,timeout,6);
					counter++;
				}	}
		}else{
			add_end_lsp(lsp_cbf_uc,*pkt,6);
			if(memcmp(PKT_mac->address,h_source,6)==0){
				counter=1;

				nh=Greedy_Forwarding_UC(&A);
				timeout=itsGnGeoUnicastCbfMaxTime;
				AddTimer(pkt->payload.itsnet_geobroadcast.sequencenumber,timeout,6);
				return(nh);
			}else{
				int pos_locT=search_in_locT(lpv_se,locT_general);
				Element_locT * p_locT=locT_general->init;
				int aa=0;int cont=0;
				while(p_locT!=NULL && aa==0 ){
					if(cont==pos_locT){aa=1;}else{cont++;p_locT=p_locT->next;}				}
				if(aa==1 && p_locT->data.pos_vector.pai==1){
					int dist=Distance(lpv_se, LPV);
					if (dist<=itsGnGeoUnicastCbfMaxTime){
						timeout=itsGnGeoUnicastCbfMaxTime +(((itsGnGeoUnicastCbfMinTime-itsGnGeoUnicastCbfMaxTime)/itsGnDefaultMaxCommunicationRange)*dist);
					}else{timeout=itsGnGeoUnicastCbfMinTime;}
				}else timeout=itsGnGeoUnicastCbfMaxTime;
				AddTimer(pkt->payload.itsnet_geobroadcast.sequencenumber,timeout,6);
				memcpy(nh.address,ZEROS,6);return(nh);			}
		}}else{
			int pos_locT=search_in_locT(lpv_se,locT_general);
			Element_locT * p_locT=locT_general->init;
			int aa=0;int cont=0;
			while(p_locT!=NULL && aa==0 ){
				if(cont==pos_locT){aa=1;}else{cont++;p_locT=p_locT->next;}
			}
			if(aa==1 && p_locT->data.pos_vector.pai==1){
				int INOUT3=geo_limit(&pkt->common_header.HT_HST,pkt,lpv_se);
				if(INOUT3<0){ nh=Greedy_Forwarding_UC(&A);return(nh);}else{	memcpy(nh.address,TWOS,6);return(nh);}
			}else {memcpy(nh.address,ETH_ADDR_BROADCAST,6);return(nh);}	}
	return(nh);
}
int geo_limit(void *HT,itsnet_packet_f *pkt,itsnet_position_vector * lpv_se)
{
	int x,y,r,total,a,b;
	x=abs(pkt->common_header.pv.latitude - lpv_se->latitude)/111;
	y=abs(pkt->common_header.pv.longitude - lpv_se->longitude)/111;
	char distA[2];
	char distB[2];
	memcpy(distA,pkt->payload.itsnet_geocast.distanceA,2);
	memcpy(distB,pkt->payload.itsnet_geocast.distanceB,2);
	if(memcmp(geobroad0,HT,1)==0){
		//PRF("circular \n");print_hex_data(distA,2);
		r=sprint_hex_data(distA, 2);
		total= 1- pow((x/r),2) - pow((y/r),2);
	}else
		if(memcmp(geobroad1,HT,1)==0){
			a=sprint_hex_data(distA, 2);
			b=sprint_hex_data(distB, 2);
			total= fmin(1- pow((x/a),2) ,1- pow((y/b),2));
			PRF("rectangular \n");
		} else
			if(memcmp(geobroad2,HT,1)==0){
				a=sprint_hex_data(distA, 2);
				b=sprint_hex_data(distB, 2);
				total= 1- pow((x/a),2) - pow((y/b),2);
				PRF("eliptica \n");
			}
	//free(pkt1);pkt1=NULL;
	return(total);
}


int geo_limit_ll(itsnet_packet *pkt,itsnet_position_vector * lpv_se)
{
	int x,y,r,total,a,b;
	x=abs(pkt->payload.itsnet_geobroadcast.dest_latitude - lpv_se->latitude);
	y=abs(pkt->payload.itsnet_geobroadcast.dest_longitude - lpv_se->longitude);
	char distA[2];
	char distB[2];
	memcpy(distA,pkt->payload.itsnet_geobroadcast.distanceA,2);
	memcpy(distB,pkt->payload.itsnet_geobroadcast.distanceB,2);
	char HT[1];
	memcpy(HT,(void *)&pkt->basic_header.version_nh,1);
	if(memcmp(geobroad0,HT,1)==0){
		//PRF("circular \n");print_hex_data(distA,2);
		r=sprint_hex_data(distA, 2);
		total= 1- pow((x/r),2) - pow((y/r),2);
	}else
		if(memcmp(geobroad1,HT,1)==0){
			a=sprint_hex_data(distA, 2);
			b=sprint_hex_data(distB, 2);
			total= fmin(1- pow((x/a),2) ,1- pow((y/b),2));
			PRF("rectangular \n");
		} else
			if(memcmp(geobroad2,HT,1)==0){
				a=sprint_hex_data(distA, 2);
				b=sprint_hex_data(distB, 2);
				total= 1- pow((x/a),2) - pow((y/b),2);
				PRF("eliptica \n");			}
	//free(pkt1);pkt1=NULL;
	return(total);
}
