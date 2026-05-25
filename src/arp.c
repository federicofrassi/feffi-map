#include "arp.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
/*

	prima versione:
      	1. controllare input
	2. convertire target_ip con inet_pton()
	3. aprire raw socket AF_PACKET
	4. se riesce, stampare/debuggare "raw socket created"
	5. chiudere socket
	6. return 0

*/


int arp_scan_host(const iface_info_t *iface, const char *target_ip){
	//controllo iface e target ip
	if(iface == NULL || target_ip == NULL){
		return -1; 
	}
	//creo la struct in_addr per poter convertire l'ip da stringa a binario
	struct in_addr target_addr;
	if(inet_pton(AF_INET, target_ip, &target_addr) != 1){
		printf("Errore in conversione ip target\n"); 
		return -1; 
	}

	//apro raw socket
	int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if(sockfd == -1){
		printf("Errore creazione raw socket\n"); 
	return -1; 
	} else {
		printf("Socket creata (DEBUG)\n"); 
	}	
	close(sockfd); 

	
	return 0; 
}



