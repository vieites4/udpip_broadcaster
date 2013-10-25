/**
 * @file main.h
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
 * This file is part of udpip-broadcaster.
 * udpip-broadcaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * udpip-broadcaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with udpip-broadcaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Header file with main dependencies for this application and all of its
 * components.
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "configuration.h"

//MIB ATTRIBUTES

//#define itsGnLocalGnAddr GNAD;// {"0x14","0x00","0x00","0x00","0x00","0x00","0x00","0x00"} //é do tipo itnet_node_id



#include "udpev/udp_events.h"
#include "udpev/cb_udp_events.h"
#include "novas/netmanagement.h"
#include "execution_codes.h"



/*!< Flag for activating the verbose mode. */
extern bool __verbose;
void *thr_h1();
void print_version();
void print_help();

#endif /* MAIN_H_ */

