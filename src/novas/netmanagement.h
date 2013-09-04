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
//#include "main.h"
#include "itsnet_header.h"
void startup1();


void LPV_update();
void Time_update();

void Beacon_send();
void LS(int option);

#endif /* NETMANAGEMENT_H_ */
