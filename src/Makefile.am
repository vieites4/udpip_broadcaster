# flags for the compiler and for the linker
AM_CFLAGS = --pedantic -std=gnu99 -Wall -O0 -g3
AM_LDFLAGS = 
# binaries to be produced
bin_PROGRAMS = udpipbroadcaster
udpipbroadcaster_SOURCES = novas/forw_algorithm.c novas/netmanagement.c novas/pkt_handling.c configuration.c main.c udpev/__NEC__gnbtpapi_udp_msg.c udpev/cb_udp_events.c udpev/udp_events.c udpev/udp_socket.c
