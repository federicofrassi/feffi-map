#ifndef IFACE_H
#define IFACE_H

#include <net/if.h>
#include <stdint.h>

typedef struct {
    char name[IFNAMSIZ]; 
    int if_index; 
    uint8_t MAC[6]; 
    uint32_t ip; 
} iface_info_t; 


int get_iface_info(const char *ifname, iface_info_t *info);
void print_iface_info(const iface_info_t *info);

#endif