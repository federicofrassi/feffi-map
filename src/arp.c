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

typedef struct __attribute__((packed)) { //attribute((packed)) = no padding
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_len;
    uint8_t protocol_len;
    uint16_t opcode;
    uint8_t sender_mac[6];
    uint32_t sender_ip;
    uint8_t target_mac[6];
    uint32_t target_ip;
} arp_header_t;


/*

	Versione 2:
		1. controllo input 						V
		2. converto target_ip con inet_pton()	V
		3. apro raw socket						V
		4. preparo un buffer per il frame Ethernet + ARP V
		5. riempio Ethernet header
		6. riempio ARP packet
		7. stampo/debuggo “ARP request built”
		8. chiudo socket
		9. return 0

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

	//creo buffer di 42 byte per pacchetto e lo azzero, poi creo le due struct
	uint8_t packet[42]; 
	memset(packet, 0, sizeof(packet)); 
	
	struct ether_header *ethernet = (struct ether_header *) packet;
	arp_header_t *arp = (arp_header_t *) (packet + sizeof(struct ether_header));
	
	//riempio l'header ethernet
	uint8_t broadcast_mac[6] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};
	memcpy(ethernet->ether_dhost, broadcast_mac, 6);

	memcpy(ethernet->ether_shost, iface->mac, 6); 

	ethernet->ether_type = htons(ETH_P_ARP);

	//riempio l'header ARP
	// ARP header
	arp->hardware_type = htons(1);          // Ethernet
	arp->protocol_type = htons(ETH_P_IP);   // IPv4
	arp->hardware_len = 6;                  // MAC length
	arp->protocol_len = 4;                  // IPv4 length
	arp->opcode = htons(1);                 // ARP request

	memcpy(arp->sender_mac, iface->mac, 6);
	arp->sender_ip = iface->ip;

	memset(arp->target_mac, 0x00, 6);
	arp->target_ip = target_addr.s_addr;
	
	//debug
	printf("ARP request buildata\n"); 

	//chiudo socket
	close(sockfd); 

	

	
	return 0; 
}



