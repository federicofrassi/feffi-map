#include "iface.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_usage(void){     
    printf("Usage:\n");
    printf("  sudo ./feffi-map -sn -i <interface> <target>\n\n");
}

int main(int argc, char *argv[]){
    if (argc == 1){
        print_usage(); 
    }
    else if(argc == 5 && strcmp(argv[1], "-sn") == 0 && strcmp(argv[2], "-i") == 0){
        printf("Mode: host discovery\n"); 
        printf("Interface: %s\n", argv[3]); 
        printf("Target: %s\n", argv[4]); 

	iface_info_t info; 
	if(get_iface_info(argv[3],&info) != 0){
		perror("Errore ottenendo le info"); 
		exit(EXIT_FAILURE); 
	}
	print_iface_info(&info); 
    }
    else {
        fprintf(stderr, "Error: invalid command.\n\n");
        print_usage();
        return 1;
    }

    return 0; 

}
