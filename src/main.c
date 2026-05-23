#include <stdio.h>

int main(int argc, char *argv[]){
    (void) argc;
    (void) argv; 


    printf("feffi-map 0.1\n");
    printf("A simplified version of nmap, written by feffi (me)\n"); 
    
    printf("Usage:\n");
    printf("  sudo ./feffi-map -sn -i <interface> <target>\n\n");

    printf("Examples:\n");
    printf("  sudo ./feffi-map -sn -i eth0 192.168.1.0/24\n");
    printf("  sudo ./feffi-map -sn -i wlan0 192.168.1.1\n");
    return 0; 

}