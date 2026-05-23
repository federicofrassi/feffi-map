#include <stdio.h>
#include <string.h>

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
    }
    else {
        fprintf(stderr, "Error: invalid command.\n\n");
        print_usage();
        return 1;
    }

    return 0; 

}