#include "iface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int get_iface_info(const char *ifname, iface_info_t *info){
    //interface's name
    if(ifname == NULL || info == NULL){
        return -1; 
    }
    strncpy(info->name, ifname, sizeof(info->name)-1); 
    info->name[sizeof(info->name) - 1] = '\0'; 

    //interface's number
    info->if_index = if_nametoindex(ifname); 
    if(info->if_index == 0){
    	perror("Errore nell'index dell'interfaccia"); 
    	return -1; 
    }

    //MAC address
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(sockfd == -1){
        perror("Errore nella creazione della socket"); 
        return -1; 
    }
     
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr)); 
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';      
    if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) != 0) {
   	 perror("Errore ioctl MAC"); 
    	close(sockfd); 
    	return -1; 
    }
    memcpy(info->mac, ifr.ifr_hwaddr.sa_data, 6); 
     


    //IP address
    memset(&ifr, 0, sizeof(ifr)); 
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1); 
    ifr.ifr_name[sizeof(ifr.ifr_name)- 1] = '\0'; 

    if(ioctl(sockfd, SIOCGIFADDR, &ifr) != 0){
    	perror("Errore ioctl IP"); 
	close(sockfd); 
	return -1; 	
    } 
    struct sockaddr_in *ip_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    info->ip = ip_addr->sin_addr.s_addr;    close(sockfd); 
    return 0; 
}


void print_iface_info(const iface_info_t *info){
	if(info == NULL){
		return; 
	}
	//print interface's name
	printf("Interface: %s\n", info->name); 

	//print interface's number
	printf("Interface index: %d\n", info->if_index); 

	//print MAC address
	printf("MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n", info->mac[0], info->mac[1], info->mac[2], info->mac[3], info->mac[4], info->mac[5]); 
	
	//print IP address
	char ip_str[INET_ADDRSTRLEN];
	struct in_addr addr;

	addr.s_addr = info->ip;

	if (inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str)) == NULL) {
   	 	perror("inet_ntop");
    		return;
	}

	printf("IP address: %s\n", ip_str);
}
