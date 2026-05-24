#include "iface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

int get_iface_info(const char *ifname, iface_info_t *info){
    //interface's name
    if(ifname == NULL || info == NULL){
        return -1; 
    }
    strncpy(info->name, ifname, sizeof(info->name)-1); 
    info->name[sizeof(info->name) - 1] = '\0'; 

    //interface's number
    info->if_index = if_nametoindex(ifname); 

    //MAC address
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(sockfd == -1){
        perror("Errore nella creazione della socket"); 
        exit(EXIT_FAILURE); 
    }
     
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr)); 
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';      

    

    return 0; 
}
