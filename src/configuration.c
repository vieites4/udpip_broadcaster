/*
 * @file configuration.c
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
 */

#include "configuration.h"
#if DEBUG_PRINT_ENABLED
#define PRF printf
#else
#define PRF(format, args...) ((void)0)
#endif
/* new_configuration */
configuration_t *new_configuration()
{

        configuration_t *cfg = NULL;
        cfg = (configuration_t *)malloc(LEN__T_CONFIGURATION);

        memset(cfg, 0, LEN__T_CONFIGURATION);
        cfg->__tx_test = false;
        cfg->__verbose = false;
        cfg->manually=false;
        return(cfg);

}

/* create_configuration */
configuration_t *create_configuration(int argc, char** argv)
{

        configuration_t *cfg = new_configuration();

        read_configuration(argc, argv, cfg);
        check_configuration(cfg);

        return(cfg);

}

/* read_configuration */
int read_configuration(int argc, char** argv, configuration_t* cfg)
{
        bool tic=false;
        int idx = 0, read = 0;

        static struct option args[] =
        {
                        {"help",        no_argument,                NULL,         'h'        },
                        {"verbose",        no_argument,                NULL,        'e'        },
                        //{"version",        required_argument,                NULL,         'v'        },
                        {"nettx",        required_argument,        NULL,         't'        },
                        {"netrx",         required_argument,        NULL,         'r'        },
                        {"apptx",        required_argument,        NULL,         'u'        },
                        {"apprx",         required_argument,        NULL,         'w'        },
                        {"appaddr", required_argument, NULL,        'd'        },
                        {"ifname",        required_argument,        NULL,        'i'        }, //wlan0
                        {"txtest", no_argument,                NULL, 's' },
                        {"app", no_argument,                NULL, 'a' },
                        {"nec",                no_argument,                NULL,        'n' },

                        {"scc",                required_argument,        NULL,        'x' },
                        {"manually",no_argument,                NULL,        'm' },
                        {"itss_type",required_argument,        NULL,        'y' },

                        {0,0,0,0}
        };

        while
                ( ( read = getopt_long(argc, argv, "manhset:r:i:u:w:d:y:x:", args, &idx) )
                                > -1 )
        {

                switch(read)
                {
                case 't':

                        cfg->tx_port = atoi(optarg);
                        break;
                case 'y':

                                        cfg->itss_type= atoi(optarg);

                                        break;
                case 'x':

                                                cfg->scc = atoi(optarg);
                                                break;

                case 'r':

                        cfg->rx_port = atoi(optarg);
                        break;

                case 'd': // se se define esa é a dirección de destino, se non o destino é broadcast

                        if ( strlen(optarg) <= 0 )
                        { handle_app_error("read_configuration: " \
                                        "wrong application address.\n"); }
                        cfg->app_address = optarg;
                        cfg->app_inet_addr = inet_addr(optarg);
                        tic=true;

                        if ( ( cfg->app_inet_addr == 0 ) ||        ( cfg->app_inet_addr < 0 ) ||
                                        ( cfg->app_inet_addr == 0xFFFFFFfF )        )
                        { handle_app_error("read_configuration: " \
                                        "wrong application address.\n"); }

                        break;

                case 'u':

                        cfg->app_tx_port = atoi(optarg);
                        break;

                case 'w':

                        cfg->app_rx_port = atoi(optarg);
                        break;

                case 's':

                        cfg->__tx_test = true;
                        break;
                case 'm':

                                        cfg->manually = true;
                                        break;

                case 'i':

                        if ( strlen(optarg) > IF_NAMESIZE )
                        {
                        	PRF("[WARNING] if_name length = %d, maximum = %d.\
                                                                TRUNCATING!\n", (int)strlen(optarg), \
                                                                IF_NAMESIZE);
                        }

                        strncpy(cfg->if_name, optarg, IF_NAMESIZE);
                        break;

                case 'n':

                        cfg->nec_mode = true;
                        break;


                case 'e':

                        __verbose = true;
                        cfg->__verbose = true;
                        break;


                case 'a':
                        cfg->ladoapp=true;
                        break;

                case 'h':
                default:

                        //        print_help();
                        exit(EXIT_SUCCESS);
                        break;
                }

        };
        //        if (!tic){ cfg->app_address = "255.255.255.255";
        //        cfg->app_inet_addr = inet_addr("255.255.255.255");}

//if (cfg->gn) PRF("é positivo \n");
        return(EX_OK);

}

/* check_configuration */
int check_configuration(configuration_t *cfg)
{



        if ( cfg == NULL )
        {
                handle_app_error("Configuration to be checked is NULL\n");
        }
        if ( cfg->itss_type<0 ||cfg->itss_type>15 )
                {
                        handle_app_error("ITS-S type is incorrect , must be between 0 and 15\n");
                }

        if ( ( cfg->tx_port <= 0 ) || ( cfg->rx_port <= 0 ) )
        { handle_app_error("Both TX and RX port must be set.\n"); }

        if ( strlen(cfg->if_name) <= 0 )
        { handle_app_error("Network interface name must be provided.\n"); }

        if ( cfg->app_address != NULL )
        {
                if ( strlen(cfg->app_address) <= 0 )
                { handle_app_error("Application address must be provided.\n"); }
        }

        if ( ( cfg->app_tx_port <= 0 ) || ( cfg->app_rx_port <= 0 ) )
        { handle_app_error("Both APP. TX and RX port must be set.\n"); }
        return(EX_OK);

}

/* print_configuration */
void print_configuration(const configuration_t *cfg)
{

        if ( cfg == NULL )
        { handle_app_error("Given configuration is NULL.\n"); }

        PRF(">>> Configuration = \n{\n");
        PRF("\t.app_addr = %s\n", cfg->app_address);
        PRF("\t.app_inet_addr = %.2X\n", cfg->app_inet_addr);
        PRF("\t.app_tx_port = %d\n", cfg->app_tx_port);
        PRF("\t.app_rx_port = %d\n", cfg->app_rx_port);
        PRF("\t.tx_port = %d\n", cfg->tx_port);
        PRF("\t.rx_port = %d\n", cfg->rx_port);
        PRF("\t.if_name = %s\n", cfg->if_name);
        PRF("\t.nec_mode = %s\n", cfg->nec_mode ? "true" : "false");
        PRF("\t.__tx_test = %s\n", cfg->__tx_test ? "true" : "false");
        PRF("\t.ladoapp = %s\n", cfg->ladoapp ? "true" : "false");
        PRF("\t.__verbose = %s\n", cfg->__verbose ? "true" : "false");
        PRF("\t.itss_type = %d\n", cfg->itss_type);
        PRF("}\n");

}


